#include "../../Engine.h"
#include "SubObject.h"

#include <utility>

namespace Win32 {
    SubObject::SubObject(std::string className, std::string classTitle, HICON icon)
        : m_Class(std::move(className)), m_Title(std::move(classTitle)), m_hIcon(icon), m_Handle(nullptr) {
    }

    SubObject::~SubObject() = default;


    VOID SubObject::RegisterNewClass() {
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
        wcex.hIcon = m_hIcon;
        wcex.hIconSm = m_hIcon;
        wcex.lpszClassName = m_Class.c_str();
        wcex.lpszMenuName = nullptr;
        wcex.hInstance = HInstance();
        wcex.lpfnWndProc = SetupMessageHandler;
        RegisterClassEx(&wcex);
    }

    LRESULT SubObject::SetupMessageHandler(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
        if (msg == WM_NCCREATE) {
            const CREATESTRUCTW *const pCreate = reinterpret_cast<CREATESTRUCTW *>(lParam);
            auto *const pWnd = static_cast<SubObject *>(pCreate->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&SubObject::AssignMessageHandler));
            return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    LRESULT SubObject::AssignMessageHandler(HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
        auto *const pWnd = reinterpret_cast<SubObject *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
    }

    LRESULT SubObject::CommonMessageHandler(HWND hwnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

}
