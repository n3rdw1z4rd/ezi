/**
 * MIT License
 *
 * Copyright (c) 2021 John Wakley
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/

#include "input.hpp"

#include <chrono>

namespace ezi
{
    Input::Input() : EventEmitter() {}

    Input::~Input() {}

    void Input::init(GLFWwindow *window)
    {
        glfwSetWindowUserPointer(window, this);
        glfwSetKeyCallback(window, &Input::_keyCallback);
        glfwSetMouseButtonCallback(window, &Input::_mouseButtonCallback);
        glfwSetScrollCallback(window, &Input::_mouseWheelCallback);
        glfwSetCursorPosCallback(window, &Input::_mousePositionCallback);
    }

    void Input::_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        static_cast<Input *>(glfwGetWindowUserPointer(window))->_onKeyboardEvent(key, scancode, action, mods);
    }

    void Input::_mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
    {
        static_cast<Input *>(glfwGetWindowUserPointer(window))->_onMouseButtonEvent(button, action, mods);
    }

    void Input::_mouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        static_cast<Input *>(glfwGetWindowUserPointer(window))->_onMouseWheelEvent(xoffset, yoffset);
    }

    void Input::_mousePositionCallback(GLFWwindow *window, double xpos, double ypos)
    {
        static_cast<Input *>(glfwGetWindowUserPointer(window))->_onMouseMoveEvent(xpos, ypos);
    }

    long long _now()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
            .count();
    }

    void Input::_onKeyboardEvent(int key, int scancode, int action, int mods)
    {
        long long now = _now();

        if (action == GLFW_PRESS)
        {
            keyDown[key] = true;
            keyDownTime[key] = now;

            emit("key_down", key, mods);
        }
        else if (action == GLFW_RELEASE)
        {
            keyDown[key] = false;

            if (now - keyDownTime[key] < downTimeThreshold)
            {
                emit("key_pressed", key, mods);
            }
            else
            {
                emit("key_up", key, mods);
            }

            keyDownTime[key] = 0;
        }
    }

    void Input::_onMouseButtonEvent(int button, int action, int mods)
    {
        long long now = _now();

        if (action == GLFW_PRESS)
        {
            buttonDown[button] = true;
            buttonDownTime[button] = now;

            emit("button_down", button, mods);
        }
        else if (action == GLFW_RELEASE)
        {
            buttonDown[button] = false;

            if (now - buttonDownTime[button] < downTimeThreshold)
            {
                emit("button_clicked", button, mods);
            }
            else
            {
                emit("button_up", button, mods);
            }

            buttonDownTime[button] = 0;
        }
    }

    void Input::_onMouseWheelEvent(double xoffset, double yoffset)
    {
        if (xoffset > 0)
        {
            emit("wheel_left", (int)xoffset);
        }
        else if (xoffset < 0)
        {
            emit("wheel_right", (int)xoffset);
        }

        if (yoffset > 0)
        {
            emit("wheel_up", (int)yoffset);
        }
        else if (yoffset < 0)
        {
            emit("wheel_down", (int)yoffset);
        }
    }

    void Input::_onMouseMoveEvent(double xpos, double ypos)
    {
        mousePositionOffset.x = mousePosition.x - xpos;
        mousePositionOffset.y = mousePosition.y - ypos;
        mousePosition.x = xpos;
        mousePosition.y = ypos;

        emit("mouse_moved", mousePosition, mousePositionOffset);
    }
}