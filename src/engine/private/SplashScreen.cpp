#include "../../Engine.h"
#include "../SplashScreen.h"

#include "../../platform/win32/Win32Utils.h"

namespace SplashScreen {

#define WM_OUTPUTMESSAGE (WM_USER + 0x0001)

    SplashWindow* m_SplashWindow;

    VOID Open() {
        if (m_SplashWindow != nullptr)
            return;
        // m_SplashWindow = new SplashWindow();
    }

    VOID Close() {
        return VOID();
    }

    VOID AddMessage(const std::string& message) {
        // PostMessage(m_SplashWindow->GetHandle(), WM_OUTPUTMESSAGE, reinterpret_cast<WPARAM>(message.c_str()), 0);
    }
}


// SplashWindow::SplashWindow() : Panel("SplashScreen", 1280, 720) {
//     m_outputMessage = "SplashScreen Starting...";
// }


// LRESULT SplashWindow::MessageHandler(HWND hwnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
//     switch (message) {
//         case WM_PAINT: {
//             PAINTSTRUCT ps;
//
//             HDC hdc = BeginPaint(hwnd, &ps);
//
//             Win32::Utils::AddBitmap(PerGameSettings::SplashURL(), hdc);
//
//             SetBkMode(hdc, TRANSPARENT);
//             SetTextColor(hdc, RGB(255, 255, 255));
//
//             // if (GetMode() != RELEASE) {
//                 const std::string engineModeText = EngineModeToString() + " Mode";
//                 SetTextAlign(hdc, TA_RIGHT);
//                 TextOut(hdc, width - 15, 15, engineModeText.c_str(), strlen(engineModeText.c_str()));
//             // }
//
//             SetTextAlign(hdc, TA_CENTER);
//
//             TextOutA(hdc, width / 2, height - 30, m_outputMessage.c_str(), strlen(m_outputMessage.c_str()));
//             EndPaint(hwnd, &ps);
//
//         }
//         break;
//         case WM_OUTPUTMESSAGE: {
//             const auto* msg = reinterpret_cast<char *>(wParam);
//             m_outputMessage = msg;
//             RedrawWindow(GetHandle(), nullptr, nullptr, RDW_INVALIDATE);
//             return 0;
//         }
//         default: ;
//     }
//
//     return CommonMessageHandler(hwnd, message, wParam, lParam);
// }