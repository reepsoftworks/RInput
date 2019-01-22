/*
MIT License

Copyright (c) 2019 Reep Softworks

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef RINPUT_H
#define RINPUT_H
#if defined( _WIN32 )
#pragma once
#endif

#if defined (_WIN32)
#include <SDL.h>
#else
	// TODO: Test SDL decs on Linux/MacOS.
	#if defined(__APPLE__) && defined(__MACH__)
		#include <SDL2/SDL.h>
	#else
		#include "SDL2/SDL.h"
	#endif
#endif

#ifdef main
#undef main
#endif 

#include <map>
#include <string>

extern SDL_Window* actioncontroller_window;
extern SDL_Event actioncontroller_event;

//-----------------------------------------------------------------------------
// Namespace for Keyboard + Mouse.
//-----------------------------------------------------------------------------
namespace RInput_KM
{
	void SimulateButton(const Sint32& pKey, bool bDown);
	const float ButtonDown(const Sint32& pKey);
	
	void ShowMouse();
	void HideMouse();
	void SetMousePosition(const int x, const int y, bool bGlobal = false);
	void ModMousePosition(const int x, const int y, bool bGlobal = false);
	void UpdateMousePosition();
	void SimulateMouse(const int x, const int y);
	const int GetMouseX();
	const int GetMouseY();
	void SetMouseWheelPosition(const int y);
	const bool OnMouseWheelUp();
	const bool OnMouseWheelDown();

	// Returns the long to a character.
	const char* GetButtonName(const Sint32& pButton);

	// Returns the character to a long
	const Sint32 GetButtonIndex(const char* pButton);

	void Enable();
	void Disable();
	void FlushKeyboard();
	void FlushMouse();
	void Flush();	
}

//-----------------------------------------------------------------------------
// Namespace for GamePads.
//-----------------------------------------------------------------------------
namespace RInput_GamePad
{
	typedef enum
	{
		ENUM_GAMEPAD_NULL = -1,
		ENUM_GAMEPAD_ONE,
		ENUM_GAMEPAD_TWO,
		ENUM_GAMEPAD_THREE,
		ENUM_GAMEPAD_FOUR,
		ENUM_GAMEPAD_MAX = ENUM_GAMEPAD_FOUR
	} GamePadIndex;

	typedef struct
	{
		SDL_GameController* controller;
		const char* pszDeviceName;
		bool bEnabled;
		std::map<Uint8, bool> mControllerButtons;

	} gamepad_t;

	void Connect(const Sint32& pWhich);
	void Disconnect(const Sint32& pWhich);

	gamepad_t GetDeviceFromPort(const GamePadIndex& pPort);

	// Digital input:
	void SimulateButton(const Sint32& pWhich, const Uint8& pButton, bool bDown);

	const float ButtonDown(const Uint8& pButton, const GamePadIndex& iIndex = ENUM_GAMEPAD_ONE);

	const char* GetButtonName(const Uint8& pButton);
	const Uint8 GetButtonIndex(const char* pButton);

	// Analog input:
	void UpdateAxisMotions(const Sint32& pWhich, const Uint32& pAxis);
	const Sint16 GetAxisValue(const Sint32& pWhich, const Uint32& pAxis, bool bFlip = false);
	const float GetAxisFloat(const Sint32& pWhich, const Uint32& pAxis, bool bFlip = false);

	void Flush(const Sint32& pWhich);
	void FlushAll();
}

#define RUMBLE_MAX 65535
#define RUMBLE_HALFMAX RUMBLE_MAX / 2

//-----------------------------------------------------------------------------
// The access point for the API. 
//-----------------------------------------------------------------------------
namespace RInput
{
	typedef enum
	{
		CONTROLLER_KEYBOARDMOUSE,
		CONTROLLER_GAMEPAD
	} Controllers_t;

	void Init(SDL_Window* pWindow);
	void InitSDL(const void *data);

	void SetActiveDevice(const Controllers_t& pDevice);
	Controllers_t GetActiveDevice();
	const char* GetActiveDeviceAsString();
	const char* GetGamePadDeviceAsString(const int pPort);
	const Sint8 GetGamePadCount();

	void TestEvents(const SDL_Event& pEvent);
	void PollEvents(); // <- Use this function if you're not using SDL event polling.

	void Flush(const Controllers_t& pController);
	void FlushAll();

	//====================================================================
	typedef struct
	{
		Sint32 key;
		Uint8 button;
		bool bDown;
		bool bHit;

	} action_t;

	const float GetActionInput(action_t& pButton);
	void RegisterAction(const std::string& pActionName, Sint32 iKey, Uint8 iButton, bool bConistant);
	void ModifyAction(const std::string& pActionName, Sint32 iKey, Uint8 iButton);
	action_t& GetAction(const std::string& pActionName);
	bool LoadActionsFromFile(const char* pszPath);

	void UpdateGamePadStickAsMouse(const Sint32& pWhich, const Sint8& pAxis);
	void RumbleGamePad(const int pPort = 0, Uint16 iLeftMotor = RUMBLE_MAX, Uint16 iRightMotor = RUMBLE_MAX, Uint32 iDuration = 1000);
}

// Keyboard:
#define KEYBOARD_A SDLK_a
#define KEYBOARD_B SDLK_b
#define KEYBOARD_C SDLK_c
#define KEYBOARD_D SDLK_d
#define KEYBOARD_E SDLK_e
#define KEYBOARD_F SDLK_f
#define KEYBOARD_G SDLK_g
#define KEYBOARD_H SDLK_h
#define KEYBOARD_I SDLK_i
#define KEYBOARD_J SDLK_j
#define KEYBOARD_K SDLK_k
#define KEYBOARD_L SDLK_l
#define KEYBOARD_M SDLK_m
#define KEYBOARD_N SDLK_n
#define KEYBOARD_O SDLK_o
#define KEYBOARD_P SDLK_p
#define KEYBOARD_Q SDLK_q
#define KEYBOARD_R SDLK_r
#define KEYBOARD_S SDLK_s
#define KEYBOARD_T SDLK_t
#define KEYBOARD_U SDLK_u
#define KEYBOARD_V SDLK_v
#define KEYBOARD_W SDLK_w
#define KEYBOARD_X SDLK_x
#define KEYBOARD_Y SDLK_y
#define KEYBOARD_Z SDLK_z

#define KEYBOARD_D0 SDLK_0
#define KEYBOARD_D1 SDLK_1
#define KEYBOARD_D2 SDLK_2
#define KEYBOARD_D3 SDLK_3
#define KEYBOARD_D4 SDLK_4
#define KEYBOARD_D5 SDLK_5
#define KEYBOARD_D6 SDLK_6
#define KEYBOARD_D7 SDLK_7
#define KEYBOARD_D8 SDLK_9
#define KEYBOARD_D9 SDLK_9

#define KEYBOARD_F1 SDLK_F1
#define KEYBOARD_F2 SDLK_F2
#define KEYBOARD_F3 SDLK_F3
#define KEYBOARD_F4 SDLK_F4
#define KEYBOARD_F5 SDLK_F5
#define KEYBOARD_F6 SDLK_F6
#define KEYBOARD_F7 SDLK_F7
#define KEYBOARD_F8 SDLK_F8
#define KEYBOARD_F9 SDLK_F9
#define KEYBOARD_F10 SDLK_F10
#define KEYBOARD_F11 SDLK_F11
#define KEYBOARD_F12 SDLK_F12

#define KEYBOARD_NUMPAD0 SDLK_KP_0
#define KEYBOARD_NUMPAD1 SDLK_KP_1
#define KEYBOARD_NUMPAD2 SDLK_KP_2
#define KEYBOARD_NUMPAD3 SDLK_KP_3
#define KEYBOARD_NUMPAD4 SDLK_KP_4
#define KEYBOARD_NUMPAD5 SDLK_KP_5
#define KEYBOARD_NUMPAD6 SDLK_KP_6
#define KEYBOARD_NUMPAD7 SDLK_KP_7
#define KEYBOARD_NUMPAD8 SDLK_KP_8
#define KEYBOARD_NUMPAD9 SDLK_KP_9

#define KEYBOARD_NUMPAD_PERIOD SDLK_KP_PERIOD
#define KEYBOARD_NUMPAD_DIVIDE SDLK_KP_DIVIDE
#define KEYBOARD_NUMPAD_MULTIPLY SDLK_KP_MULTIPLY
#define KEYBOARD_NUMPAD_SUBTRACT SDLK_KP_MINUS
#define KEYBOARD_NUMPAD_ADDITION SDLK_KP_PLUS
#define KEYBOARD_NUMPAD_ENTER SDLK_KP_ENTER
#define KEYBOARD_BACKSPACE SDLK_BACKSPACE
#define KEYBOARD_CAPSLOCK SDLK_CAPSLOCK

#define KEYBOARD_DELETE SDLK_DELETE

#define KEYBOARD_UP SDLK_UP
#define KEYBOARD_DOWN SDLK_DOWN
#define KEYBOARD_LEFT SDLK_LEFT
#define KEYBOARD_RIGHT SDLK_RIGHT

#define KEYBOARD_END SDLK_END
#define KEYBOARD_ENTER SDLK_RETURN
#define KEYBOARD_ESC SDLK_ESCAPE
#define KEYBOARD_ESCAPE SDLK_ESCAPE
#define KEYBOARD_HOME SDLK_HOME
#define KEYBOARD_INSERT SDLK_INSERT

#define KEYBOARD_NUMLOCK SDLK_NUMLOCKCLEAR
#define KEYBOARD_PAGEDOWN SDLK_PAGEDOWN
#define KEYBOARD_PAGEUP SDLK_PAGEUP
#define KEYBOARD_LEFTCONTROL SDLK_LCTRL
#define KEYBOARD_RIGHTCONTROL SDLK_RCTRL
#define KEYBOARD_SHIFT SDLK_LSHIFT

#define KEYBOARD_SPACE SDLK_SPACE

#define KEYBOARD_SUBTRACT SDLK_MINUS
#define KEYBOARD_TAB SDLK_TAB
#define KEYBOARD_TILDE SDL_SCANCODE_GRAVE
#define KEYBOARD_GRAVE KEYBOARD_TILDE
#define KEYBOARD_EQUALS SDLK_EQUALS

#define KEYBOARD_OPENBRACKET SDLK_LEFTBRACKET
#define KEYBOARD_CLOSEBRACKET SDLK_RIGHTBRACKET
#define KEYBOARD_BLACKSLASH SDLK_BACKSLASH
#define KEYBOARD_SEMICOLON SDLK_SEMICOLON
#define KEYBOARD_QUOTES SDLK_QUOTEDBL
#define KEYBOARD_COMMA SDLK_COMMA
#define KEYBOARD_SLASH SDLK_SLASH
#define KEYBOARD_PERIOD SDLK_PERIOD

// Mouse:
#define MOUSE_BUTTON_LEFT SDLK_AUDIOFASTFORWARD + 1
#define MOUSE_BUTTON_MIDDLE MOUSE_BUTTON_LEFT + 1
#define MOUSE_BUTTON_RIGHT MOUSE_BUTTON_MIDDLE + 1
#define MOUSE_BUTTON_LEFT_NAME "leftmouse"
#define MOUSE_BUTTON_MIDDLE_NAME "middlemouse"
#define MOUSE_BUTTON_RIGHT_NAME "rightmouse"

// Fake button:
#define MOUSE_BUTTON_WHEELUP MOUSE_BUTTON_RIGHT + 1
#define MOUSE_BUTTON_WHEELUP_NAME "mousewheelup"
#define MOUSE_BUTTON_WHEELDOWN MOUSE_BUTTON_WHEELUP + 1
#define MOUSE_BUTTON_WHEELDOWN_NAME "mousewheeldown"

// GamePad:
#define GAMEPAD_BUTTON_A SDL_CONTROLLER_BUTTON_A
#define GAMEPAD_BUTTON_B SDL_CONTROLLER_BUTTON_B
#define GAMEPAD_BUTTON_X SDL_CONTROLLER_BUTTON_X
#define GAMEPAD_BUTTON_Y SDL_CONTROLLER_BUTTON_Y
#define GAMEPAD_BUTTON_BACK SDL_CONTROLLER_BUTTON_BACK
#define GAMEPAD_BUTTON_SELECT GAMEPAD_BUTTON_BACK
#define GAMEPAD_BUTTON_START SDL_CONTROLLER_BUTTON_START
#define GAMEPAD_BUTTON_LSTICK SDL_CONTROLLER_BUTTON_LEFTSTICK
#define GAMEPAD_BUTTON_RSTICK SDL_CONTROLLER_BUTTON_RIGHTSTICK
#define GAMEPAD_BUTTON_LSHOULDER SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define GAMEPAD_BUTTON_RSHOULDER SDL_CONTROLLER_BUTTON_RIGHTSHOULDER

#define GAMEPAD_BUTTON_DUP SDL_CONTROLLER_BUTTON_DPAD_UP
#define GAMEPAD_BUTTON_DDOWN SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define GAMEPAD_BUTTON_DLEFT SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define GAMEPAD_BUTTON_DRIGHT SDL_CONTROLLER_BUTTON_DPAD_RIGHT

// These are not physical buttons but 'fake' buttons for when
// the analog is in it's activated threshold. You can use the
// same button functions calling these defines.
#define GAMEPAD_BUTTON_LTRIGGER 16 
#define GAMEPAD_BUTTON_RTRIGGER 17 
#define GAMEPAD_BUTTON_LTRIGGER_NAME "lefttrigger"
#define GAMEPAD_BUTTON_RTRIGGER_NAME "righttrigger"

#define GAMEPAD_BUTTON_LSTICK_UP 18
#define GAMEPAD_BUTTON_LSTICK_UP_NAME "leftstickup"
#define GAMEPAD_BUTTON_LSTICK_DOWN 19
#define GAMEPAD_BUTTON_LSTICK_DOWN_NAME "leftstickdown"
#define GAMEPAD_BUTTON_LSTICK_LEFT 20
#define GAMEPAD_BUTTON_LSTICK_LEFT_NAME "leftstickleft"
#define GAMEPAD_BUTTON_LSTICK_RIGHT 21
#define GAMEPAD_BUTTON_LSTICK_RIGHT_NAME "leftstickright"

#define GAMEPAD_BUTTON_RSTICK_UP 22
#define GAMEPAD_BUTTON_RSTICK_UP_NAME "rightstickup"
#define GAMEPAD_BUTTON_RSTICK_DOWN 23
#define GAMEPAD_BUTTON_RSTICK_DOWN_NAME "rightstickdown"
#define GAMEPAD_BUTTON_RSTICK_LEFT 24
#define GAMEPAD_BUTTON_RSTICK_LEFT_NAME "rightstickleft"
#define GAMEPAD_BUTTON_RSTICK_RIGHT 25
#define GAMEPAD_BUTTON_RSTICK_RIGHT_NAME "rightstickright"

#define CONTROLLER_PORT_ALL -1
#define CONTROLLER_PORT_ONE RInput_GamePad::ENUM_GAMEPAD_ONE
#define CONTROLLER_PORT_TWO RInput_GamePad::ENUM_GAMEPAD_TWO
#define CONTROLLER_PORT_THREE RInput_GamePad::ENUM_GAMEPAD_THREE
#define CONTROLLER_PORT_FOUR RInput_GamePad::ENUM_GAMEPAD_FOUR

#define GAMEPAD_AXIS_LSTICK 1
#define GAMEPAD_AXIS_RSTICK 2

#define GAMEPAD_AXIS_LTRIGGER 3
#define GAMEPAD_AXIS_RTRIGGER 4

#define GAMEPAD_THUMB_DEADZONE 8000
#define GAMEPAD_TRIGGER_THRESHOLD 30

#endif // RINPUT_H