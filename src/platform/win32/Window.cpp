#include "../../Engine.h"
#include "Window.h"

namespace Win32 {
    Window::Window(const std::string &className, const std::string &classTitle, HICON icon, const INT width, const INT height)
        : SubObject(className, classTitle, icon), m_Width(width), m_Height(height) {
    }

    Window::~Window() = default;

    VOID Window::Initialize() {
        RECT desktop;
        GetWindowRect(GetDesktopWindow(), &desktop);

        RECT R = {0, 0, m_Width, m_Height};
        AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

        m_Handle = CreateWindow(m_Class.c_str(),
                                m_Title.c_str(),
                                WS_POPUP,
                                desktop.right / 2 - m_Width / 2,
                                desktop.bottom / 2 - m_Height / 2,
                                m_Width,
                                m_Height,
                                nullptr,
                                nullptr,
                                HInstance(),
                                this);

        ShowWindow(m_Handle, SW_SHOW);
        UpdateWindow(m_Handle);
    }
}
