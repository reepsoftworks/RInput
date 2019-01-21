# RInput

## About

RInput is a direct input API in which combines registers Keyboard, Mouse and GamePad inputs as actions. Actions can be registered by the developer and binded by the end user via a XML document.

No matter if it's a digital or a analog input, all events are returned with a float value ranging from 0 to 1. This is to ensure that any key can be binded to any action. RInput uses the SDL2 GameController class for complete flexiblity. As the most common controller on PC is the XInput controller, the SDL2 GameController class will consider all GamePads as XInput devices.

Please have a look at "test/main.cpp" for examples how to register and use actions.

## Dependencies:
- SDL2 - https://www.libsdl.org/
- tinyxml2 (Included in src) - https://github.com/leethomason/tinyxml2
