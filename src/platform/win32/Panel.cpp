#include "../../Engine.h"
#include "Panel.h"
#include <sstream>

// error exception helper macro
#define eException( hr ) Panel::Exception( __LINE__, __FILE__, hr )
#define CHWND_LAST_EXCEPT() Panel::Exception( __LINE__, __FILE__, GetLastError() )

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

Panel::Panel(const char *name, const int width, const int height) {
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if (AdjustWindowRect(&wr,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE) == 0) {
        throw CHWND_LAST_EXCEPT();
    };

    hWnd = CreateWindow(
        PanelClass::GetName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, PanelClass::GetInstance(), this
    );
}

Panel::~Panel() {
    DestroyWindow(hWnd);
}

void Panel::SetTitle(const std::string &title) const {
    if (SetWindowText(hWnd, title.c_str()) == 0) {
        throw CHWND_LAST_EXCEPT();
    }
}

void Panel::Open() const {
    ShowWindow(hWnd,SW_SHOWDEFAULT);
}

void Panel::Close() const {
    CloseWindow(hWnd);
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
    const POINTS pt = MAKEPOINTS(lParam);
    switch (msg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_KILLFOCUS:
            keyboard.ClearState();
            break;

        /*********** KEYBOARD MESSAGES ***********/
        case WM_KEYDOWN:
        // syskey commands need to be handled to track ALT key (VK_MENU) and F10
        case WM_SYSKEYDOWN:
            if (!(lParam & 0x40000000) || keyboard.AutorepeatIsEnabled()) // filter autorepeat
            {
                keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
            break;
        case WM_CHAR:
            keyboard.OnChar(static_cast<char>(wParam));
            break;

        /*********** END KEYBOARD MESSAGES ***********/

        /************** MOUSE MESSAGES ***************/
        case WM_MOUSEMOVE:
            mouse.OnMouseMove(pt.x, pt.y);
            break;
        case WM_LBUTTONDOWN:
            mouse.OnLeftPressed(pt.x, pt.y);
            break;
        case WM_LBUTTONUP:
            mouse.OnLeftReleased(pt.x, pt.y);
            break;
        case WM_RBUTTONDOWN:
            mouse.OnRightPressed(pt.x, pt.y);
            break;
        case WM_RBUTTONUP:
            mouse.OnRightReleased(pt.x, pt.y);
            break;
        case WM_MOUSEWHEEL:
            if (GET_WHEEL_DELTA_WPARAM(wParam) >= 0) {
                mouse.OnWheelUp(pt.x, pt.y);
            } else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
                mouse.OnWheelDown(pt.x, pt.y);
            }
            break;
        case WM_MBUTTONDOWN:
            // unimplemented yet
            break;
        case WM_MBUTTONUP:
            // unimplemented yet
            break;
        /************ END MOUSE MESSAGES *************/

        default: ;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}


HWND Panel::GetHandle() const {
    return hWnd;
}

// Window Exception Stuff
inline Panel::Exception::Exception(int line, const char *file, HRESULT hr) noexcept: EngineException(line, file),
    hr(hr) {
}

inline const char *Panel::Exception::what() const noexcept {
    std::ostringstream oss;
    oss << GetOriginString() << " " << GetErrorString();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

inline std::string Panel::Exception::TranslateErrorCode(HRESULT hr) noexcept {
    char *pMsgBuf = nullptr;
    // windows will allocate memory for err string and make our pointer point to it
    const DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
    );
    // 0 string length returned indicates a failure
    if (nMsgLen == 0) {
        return "Unidentified error code";
    }
    // copy error string from windows-allocated buffer to std::string
    std::string errorString = pMsgBuf;
    // free windows buffer
    LocalFree(pMsgBuf);
    return errorString;
}

inline HRESULT Panel::Exception::GetErrorCode() const noexcept {
    return hr;
}

inline std::string Panel::Exception::GetErrorString() const noexcept {
    return TranslateErrorCode(hr);
}
