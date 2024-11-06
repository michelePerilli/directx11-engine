#pragma once
#include <comdef.h>
#include <string>
#include "Exception.h"

#define OFT_COM_EXCEPTION( hr ) if (HRESULT result = hr; FAILED(result)) throw COMException( __LINE__, __FILE__, result )
#define COM_EXCEPTION_LASTERROR() COMException( __LINE__, __FILE__, GetLastError() )

class COMException final : public Exception {
public:
    COMException(const int line, const std::string &file, const HRESULT hr)
        : Exception(line, file, "") {
        const _com_error error(hr);
        this->SetMessage(error.ErrorMessage());
    }

    [[nodiscard]] std::string Print() const override {
        std::ostringstream oss;
        oss << "COMException: " << Exception::Print();
        return oss.str();
    };

};
