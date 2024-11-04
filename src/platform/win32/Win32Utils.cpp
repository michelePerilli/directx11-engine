#include "../../Engine.h"
#include "Win32Utils.h"

#include <codecvt>

namespace Win32::Utils {
    BOOL AddBitmap(const std::string &file, const HDC hWinDC, const INT x, const INT y) {
        BITMAP qBitmap;
        HDC hLocalDC = CreateCompatibleDC(hWinDC);
        const auto hBitmap = static_cast<HBITMAP>(
            LoadImage(nullptr, file.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
        if (hBitmap == nullptr) {
            MessageBox(nullptr, "LoadImage Failed", "Error", MB_OK);
            return false;
        }
        GetObject(hBitmap, sizeof(BITMAP), &qBitmap);

        SelectObject(hLocalDC, hBitmap);


        if (!BitBlt(hWinDC, x, y, qBitmap.bmWidth, qBitmap.bmHeight, hLocalDC, 0, 0, SRCCOPY)) {
            MessageBox(nullptr, "Blit Failed", "Error", MB_OK);
            return false;
        }

        // Unitialize and deallocate resources
        DeleteDC(hLocalDC);
        DeleteObject(hBitmap);
        return true;
    }
}
