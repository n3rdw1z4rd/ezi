# ezi
E-Z Input: a c++ library for GLFW that handles user input with down state and events

## Description:
This library captures user input on the following devices using methods from the GLFW library:
* Keyboard
* Mouse
    * Position
    * Buttons
    * Wheel (both x and y)

## Dependencies:
* [glfw v3.3.4+](https://github.com/glfw/glfw)
* [glm v0.9.9.8+](https://github.com/g-truc/glm)

## Installation:
* Easily drop the contents of the `/src` folder into your project source.
* Build and link the library

## Build:
Using CMAKE 3.14+:
* `$ mkdir build && cd build`
* `$ cmake ..` or `$ cmake -G "MinGW Makefiles" ..`

## Usage:
Please refer to the `test/test.cpp` file in the repo for a full example.

## Credits:
I must mention the following as the EventEmitter was highly adapted from their work:
* [rioki](https://gist.github.com/rioki/1290004d7505380f2b1d)
* [martinfinke](https://gist.github.com/martinfinke/a636dcddbcf112344b59)
