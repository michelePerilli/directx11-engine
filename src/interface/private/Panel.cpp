#include "../Panel.h"

#include "../../component/Logger.h"
#include "../../component/imgui/imgui_impl_win32.h"
#include "assimp/Logger.hpp"


Panel::PanelClass Panel::PanelClass::panelClass;

Panel::PanelClass::PanelClass() noexcept: hInst(GetModuleHandle(nullptr)) {
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(101), IMAGE_ICON, 16, 16, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(101), IMAGE_ICON, 16, 16, 0));
    RegisterClassEx(&wc);
}

Panel::PanelClass::~PanelClass() {
    UnregisterClass(panelClassName, GetInstance());
}

const char *Panel::PanelClass::GetName() noexcept {
    return panelClassName;
}

HINSTANCE Panel::PanelClass::GetInstance() noexcept {
    return panelClass.hInst;
}

Panel::Panel(const char *name, const int width, const int height)
    : width(width),
      height(height) {
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if (AdjustWindowRect(&wr,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE) == 0) {
        throw std::runtime_error("Failed to get window size");
    };

    hWnd = CreateWindow(
        PanelClass::GetName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, PanelClass::GetInstance(), this
    );

    if (hWnd == nullptr) {
        throw std::runtime_error("Failed to create window");
    }

    ImGui_ImplWin32_Init(hWnd);
    pGfx = std::make_unique<Graphics>(hWnd, width, height);

    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    rid.hwndTarget = nullptr;
    if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == false) {
        throw THROW_WINDOW_EXCEPTION("Disastro");
    }
}

Panel::~Panel() {
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(hWnd);
}

void Panel::SetTitle(const std::string &title) const {
    if (SetWindowText(hWnd, title.c_str()) == 0) {
        throw std::runtime_error("Failed to set title");
    }
}

void Panel::Open() const {
    ShowWindow(hWnd,SW_SHOWDEFAULT);
}

void Panel::Close() const {
    CloseWindow(hWnd);
}

void Panel::EnableCursor() {
    cursorEnabled = true;
    ShowCursor();
    EnableImGuiCursor();
    FreeCursor();
}

void Panel::DisableCursor() {
    cursorEnabled = false;
    HideCursor();
    DisableImGuiCursor();
    ConfineCursor();
}

bool Panel::CursorEnabled() {
    return cursorEnabled;
}

void Panel::EnableImGuiCursor() {
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Panel::DisableImGuiCursor() {
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

void Panel::ConfineCursor() {
    RECT rect;
    GetClientRect(hWnd, &rect);
    MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT *>(&rect), 2);
    ClipCursor(&rect);
}

void Panel::FreeCursor() {
    ClipCursor(nullptr);
}

// todo per uso futuro.
std::optional<int> Panel::ProcessWindowMessages() const {
    MSG msg;
    while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return msg.wParam;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
}

Graphics &Panel::Gfx() {
    return *pGfx;
}


LRESULT CALLBACK Panel::HandleMsgSetup(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept {
    if (msg == WM_NCCREATE) {
        // extract ptr to window class from creation data
        const CREATESTRUCTW *const pCreate = reinterpret_cast<CREATESTRUCTW *>(lParam);
        auto *const pWnd = static_cast<Panel *>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(hWnd,GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd,GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Panel::HandleMsgThunk));
        // forward message to window class handler
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Panel::HandleMsgThunk(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept {
    // retrieve ptr to window class
    auto *const pWnd = reinterpret_cast<Panel *>(GetWindowLongPtr(hWnd,GWLP_USERDATA));
    // forward message to window class handler
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Panel::HandleMsg(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

    const POINTS pt = MAKEPOINTS(lParam);
    switch (msg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_KILLFOCUS:
            keyboard.ClearState();
            break;
        case WM_ACTIVATE:
            Logger::info("window activated");
            if (!cursorEnabled) {
                if (wParam & WA_ACTIVE || wParam & WA_CLICKACTIVE) {
                    Logger::info("window activated => confine");
                    ConfineCursor();
                    HideCursor();
                } else {
                    Logger::info("window activated => free");
                    FreeCursor();
                    ShowCursor();
                }
            }
            break;

        /*********** KEYBOARD MESSAGES ***********/
        case WM_KEYDOWN:
        // syskey commands need to be handled to track ALT key (VK_MENU) and F10
        case WM_SYSKEYDOWN:

            if (ImGui::GetIO().WantCaptureKeyboard) {
                break;
            }
            if (!(lParam & 0x40000000) || keyboard.AutoRepeatIsEnabled()) // filter autorepeat
            {
                keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (ImGui::GetIO().WantCaptureKeyboard) {
                break;
            }
            keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
            break;
        case WM_CHAR:
            if (ImGui::GetIO().WantCaptureKeyboard) {
                break;
            }
            keyboard.OnChar(static_cast<char>(wParam));
            break;

        /*********** END KEYBOARD MESSAGES ***********/

        /************** MOUSE MESSAGES ***************/
        case WM_MOUSEMOVE:
            // cursorless exclusive gets first dibs
            if (!cursorEnabled) {
                if (!mouse.IsInWindow()) {
                    SetCapture(hWnd);
                    mouse.OnMouseEnter();
                    HideCursor();
                }
                break;
            }
            if (ImGui::GetIO().WantCaptureMouse) {
                break;
            }
            if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
                mouse.OnMouseMove(pt.x, pt.y);
                if (!mouse.IsInWindow()) {
                    SetCapture(hWnd);
                    mouse.OnMouseEnter();
                }
            } else {
                if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
                    mouse.OnMouseMove(pt.x, pt.y);
                }
                // button up -> release capture / log event for leaving
                else {
                    ReleaseCapture();
                    mouse.OnMouseLeave();
                }
            }
        // mouse.OnMouseMove(pt.x, pt.y);
            break;
        case WM_LBUTTONDOWN:
            SetForegroundWindow(hWnd);

            if (!cursorEnabled) {
                Logger::info("window activated => confine");
                ConfineCursor();
                HideCursor();
            }

            if (ImGui::GetIO().WantCaptureMouse) {
                break;
            }
            mouse.OnLeftPressed(pt.x, pt.y);
            break;
        case WM_LBUTTONUP:
            if (ImGui::GetIO().WantCaptureMouse) {
                break;
            }
            mouse.OnLeftReleased(pt.x, pt.y);
            break;
        case WM_RBUTTONDOWN:
            if (ImGui::GetIO().WantCaptureMouse) {
                break;
            }
            mouse.OnRightPressed(pt.x, pt.y);
            break;
        case WM_RBUTTONUP:
            if (ImGui::GetIO().WantCaptureMouse) {
                break;
            }
            mouse.OnRightReleased(pt.x, pt.y);
            break;
        case WM_MOUSEWHEEL:
            if (ImGui::GetIO().WantCaptureMouse) {
                break;
            }
            if (GET_WHEEL_DELTA_WPARAM(wParam) >= 0) {
                mouse.OnWheelUp(pt.x, pt.y);
            } else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
                mouse.OnWheelDown(pt.x, pt.y);
            }
            break;
        case WM_MBUTTONDOWN:
            if (ImGui::GetIO().WantCaptureMouse) {
                break;
            }
        // unimplemented yet
            break;
        case WM_MBUTTONUP:
            if (ImGui::GetIO().WantCaptureMouse) {
                break;
            }
        // unimplemented yet
            break;
        /************ END MOUSE MESSAGES *************/
        /************ RAW MOUSE MESSAGES *************/
        case WM_INPUT: {
            if (!mouse.RawEnabled()) {
                break;
            }
            UINT size;
            // first get the size of the input data
            if (GetRawInputData(
                    reinterpret_cast<HRAWINPUT>(lParam),
                    RID_INPUT,
                    nullptr,
                    &size,
                    sizeof(RAWINPUTHEADER)) == -1) {
                // bail msg processing if error
                break;
            }
            rawBuffer.resize(size);
            // read in the input data
            if (GetRawInputData(
                    reinterpret_cast<HRAWINPUT>(lParam),
                    RID_INPUT,
                    rawBuffer.data(),
                    &size,
                    sizeof(RAWINPUTHEADER)) != size) {
                // bail msg processing if error
                break;
            }
            // process the raw input data
            auto &ri = reinterpret_cast<const RAWINPUT &>(*rawBuffer.data());
            if (ri.header.dwType == RIM_TYPEMOUSE &&
                (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0)) {
                mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
            }
            break;
        }
        default: ;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Panel::HideCursor() {
    while (::ShowCursor(FALSE) >= 0);
}

void Panel::ShowCursor() {
    while (::ShowCursor(TRUE) < 0);
}


HWND Panel::GetHandle() const {
    return hWnd;
}
