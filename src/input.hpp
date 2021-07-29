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

#pragma once

#include "event_emitter.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>
#include <map>

namespace ezi
{
    class Input : public EventEmitter
    {
    public:
        Input();
        ~Input();

        void init(GLFWwindow *window);

        bool isKeyDown(int key) { return keyDown[key]; }
        bool isMouseButtonDown(int button) { return buttonDown[button]; }
        glm::vec2 getMousePosition() { return mousePosition; }
        glm::vec2 getMousePositionOffset() { return mousePositionOffset; }

    private:
        static void _keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void _mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
        static void _mouseWheelCallback(GLFWwindow *window, double xoffset, double yoffset);
        static void _mousePositionCallback(GLFWwindow *window, double xpos, double ypos);

        void _onKeyboardEvent(int key, int scancode, int action, int mods);
        void _onMouseButtonEvent(int button, int action, int mods);
        void _onMouseWheelEvent(double xoffset, double yoffset);
        void _onMouseMoveEvent(double xpos, double ypos);

        int downTimeThreshold{250};

        bool keyDown[GLFW_KEY_LAST + 1] = {false};
        long long keyDownTime[GLFW_KEY_LAST + 1] = {0};

        bool buttonDown[GLFW_MOUSE_BUTTON_LAST + 1] = {false};
        long long buttonDownTime[GLFW_MOUSE_BUTTON_LAST + 1] = {0};

        glm::vec2 mousePosition{0.f};
        glm::vec2 mousePositionOffset{0.f};
    };
}