#include "../../Engine.h"
#include "Win32Utils.h"

#include <codecvt>

namespace Win32 {

    namespace Utils {

        BOOL AddBitmap(const WCHAR* szFileName, HDC hWinDC, INT x, INT y)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

            BITMAP qBitmap;
            HDC hLocalDC = CreateCompatibleDC(hWinDC);
            std::string str = converter.to_bytes(szFileName);
            HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, str.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            if (hBitmap == NULL) {
                MessageBox(NULL, "LoadImage Failed", "Error", MB_OK);
                return false;
            }
            GetObject(hBitmap, sizeof(BITMAP), &qBitmap);

            SelectObject(hLocalDC, hBitmap);


            if (!BitBlt(hWinDC, x, y, qBitmap.bmWidth, qBitmap.bmHeight, hLocalDC, 0, 0, SRCCOPY)) {
                MessageBox(NULL, "Blit Failed", "Error", MB_OK);
                return false;
            }

            // Unitialize and deallocate resources
            ::DeleteDC(hLocalDC);
            ::DeleteObject(hBitmap);
            return true;
        }

    }
}
