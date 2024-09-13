#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <Xinput.h>
#include <wrl/client.h>

#include <cstdint>
#include <cmath>

#define MOUSE_BUTTON_LEFT  0
#define MOUSE_BUTTON_RIGHT 1
#define MOUSE_BUTTON_MID   2

#define MOUSE_CLICKED_VALUE 0x80

struct GamepadButton {
    enum Enum {
        UpArrow = XINPUT_GAMEPAD_DPAD_UP,
        DownArrow = XINPUT_GAMEPAD_DPAD_DOWN,
        LeftArrow = XINPUT_GAMEPAD_DPAD_LEFT,
        RightArrow = XINPUT_GAMEPAD_DPAD_RIGHT,
        Start = XINPUT_GAMEPAD_START,
        Back = XINPUT_GAMEPAD_BACK,
        LeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
        RightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
        LeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
        RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
        A = XINPUT_GAMEPAD_A,
        B = XINPUT_GAMEPAD_B,
        X = XINPUT_GAMEPAD_X,
        Y = XINPUT_GAMEPAD_Y,
    };
};

class Input {
public:

    static Input* GetInstance();

    void Initialize(HWND hWnd);
    void Update();

    bool IsKeyPressed(unsigned char keycode) const { return keys_[keycode] != 0; }
    bool IsKeyTrigger(unsigned char keycode) const { return keys_[keycode] != 0 && preKeys_[keycode] == 0; }
    bool IsKeyRelease(unsigned char keycode) const { return keys_[keycode] == 0 && preKeys_[keycode] != 0; }

    bool IsMousePressed(int button) const { return mouseState_.state.rgbButtons[button] & MOUSE_CLICKED_VALUE; }
    bool IsMouseTrigger(int button) const { return (mouseState_.state.rgbButtons[button] & MOUSE_CLICKED_VALUE) && !(preMouseState_.state.rgbButtons[button] & MOUSE_CLICKED_VALUE); }
    bool IsMouseRelease(int button) const { return !(mouseState_.state.rgbButtons[button] & MOUSE_CLICKED_VALUE) && (preMouseState_.state.rgbButtons[button] & MOUSE_CLICKED_VALUE); }
    POINT GetMousePosition() const { return mouseState_.screenPos; }

    LONG GetMouseMoveX() const { return mouseState_.state.lX; }
    LONG GetMouseMoveY() const { return mouseState_.state.lY; }
    LONG GetMouseWheel() const { return mouseState_.state.lZ; }

    bool IsGamepadButtonPressed(GamepadButton::Enum gamepadButton) const { return xInputState_.Gamepad.wButtons & gamepadButton; }
    bool IsGamepadButtonTrigger(GamepadButton::Enum gamepadButton) const { return (xInputState_.Gamepad.wButtons & gamepadButton) && !(preXInputState_.Gamepad.wButtons & gamepadButton); }
    bool IsGamepadButtonRelease(GamepadButton::Enum gamepadButton) const { return !(xInputState_.Gamepad.wButtons & gamepadButton) && (preXInputState_.Gamepad.wButtons & gamepadButton); }

    void GetRStick(float& x,float& y) const {
        if (std::fabs(xInputState_.Gamepad.sThumbRX) <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
            x = 0.0f;
        }
        else {
            x = static_cast<float>(xInputState_.Gamepad.sThumbRX);
        }
        if (std::fabs(xInputState_.Gamepad.sThumbRY) <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
            y = 0.0f;
        }
        else {
            y = static_cast<float>(xInputState_.Gamepad.sThumbRY);
        }
    }

    void GetLStick(float& x, float& y) const {
        if (std::fabs(xInputState_.Gamepad.sThumbLX) <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
            x = 0.0f;
        }
        else {
            x = static_cast<float>(xInputState_.Gamepad.sThumbLX);
        }
        if (std::fabs(xInputState_.Gamepad.sThumbLY) <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
            y = 0.0f;
        }
        else {
            y = static_cast<float>(xInputState_.Gamepad.sThumbLY);
        }
    }

    const XINPUT_STATE& GetXInputState() const { return xInputState_; }
    const XINPUT_STATE& GetPreXInputState() const { return preXInputState_; }

private:
    static const uint32_t kNumKeys = 256;
   
    Input() = default;
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    struct MouseState {
        DIMOUSESTATE state;
        POINT screenPos;
    };

    HWND hWnd_ = nullptr;
    Microsoft::WRL::ComPtr<IDirectInput8> directInput_;
    Microsoft::WRL::ComPtr<IDirectInputDevice8> keybord_;
    Microsoft::WRL::ComPtr<IDirectInputDevice8> mouse_;

    uint8_t keys_[kNumKeys]{};
    uint8_t preKeys_[kNumKeys]{};

    MouseState mouseState_{};
    MouseState preMouseState_{};

    XINPUT_STATE xInputState_{};
    XINPUT_STATE preXInputState_{};
};

#ifdef MOUSE_CLICKED_VALUE
#undef MOUSE_CLICKED_VALUE
#endif // MOUSE_CLICKED_VALUE
