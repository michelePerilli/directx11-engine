#pragma once

#include "SubObject.h"

namespace Win32 {
    class ENGINE_DLL Window : public SubObject {
    public:
        Window(const std::string &className, const std::string &classTitle, HICON icon, INT width, INT height);

        ~Window() override;

        VOID Initialize() override;

    protected:
        INT m_Width;
        INT m_Height;
    };
}
