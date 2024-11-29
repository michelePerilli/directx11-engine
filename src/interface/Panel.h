#pragma once

#include <memory>
#include <optional>

#include "../exception/COMException.h"
#include "../interface/Keyboard.h"
#include "../interface/Mouse.h"
#include "../interface/Graphics.h"



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
    void HideCursor();
    void ShowCursor();
    void EnableImGuiCursor();
    void DisableImGuiCursor();
    HWND hWnd;
    std::unique_ptr<Graphics> pGfx;
    int width;
    int height;
    bool cursorEnabled = true;
    std::vector<char> rawBuffer;
public:
    Keyboard keyboard;
    Mouse mouse;
    Panel(const char *name, int width, int height);
    ~Panel();

    void SetTitle(const std::string &title) const;
    void Open() const;
    void Close() const;

    void EnableCursor();
    void DisableCursor();
    bool CursorEnabled();

    void ConfineCursor();
    void FreeCursor();

    [[nodiscard]] std::optional<int> ProcessWindowMessages() const;
    Graphics &Gfx();

    Panel(const Panel &) = delete;
    Panel &operator=(const Panel &) = delete;

    [[nodiscard]] HWND GetHandle() const;

};



