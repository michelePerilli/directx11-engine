#include "../../Engine.h"
#include "Panel.h"

// error exception helper macro
#define eException( hr ) Panel::Exception( __LINE__, __FILE__, hr )
#define CHWND_LAST_EXCEPT() Panel::Exception( __LINE__, __FILE__, GetLastError() )

Panel::PanelClass Panel::PanelClass::panelClass;

Panel::PanelClass::PanelClass() noexcept: hInst(GetModuleHandle(nullptr)) {
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WindowProcedure;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = nullptr;
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
    this->width = width;
    this->height = height;
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    AdjustWindowRect(&wr,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE);
    // create window & get hWnd

    hWnd = CreateWindow(
        PanelClass::GetName(), name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, PanelClass::GetInstance(), this
    );
    // show window
    ShowWindow(hWnd,SW_SHOWDEFAULT);
}

Panel::~Panel() {
    DestroyWindow(hWnd);
}

LRESULT Panel::WindowProcedure(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
    if (msg == WM_NCCREATE) {
        // extract ptr to window class from creation data
        const CREATESTRUCTW *const pCreate = reinterpret_cast<CREATESTRUCTW *>(lParam);
        auto *const pWnd = static_cast<Panel *>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window class
        SetWindowLongPtr(hWnd,GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd,GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Panel::HandleMsgThunk));
        // forward message to window class handler
        return HandleMsg(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Panel::HandleMsgThunk(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept {
    // retrieve ptr to window class
    auto *const pWnd = reinterpret_cast<Panel *>(GetWindowLongPtr(hWnd,GWLP_USERDATA));
    // forward message to window class handler
    return HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Panel::HandleMsg(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept {
    switch (msg) {
        // we don't want the DefProc to handle this message because
        // we want our destructor to destroy the window, so return 0 instead of break
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Panel::AssignMessageHandler(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
    auto *const pWnd = reinterpret_cast<Panel *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
}

LRESULT Panel::CommonMessageHandler(HWND hwnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
    return DefWindowProc(hwnd, message, wParam, lParam);
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