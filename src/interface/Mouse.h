#pragma once
#include <queue>

class Mouse {
    friend class Panel;

public:
    class Event {
    public:
        enum class Type {
            LPress,
            LRelease,
            RPress,
            RRelease,
            WheelUp,
            WheelDown,
            Move,
            Enter,
            Leave,
            Invalid
        };

    private:
        Type type;
        bool leftIsPressed;
        bool rightIsPressed;
        int x;
        int y;

    public:
        Event() noexcept
            : type(Type::Invalid),
              leftIsPressed(false),
              rightIsPressed(false),
              x(0),
              y(0) {
        }

        Event(Type type, const Mouse &parent) noexcept
            : type(type),
              leftIsPressed(parent.leftIsPressed),
              rightIsPressed(parent.rightIsPressed),
              x(parent.x),
              y(parent.y) {
        }

        Type GetType() const noexcept {
            return type;
        }

        std::pair<int, int> GetPos() const noexcept {
            return {x, y};
        }

        int GetPosX() const noexcept {
            return x;
        }

        int GetPosY() const noexcept {
            return y;
        }

        bool LeftIsPressed() const noexcept {
            return leftIsPressed;
        }

        bool RightIsPressed() const noexcept {
            return rightIsPressed;
        }
    };

    Mouse() = default;

    Mouse(const Mouse &) = delete;

    Mouse &operator=(const Mouse &) = delete;

    std::pair<int, int> GetPos() const noexcept {
        return {x, y};
    }

    int GetPosX() const noexcept {
        return x;
    }

    int GetPosY() const noexcept {
        return y;
    }

    bool IsInWindow() const noexcept {
        return isInWindow;
    }

    bool LeftIsPressed() const noexcept {
        return leftIsPressed;
    }

    bool RightIsPressed() const noexcept {
        return rightIsPressed;
    }


    Event Read() noexcept {
        if (!buffer.empty()) {
            const Event e = buffer.front();
            buffer.pop();
            return e;
        }
        return {};
    }

    bool IsEmpty() const noexcept {
        return buffer.empty();
    }

    void Flush() noexcept {
        buffer = std::queue<Event>();
    }

private:
    void OnMouseMove(int newx, int newy) noexcept {
        x = newx;
        y = newy;

        buffer.push(Event(Event::Type::Move, *this));
        TrimBuffer();
    }


    void OnMouseLeave() noexcept {
        isInWindow = false;
        buffer.push(Event(Event::Type::Leave, *this));
        TrimBuffer();
    }

    void OnMouseEnter() noexcept {
        isInWindow = true;
        buffer.push(Event(Event::Type::Enter, *this));
        TrimBuffer();
    }


    void OnLeftPressed(int x, int y) noexcept {
        leftIsPressed = true;

        buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
        TrimBuffer();
    }

    void OnLeftReleased(int x, int y) noexcept {
        leftIsPressed = false;

        buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
        TrimBuffer();
    }

    void OnRightPressed(int x, int y) noexcept {
        rightIsPressed = true;

        buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
        TrimBuffer();
    }

    void OnRightReleased(int x, int y) noexcept {
        rightIsPressed = false;

        buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
        TrimBuffer();
    }

    void OnWheelUp(int x, int y) noexcept {
        buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
        TrimBuffer();
    }

    void OnWheelDown(int x, int y) noexcept {
        buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
        TrimBuffer();
    }

    void TrimBuffer() noexcept {
        while (buffer.size() > bufferSize) {
            buffer.pop();
        }
    }


    void OnWheelDelta(int x, int y, int delta) noexcept {
        wheelDeltaCarry += delta;
        // generate events for every 120
        while (wheelDeltaCarry >= WHEEL_DELTA) {
            wheelDeltaCarry -= WHEEL_DELTA;
            OnWheelUp(x, y);
        }
        while (wheelDeltaCarry <= -WHEEL_DELTA) {
            wheelDeltaCarry += WHEEL_DELTA;
            OnWheelDown(x, y);
        }
    }

    static constexpr unsigned int bufferSize = 16u;
    int x;
    int y;
    bool leftIsPressed = false;
    bool rightIsPressed = false;
    bool isInWindow = false;
    int wheelDeltaCarry = 0;
    std::queue<Event> buffer;
};
