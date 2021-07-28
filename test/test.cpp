#include "../src/input.hpp"

#include <iostream>

int main(void)
{
    if (glfwInit())
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        GLFWwindow *window = glfwCreateWindow(800, 600, "ezi - test", nullptr, nullptr);
        
        // this is needed in order to setup the callbacks in ezi:
        glfwSetWindowUserPointer(window, nullptr);

        // instatiate ezi:
        ezi::Input input{window};

        // add a Lambda listener to the "key_down" event:
        input.addListener("key_down", [](int key, int mods)
                          { std::cout << "key_down: " << key << std::endl; });

        // add a Lambda listener to the "key_up" event:
        input.addListener("key_up", [](int key, int mods)
                          { std::cout << "key_up: " << key << std::endl; });

        // the following "key_pressed" event uses the downTimeThreshold (250ms)
        // to determine the if the key 'pressed' or just released. If the key
        // has only been down for < downTimeThreshold, it assumed it was pressed,
        // not held down and then released.
        // add a Lambda listener to the "key_pressed" event:
        input.addListener("key_pressed", [&](int key, int mods)
                          {
                              if (key == GLFW_KEY_ESCAPE)
                              {
                                  std::cout << "shutting down..." << std::endl;
                                  glfwSetWindowShouldClose(window, 1);
                              }
                              else
                              {
                                  std::cout << "key_pressed: " << key << std::endl;
                              }
                          });

        // add a Lambda listener to the "button_down" event:
        input.addListener("button_down", [](int button, int mods)
                          { std::cout << "button_down: " << button << std::endl; });

        // add a Lambda listener to the "button_up" event:
        input.addListener("button_up", [](int button, int mods)
                          { std::cout << "button_up: " << button << std::endl; });

        // the following "button_clicked" event uses the downTimeThreshold (250ms)
        // to determine the if the button 'clicked' or just released. If the button
        // has only been down for < downTimeThreshold, it assumed it was clicked,
        // not held down and then released.
        // add a Lambda listener to the "button_clicked" event:
        input.addListener("button_clicked", [](int button, int mods)
                          { std::cout << "button_clicked: " << button << std::endl; });

        // add a Lambda listener to the "wheel_left" event:
        input.addListener("wheel_left", [](int offset)
                          { std::cout << "wheel_left: " << offset << std::endl; });

        // add a Lambda listener to the "wheel_right" event:
        input.addListener("wheel_right", [](int offset)
                          { std::cout << "wheel_right: " << offset << std::endl; });

        // add a Lambda listener to the "wheel_up" event:
        input.addListener("wheel_up", [](int offset)
                          { std::cout << "wheel_up: " << offset << std::endl; });

        // add a Lambda listener to the "wheel_down" event:
        input.addListener("wheel_down", [](int offset)
                          { std::cout << "wheel_down: " << offset << std::endl; });

        // add a Lambda listener to the "mouse_moved" event:
        input.addListener("mouse_moved", [&](glm::vec2 position, glm::vec2 offset)
                          {
                              // check to see it the left mouse button is down, then
                              // log the information
                              if (input.isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
                              {
                                  std::cout << "mouse_moved: px: " << position.x << ", py: " << position.y
                                            << ", ox: " << offset.x << ", oy: " << offset.y << std::endl;
                              }
                          });

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // check to see if the space bar is being held down during the main loop
            if (input.isKeyDown(GLFW_KEY_SPACE))
            {
                std::cout << "space is down!!" << std::endl;
            }
        }

        glfwDestroyWindow(window);
        glfwTerminate();
    }
    else
    {
        std::cout << "could not initialize GLFW!\n" << std::endl;
    }

    return 0;
}