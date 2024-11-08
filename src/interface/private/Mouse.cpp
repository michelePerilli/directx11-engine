#include "../Mouse.h"

Mouse::Event::Type Mouse::Event::GetType() const noexcept {
    return type;
}

std::pair<int, int> Mouse::Event::GetPos() const noexcept {
    return {x, y};
}

int Mouse::Event::GetPosX() const noexcept {
    return x;
}

int Mouse::Event::GetPosY() const noexcept {
    return y;
}

bool Mouse::Event::LeftIsPressed() const noexcept {
    return leftIsPressed;
}

bool Mouse::Event::RightIsPressed() const noexcept {
    return rightIsPressed;
}

std::pair<int, int> Mouse::GetPos() const noexcept {
    return {x, y};
}

int Mouse::GetPosX() const noexcept {
    return x;
}

int Mouse::GetPosY() const noexcept {
    return y;
}

bool Mouse::IsInWindow() const noexcept {
    return isInWindow;
}

bool Mouse::LeftIsPressed() const noexcept {
    return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept {
    return rightIsPressed;
}

Mouse::Event Mouse::Read() noexcept {
    if (!buffer.empty()) {
        const Event e = buffer.front();
        buffer.pop();
        return e;
    }
    return {};
}

bool Mouse::IsEmpty() const noexcept {
    return buffer.empty();
}

void Mouse::Flush() noexcept {
    buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept {
    this->x = x;
    this->y = y;

    buffer.emplace(Event::Type::Move, *this);
    TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept {
    isInWindow = false;
    buffer.emplace(Event::Type::Leave, *this);
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept {
    isInWindow = true;
    buffer.emplace(Event::Type::Enter, *this);
    TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept {
    leftIsPressed = true;

    buffer.emplace(Event::Type::LPress, *this);
    TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept {
    leftIsPressed = false;

    buffer.emplace(Event::Type::LRelease, *this);
    TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept {
    rightIsPressed = true;

    buffer.emplace(Event::Type::RPress, *this);
    TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept {
    rightIsPressed = false;

    buffer.emplace(Event::Type::RRelease, *this);
    TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept {
    buffer.emplace(Event::Type::WheelUp, *this);
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept {
    buffer.emplace(Event::Type::WheelDown, *this);
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept {
    while (buffer.size() > bufferSize) {
        buffer.pop();
    }
}

void Mouse::OnWheelDelta(const int x, const int y, const int delta) noexcept {
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