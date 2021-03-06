// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "Types.h"
#include "Vector2.h"
#include "Size2.h"
#include "Input.h"

namespace ouzel
{
    enum EventModifiers
    {
        SHIFT_DOWN          = 0x0001,
        ALT_DOWN            = 0x0002,
        CONTROL_DOWN        = 0x0004,
        COMMAND_DOWN        = 0x0008,
        FUNCTION_DOWN       = 0x0010,
        LEFT_MOUSE_DOWN     = 0x0020,
        RIGHT_MOUSE_DOWN    = 0x0040,
        MIDDLE_MOUSE_DOWN   = 0x0080,
    };

    struct KeyboardEvent
    {
        uint32_t modifiers = 0;
        input::KeyboardKey key = input::KeyboardKey::NONE;
    };

    struct MouseEvent
    {
        uint32_t modifiers = 0;
        input::MouseButton button = input::MouseButton::NONE;
        Vector2 position;
        Vector2 scroll;
    };

    struct TouchEvent
    {
        uint64_t touchId = 0;
        Vector2 position;
    };

    struct GamepadEvent
    {
        input::GamepadPtr gamepad;
        input::GamepadButton button = input::GamepadButton::NONE;
        bool pressed = false;
        float value = 0.0f;
    };

    struct WindowEvent
    {
        Size2 size;
        std::string title;
        bool fullscreen = false;
    };

    struct SystemEvent
    {
        Size2 size;
        std::string filename;
    };

    struct UIEvent
    {
        Vector2 position;
    };

    struct Event
    {
        enum class Type
        {
            KEY_DOWN,
            KEY_UP,
            KEY_REPEAT,
            MOUSE_DOWN,
            MOUSE_UP,
            MOUSE_SCROLL,
            MOUSE_MOVE,
            TOUCH_BEGIN,
            TOUCH_MOVE,
            TOUCH_END,
            TOUCH_CANCEL,
            GAMEPAD_CONNECT,
            GAMEPAD_DISCONNECT,
            GAMEPAD_BUTTON_CHANGE,
            WINDOW_SIZE_CHANGE,
            WINDOW_TITLE_CHANGE,
            WINDOW_FULLSCREEN_CHANGE,
            LOW_MEMORY,
            OPEN_FILE,

            UI_ENTER_NODE, // mouse or touch entered the scene node
            UI_LEAVE_NODE, // mouse or touch left the scene node
            UI_PRESS_NODE, // mouse or touch pressed on scene node
            UI_RELEASE_NODE,  // mouse or touch released on scene node
            UI_CLICK_NODE, // mouse or touch clicked on scene node
            UI_DRAG_NODE, // mouse or touch dragged scene node
        };

        Type type;
        VoidPtr sender;

        KeyboardEvent keyboardEvent;
        MouseEvent mouseEvent;
        TouchEvent touchEvent;
        GamepadEvent gamepadEvent;
        WindowEvent windowEvent;
        SystemEvent systemEvent;
        UIEvent uiEvent;
    };
}
