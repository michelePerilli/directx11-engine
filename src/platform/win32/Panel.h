#pragma once

#include "../../include/WinSDK.h"
#include "../../exception/EngineException.h"
#include "../../interface/Keyboard.h"

class ENGINE_DLL Panel {

    class PanelClass {
    public:
        static const char *GetName() noexcept;

        static HINSTANCE GetInstance() noexcept;
        PanelClass(const PanelClass &) = delete;

        PanelClass &operator=(const PanelClass &) = delete;
    private:


        PanelClass() noexcept;

        ~PanelClass();

        static constexpr auto panelClassName = "Direct3D Engine Window";
        static PanelClass panelClass;
        HINSTANCE hInst;
    };
    // static LRESULT AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // static LRESULT CommonMessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    // virtual LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

    static LRESULT CALLBACK HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
    static LRESULT CALLBACK HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
    LRESULT HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;


    HWND hWnd;
public:
    Keyboard kbd = Keyboard();
    std::string text = "ciao";
    Panel(const char *name, int width, int height);
    ~Panel();
    Panel(const Panel &) = delete;
    Panel &operator=(const Panel &) = delete;

    HWND GetHandle() {
        return hWnd;
    }

    class Exception final : public EngineException {
    public:
        Exception(int line, const char *file, HRESULT hr) noexcept;

        const char *what() const noexcept override;

        static std::string TranslateErrorCode(HRESULT hr) noexcept;

        HRESULT GetErrorCode() const noexcept;

        std::string GetErrorString() const noexcept;

    private:
        HRESULT hr;
    };
};



