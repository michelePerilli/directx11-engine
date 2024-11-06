#include "../Keyboard.h"

Keyboard::Event::Event() noexcept: type(Type::Invalid), code(0u) {
}

Keyboard::Event::Event(const Type type, const unsigned char code) noexcept: type(type), code(code) {
}

bool Keyboard::Event::IsPress() const noexcept {
    return type == Type::Press;
}

bool Keyboard::Event::IsRelease() const noexcept {
    return type == Type::Release;
}

bool Keyboard::Event::IsValid() const noexcept {
    return type != Type::Invalid;
}

unsigned char Keyboard::Event::GetCode() const noexcept {
    return code;
}

bool Keyboard::KeyIsPressed(const unsigned char keycode) const noexcept {
    return keyStates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept {
    if (!keyBuffer.empty()) {
        const Event e = keyBuffer.front();
        keyBuffer.pop();
        return e;
    }
    return {};
}

bool Keyboard::KeyIsEmpty() const noexcept {
    return keyBuffer.empty();
}

void Keyboard::FlushKey() noexcept {
    keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept {
    if (!charBuffer.empty()) {
        const char charCode = charBuffer.front();
        charBuffer.pop();
        return charCode;
    }
    return 0;
}

bool Keyboard::CharIsEmpty() const noexcept {
    return charBuffer.empty();
}

void Keyboard::FlushChar() noexcept {
    charBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept {
    FlushKey();
    FlushChar();
}

void Keyboard::EnableAutoRepeat() noexcept {
    autoRepeatEnabled = true;
}

void Keyboard::DisableAutoRepeat() noexcept {
    autoRepeatEnabled = false;
}

bool Keyboard::AutoRepeatIsEnabled() const noexcept {
    return autoRepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept {
    keyStates[keycode] = true;
    keyBuffer.emplace(Event::Type::Press, keycode);
    TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept {
    keyStates[keycode] = false;
    keyBuffer.emplace(Event::Type::Release, keycode);
    TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(const char character) noexcept {
    charBuffer.push(character);
    TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept {
    keyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T> &buffer) noexcept {
    while (buffer.size() > bufferSize) {
        buffer.pop();
    }
}