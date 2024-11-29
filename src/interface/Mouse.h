#pragma once
#include <optional>
#include <queue>
#include "../core/CoreMacro.h"

class ENGINE_DLL Mouse {
    friend class Panel;

public:
    struct RawDelta {
        int x, y;
    };

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
            Leave
        };

    private:
        Type type;
        bool leftIsPressed;
        bool rightIsPressed;
        int x;
        int y;

    public:

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

    Mouse() = default;

    Mouse(const Mouse &) = delete;

    Mouse &operator=(const Mouse &) = delete;

    [[nodiscard]] std::pair<int, int> GetPos() const noexcept;

    std::optional<RawDelta> ReadRawDelta() noexcept;

    std::optional<Mouse::Event> Read() noexcept;

    [[nodiscard]] int GetPosX() const noexcept;

    [[nodiscard]] int GetPosY() const noexcept;

    [[nodiscard]] bool IsInWindow() const noexcept;

    [[nodiscard]] bool LeftIsPressed() const noexcept;

    [[nodiscard]] bool RightIsPressed() const noexcept;

    [[nodiscard]] bool IsEmpty() const noexcept;

    void Flush() noexcept;

    void EnableRaw() noexcept;
    void DisableRaw() noexcept;
    bool RawEnabled() const noexcept;

private:
    static constexpr unsigned int bufferSize = 16u;
    int x = 0;
    int y = 0;
    bool leftIsPressed = false;
    bool rightIsPressed = false;
    bool isInWindow = false;
    int wheelDeltaCarry = 0;
    bool rawEnabled = false;
    std::queue<Event> buffer;
    std::queue<RawDelta> rawBuffer;

    void OnMouseMove(int x, int y) noexcept;

    void OnMouseLeave() noexcept;

    void OnMouseEnter() noexcept;

    void OnRawDelta(int dx, int dy) noexcept;

    void OnLeftPressed(int x, int y) noexcept;

    void OnLeftReleased(int x, int y) noexcept;

    void OnRightPressed(int x, int y) noexcept;

    void OnRightReleased(int x, int y) noexcept;

    void OnWheelUp(int x, int y) noexcept;

    void OnWheelDown(int x, int y) noexcept;

    void TrimBuffer() noexcept;

    void TrimRawBuffer() noexcept;

    void OnWheelDelta(int x, int y, int delta) noexcept;
};
