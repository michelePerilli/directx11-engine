#pragma once
#include <queue>
#include <bitset>

class Keyboard {
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
        Event() noexcept : type(Type::Invalid), code(0u) {
        }

        Event(const Type type, const unsigned char code) noexcept : type(type), code(code) {
        }

        bool IsPress() const noexcept {
            return type == Type::Press;
        }

        bool IsRelease() const noexcept {
            return type == Type::Release;
        }

        bool IsValid() const noexcept {
            return type != Type::Invalid;
        }

        unsigned char GetCode() const noexcept {
            return code;
        }
    };

    Keyboard() = default;

    Keyboard(const Keyboard &) = delete;

    Keyboard &operator=(const Keyboard &) = delete;

    // key event stuff
    bool KeyIsPressed(const unsigned char keycode) const noexcept {
        return keystates[keycode];
    }

    Event ReadKey() noexcept {
        if (!keybuffer.empty()) {
            const Event e = keybuffer.front();
            keybuffer.pop();
            return e;
        }
        return {};
    }

    bool KeyIsEmpty() const noexcept {
        return keybuffer.empty();
    }

    void FlushKey() noexcept {
        keybuffer = std::queue<Event>();
    }

    // char event stuff
    char ReadChar() noexcept {
        if (!charbuffer.empty()) {
            const char charCode = charbuffer.front();
            charbuffer.pop();
            return charCode;
        }
        return 0;
    }

    bool CharIsEmpty() const noexcept {
        return charbuffer.empty();
    }

    void FlushChar() noexcept {
        charbuffer = std::queue<char>();
    }

    void Flush() noexcept {
        FlushKey();
        FlushChar();
    }

    // autorepeat control
    void EnableAutorepeat() noexcept {
        autorepeatEnabled = true;
    }

    void DisableAutorepeat() noexcept {
        autorepeatEnabled = false;
    }

    bool AutorepeatIsEnabled() const noexcept {
        return autorepeatEnabled;
    }

private:
    void OnKeyPressed(unsigned char keycode) noexcept {
        keystates[keycode] = true;
        keybuffer.emplace(Event::Type::Press, keycode);
        TrimBuffer(keybuffer);
    }

    void OnKeyReleased(unsigned char keycode) noexcept {
        keystates[keycode] = false;
        keybuffer.emplace(Event::Type::Release, keycode);
        TrimBuffer(keybuffer);
    }

    void OnChar(char character) noexcept {
        charbuffer.push(character);
        TrimBuffer(charbuffer);
    }

    void ClearState() noexcept {
        keystates.reset();
    }


    template<typename T>
    static void TrimBuffer(std::queue<T> &buffer) noexcept {
        while (buffer.size() > bufferSize) {
            buffer.pop();
        }
    }


    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;
    bool autorepeatEnabled = false;
    std::bitset<nKeys> keystates;
    std::queue<Event> keybuffer;
    std::queue<char> charbuffer;
};
