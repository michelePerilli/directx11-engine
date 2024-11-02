#pragma once


namespace Win32 {
    class ENGINE_DLL SubObject {
    public:
        SubObject(std::string className, std::string classTitle, HICON icon);

        virtual ~SubObject();

        virtual VOID RegisterNewClass();

        virtual VOID Initialize() = 0;

    protected:
        static LRESULT CALLBACK SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        static LRESULT AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        virtual LRESULT CommonMessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

        virtual LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

        std::string m_Class;
        std::string m_Title;

        HICON m_hIcon;

        HWND m_Handle{};

    public:
        HWND GetHandle() const { return m_Handle; }
    };
}
