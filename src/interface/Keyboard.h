#pragma once
#include <queue>
#include <bitset>
#include "../core/CoreMacro.h"

class ENGINE_DLL Keyboard {
    friend class Panel;

public:
    std::string text = "kbd";

    class Event {
    public:
        enum class Type {
            Press,
            Release,
            Invalid
        };

    private:
        Type type;
        unsigned char code;

    public:
        Event() noexcept;

        Event(Type type, unsigned char code) noexcept;

        [[nodiscard]] bool IsPress() const noexcept;

        [[nodiscard]] bool IsRelease() const noexcept;

        [[nodiscard]] bool IsValid() const noexcept;

        [[nodiscard]] unsigned char GetCode() const noexcept;
    };

    Keyboard() = default;

    Keyboard(const Keyboard &) = delete;

    Keyboard &operator=(const Keyboard &) = delete;

    // key event stuff
    bool KeyIsPressed(unsigned char keycode) const noexcept;

    Event ReadKey() noexcept;

    bool KeyIsEmpty() const noexcept;

    void FlushKey() noexcept;

    // char event stuff
    char ReadChar() noexcept;

    bool CharIsEmpty() const noexcept;

    void FlushChar() noexcept;

    void Flush() noexcept;

    // auto repeat control
    void EnableAutoRepeat() noexcept;

    void DisableAutoRepeat() noexcept;

    bool AutoRepeatIsEnabled() const noexcept;

private:
    void OnKeyPressed(unsigned char keycode) noexcept;

    void OnKeyReleased(unsigned char keycode) noexcept;

    void OnChar(char character) noexcept;

    void ClearState() noexcept;


    template<typename T>
    static void TrimBuffer(std::queue<T> &buffer) noexcept;


    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;
    bool autoRepeatEnabled = false;
    std::bitset<nKeys> keyStates;
    std::queue<Event> keyBuffer;
    std::queue<char> charBuffer;
};


