#pragma once

#include <memory>
#include <optional>

#include "../../include/WinSDK.h"
#include "../../exception/EngineException.h"
#include "../../interface/Keyboard.h"
#include "../../interface/Mouse.h"
#include "../../interface/Graphics.h"

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

    static LRESULT CALLBACK HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
    static LRESULT CALLBACK HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
    LRESULT HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;


    HWND hWnd;
    std::unique_ptr<Graphics> pGfx;
public:
    Keyboard keyboard;
    Mouse mouse;
    Panel(const char *name, int width, int height);
    ~Panel();

    void SetTitle(const std::string &title) const;
    void Open() const;
    void Close() const;

    [[nodiscard]] std::optional<int> ProcessWindowMessages() const;
    Graphics &Gfx();

    Panel(const Panel &) = delete;
    Panel &operator=(const Panel &) = delete;

    [[nodiscard]] HWND GetHandle() const;

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



