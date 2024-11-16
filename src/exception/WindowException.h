#pragma once
#include "Exception.h"
#include "../core/WinSDK.h"

#define WINDOW_EXCEPTION( hr ) WindowException( __LINE__, __FILE__, hr )
#define WINDOW_EXCEPTION_LASTERROR() WindowException( __LINE__, __FILE__, GetLastError() )

class WindowException final : public Exception {
public:
    WindowException(const int line, const std::string &file, const HRESULT hr)
        : Exception(line, file, "") {
        char *pMsgBuf = nullptr;
        // windows will allocate memory for err string and make our pointer point to it
        const DWORD nMsgLen = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, hr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
        );
        // 0 string length returned indicates a failure
        if (nMsgLen == 0) {
            this->SetMessage("Unidentified error code");
        }
        // copy error string from windows-allocated buffer to std::string
        const std::string errorString = pMsgBuf;
        // free windows buffer
        LocalFree(pMsgBuf);
        this->SetMessage(errorString);
    }


};
