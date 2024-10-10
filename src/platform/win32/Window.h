#pragma once

#include "SubObject.h"

namespace Win32 {

    class __declspec(dllexport) Window : public Win32::SubObject {

    public:
        Window(std::string className, std::string classTitle, HICON icon, INT width, INT height);
        ~Window();

        virtual VOID Initialize() override;

    protected:
        INT				m_Width;
        INT				m_Height;
    };

}