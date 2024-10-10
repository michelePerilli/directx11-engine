#pragma once


namespace Win32 {

    class __declspec(dllexport) SubObject {

    public:
        SubObject(std::string className, std::string classTitle, HICON icon);
        ~SubObject();

        virtual VOID RegisterNewClass();
        virtual VOID Initialize() = 0;

    protected:
        static			LRESULT CALLBACK	SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static			LRESULT				AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
        virtual			LRESULT				CommonMessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

        virtual			LRESULT				MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

    protected:
        std::string			m_Class;
        std::string			m_Title;

        HICON					m_hIcon;

        HWND					m_Handle;
    public:
        HWND GetHandle()		{ return m_Handle; }

    };

}