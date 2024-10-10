#pragma once

namespace Win32 {

    namespace Utils {

        BOOL __declspec(dllexport) AddBitmap(CONST WCHAR* szFileName, HDC hWinDC, INT x = 0, INT y = 0);

    }
}