#pragma once
#include <queue>
#include "../core/Core.h"

class ENGINE_DLL Mouse {
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

        Event(const Type type, const Mouse &parent) noexcept
            : type(type),
              leftIsPressed(parent.leftIsPressed),
              rightIsPressed(parent.rightIsPressed),
              x(parent.x),
              y(parent.y) {
        }

        [[nodiscard]] Type GetType() const noexcept;

        [[nodiscard]] std::pair<int, int> GetPos() const noexcept;

        [[nodiscard]] int GetPosX() const noexcept;

        [[nodiscard]] int GetPosY() const noexcept;

        [[nodiscard]] bool LeftIsPressed() const noexcept;

        [[nodiscard]] bool RightIsPressed() const noexcept;
    };

    Event Read() noexcept;

    Mouse() = default;

    Mouse(const Mouse &) = delete;

    Mouse &operator=(const Mouse &) = delete;

    [[nodiscard]] std::pair<int, int> GetPos() const noexcept;

    [[nodiscard]] int GetPosX() const noexcept;

    [[nodiscard]] int GetPosY() const noexcept;

    [[nodiscard]] bool IsInWindow() const noexcept;

    [[nodiscard]] bool LeftIsPressed() const noexcept;

    [[nodiscard]] bool RightIsPressed() const noexcept;

    [[nodiscard]] bool IsEmpty() const noexcept;

    void Flush() noexcept;



private:

    static constexpr unsigned int bufferSize = 16u;
    int x = 0;
    int y = 0;
    bool leftIsPressed = false;
    bool rightIsPressed = false;
    bool isInWindow = false;
    int wheelDeltaCarry = 0;
    std::queue<Event> buffer;

    void OnMouseMove(int x, int y) noexcept;

    void OnMouseLeave() noexcept;

    void OnMouseEnter() noexcept;

    void OnLeftPressed(int x, int y) noexcept;

    void OnLeftReleased(int x, int y) noexcept;

    void OnRightPressed(int x, int y) noexcept;

    void OnRightReleased(int x, int y) noexcept;

    void OnWheelUp(int x, int y) noexcept;

    void OnWheelDown(int x, int y) noexcept;

    void TrimBuffer() noexcept;

    void OnWheelDelta(int x, int y, int delta) noexcept;


};

