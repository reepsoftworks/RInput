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

#include <vector>
#include <map>
#include <string>
#include "rinput.h"

#include "tinyxml2.h"
using namespace tinyxml2;

SDL_Window* actioncontroller_window;
SDL_Event actioncontroller_event;

namespace RInput
{
	//-----------------------------------------------------------------------------
	// Purpose: Shared Init function.
	//-----------------------------------------------------------------------------
	void _Init()
	{
#ifndef _WIN32
		// On my rasppi, I'm not getting any controller feedback unless this is enabled.
		// With Win32, this isn't needed for XInput. Include/Remove based on your app.
		SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
#endif

		// Define this pre-processor if you have some odd controller or custom config.
#ifdef USE_RINPUT_GAMECONTROLLERCONFIG
		SDL_SetHint(SDL_HINT_GAMECONTROLLERCONFIG, "1");
		if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) >= 0)
		{
			int iNumOfControllers = SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
			if (iNumOfControllers == -1)
			{
				printf("Failed to load controller database! SDL Error: %s\n", SDL_GetError());
			}
		}
		else
		{
			printf("Error: Failed to initialize SDL_INIT_GAMECONTROLLER! SDL_Error: %s\n", SDL_GetError());
		}
#else
		SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
#endif
		SetActiveDevice(CONTROLLER_KEYBOARDMOUSE);
		RInput_KM::Enable();
	}

	//-----------------------------------------------------------------------------
	// Purpose: Init function for SDL projects.
	//-----------------------------------------------------------------------------
	void Init(SDL_Window* pWindow)
	{
		if (actioncontroller_window == NULL)
		{
			actioncontroller_window = pWindow;
		}
	
		_Init();
	}

	//-----------------------------------------------------------------------------
	// Purpose: Init function for non-SDL projects.
	//-----------------------------------------------------------------------------
	void InitSDL(const void *data)
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) >= 0)
		{
			actioncontroller_window = SDL_CreateWindowFrom(data);
			_Init();
		}
		else
		{
			printf("Error: Failed to initialize SDL2! SDL_Error: %s\n", SDL_GetError());
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Sets the device mode based on what was last 'waken'.
	//-----------------------------------------------------------------------------
	Controllers_t acitve_device;
	void SetActiveDevice(const Controllers_t& pDevice)
	{
		if (acitve_device != pDevice)
		{
			std::string s;
			if (pDevice > CONTROLLER_KEYBOARDMOUSE) s = "Gamepad";
			if (pDevice <= CONTROLLER_KEYBOARDMOUSE)
			{
				s = "Keyboard + Mouse";
				RInput_KM::Enable();
			}
				
			printf("%s is now the current device.\n", s.c_str());
			acitve_device = pDevice;
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Returns the active controller device.
	//-----------------------------------------------------------------------------
	Controllers_t GetActiveDevice()
	{
		return acitve_device;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Returns the active controller device as a string.
	//-----------------------------------------------------------------------------
	const char* GetActiveDeviceAsString()
	{
		if (acitve_device == CONTROLLER_GAMEPAD) return "GamePad";
		if (acitve_device == CONTROLLER_KEYBOARDMOUSE) return "KeyboardMouse";
		return "unknown";
	}

	//-----------------------------------------------------------------------------
	// Purpose: Returns the gamepad's device as a string. EX: "XInput Controller"
	//-----------------------------------------------------------------------------
	const char* GetGamePadDeviceAsString(const int pPort)
	{
		if (GetActiveDevice() == CONTROLLER_GAMEPAD)
		{
			return GetDeviceFromPort((RInput_GamePad::GamePadIndex)pPort).pszDeviceName;
		}
		return "unknown";
	}

	//-----------------------------------------------------------------------------
	// Purpose: Private Functions to capture mouse events as button events. 
	//-----------------------------------------------------------------------------
	void FixMouseBtnDown(const Uint8& pButton)
	{
		long b = pButton;
		if (b == SDL_BUTTON_LEFT) b = MOUSE_BUTTON_LEFT;
		if (b == SDL_BUTTON_MIDDLE) b = MOUSE_BUTTON_MIDDLE;
		if (b == SDL_BUTTON_RIGHT) b = MOUSE_BUTTON_RIGHT;
		RInput_KM::SimulateButton(b, true);
	}

	void FixMouseBtnUp(const Uint8& pButton)
	{
		long b = pButton;
		if (b == SDL_BUTTON_LEFT) b = MOUSE_BUTTON_LEFT;
		if (b == SDL_BUTTON_MIDDLE) b = MOUSE_BUTTON_MIDDLE;
		if (b == SDL_BUTTON_RIGHT) b = MOUSE_BUTTON_RIGHT;
		RInput_KM::SimulateButton(b, false);
	}

	//-----------------------------------------------------------------------------
	// Purpose: Returns the amount of gamepads connected.
	//-----------------------------------------------------------------------------
	Sint8 gamepad_count = 0;
	Sint8 GetGamePadCount()
	{
		return gamepad_count;
	}

	//-----------------------------------------------------------------------------
	// Purpose: SDL Polling.
	//-----------------------------------------------------------------------------
	void TestEvents(const SDL_Event& pEvent)
	{
		if (actioncontroller_event.type != pEvent.type) actioncontroller_event = pEvent;

		switch (pEvent.type)
		{
		// Game Pad:
		case SDL_CONTROLLERDEVICEADDED:
			RInput_GamePad::Connect(pEvent.cdevice.which);
			gamepad_count++;
			SetActiveDevice(CONTROLLER_GAMEPAD);
			break;

		case SDL_CONTROLLERDEVICEREMOVED:
			RInput_GamePad::Disconnect(pEvent.cdevice.which);
			gamepad_count--;
			SetActiveDevice(CONTROLLER_KEYBOARDMOUSE);
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			RInput_GamePad::SimulateButton(pEvent.cdevice.which, pEvent.cbutton.button, true);
			SetActiveDevice(CONTROLLER_GAMEPAD);
			break;

		case SDL_CONTROLLERBUTTONUP:
			RInput_GamePad::SimulateButton(pEvent.cdevice.which, pEvent.cbutton.button, false);
			SetActiveDevice(CONTROLLER_GAMEPAD);
			break;

		case SDL_CONTROLLERAXISMOTION:
			//RInput_GamePad::UpdateAxisMotions(pEvent.cdevice.which, pEvent.caxis.axis);
			SetActiveDevice(CONTROLLER_GAMEPAD);
			break;

		// Keyboard + Mouse:
		case SDL_KEYDOWN:
			RInput_KM::SimulateButton(pEvent.key.keysym.sym, true);
			SetActiveDevice(CONTROLLER_KEYBOARDMOUSE);
			break;

		case SDL_KEYUP:
			RInput_KM::SimulateButton(pEvent.key.keysym.sym, false);
			SetActiveDevice(CONTROLLER_KEYBOARDMOUSE);
			break;

		case SDL_MOUSEBUTTONDOWN:
			FixMouseBtnDown(pEvent.button.button);
			SetActiveDevice(CONTROLLER_KEYBOARDMOUSE);
			break;

		case SDL_MOUSEBUTTONUP:
			FixMouseBtnUp(pEvent.button.button);
			SetActiveDevice(CONTROLLER_KEYBOARDMOUSE);
			break;

		case SDL_MOUSEWHEEL:
			RInput_KM::SetMouseWheelPosition(pEvent.wheel.y);
			break;

		case SDL_MOUSEMOTION:
			RInput_KM::UpdateMousePosition();
			break;

		default:
			break;
		}

		if (GetActiveDevice() == CONTROLLER_GAMEPAD)
		{
			SDL_GameControllerUpdate();
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Poll events for non-SDL projects.
	//-----------------------------------------------------------------------------
	void PollEvents()
	{
		while (SDL_PollEvent(&actioncontroller_event) != 0)
		{
			TestEvents(actioncontroller_event);
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Flush inputs from the controller.
	//-----------------------------------------------------------------------------
	void Flush(const Controllers_t& pController)
	{
		switch (pController)
		{
		case CONTROLLER_KEYBOARDMOUSE:
			RInput_KM::Flush();
			break;

		case CONTROLLER_GAMEPAD:
			RInput_GamePad::FlushAll();
			break;

		default:
			break;
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Flushes everything.
	//-----------------------------------------------------------------------------
	void FlushAll()
	{
		for (int i = 0; i <= CONTROLLER_GAMEPAD; i++)
		{
			Flush((Controllers_t)i);
		}
	}

	//====================================================================
	std::map<std::string, action_t> mActions;

	//-----------------------------------------------------------------------------
	// Purpose: Test the action state.
	//-----------------------------------------------------------------------------
	float GetActionInput(action_t& pButton)
	{
		float t = 0.0f;

		if (GetActiveDevice() == CONTROLLER_KEYBOARDMOUSE)
		{
			t = RInput_KM::ButtonDown(pButton.key);
		}
		else if (GetActiveDevice() == CONTROLLER_GAMEPAD)
		{
			t = RInput_GamePad::ButtonDown(pButton.button);
		}

		// Button is not returning 0.
		if (t > 0.0f)
		{
			if (pButton.bHit)
			{
				if (!pButton.bDown)
				{
					pButton.bDown = true;
				}
				else
				{
					if (pButton.bDown)
					{
						return 0.0f;
					}
				}
			}
		}
		else
		{
			if (pButton.bDown)
			{
				pButton.bDown = false;
			}
		}

		return t;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Register the action.
	//-----------------------------------------------------------------------------
	void RegisterAction(const std::string& pActionName, Sint32 iKey, Uint8 iButton, bool bConistant)
	{
		mActions[pActionName].key = iKey;
		mActions[pActionName].button = iButton;
		mActions[pActionName].bHit = !bConistant;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Modify the action.
	//-----------------------------------------------------------------------------
	void ModifyAction(const std::string& pActionName, Sint32 iKey, Uint8 iButton)
	{
		printf("Setting action '%s' Key: '%s' Button '%s'\n", pActionName.c_str(), RInput_KM::GetButtonName(iKey), RInput_GamePad::GetButtonName(iButton));
		mActions[pActionName].key = iKey;
		mActions[pActionName].button = iButton;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Returns the Action struct.
	//-----------------------------------------------------------------------------
	action_t& GetAction(const std::string& pActionName)
	{
		return mActions[pActionName];
	}

	//-----------------------------------------------------------------------------
	// Purpose: Uses tinyxml2 to phrase the config for action defs.
	//-----------------------------------------------------------------------------
	XMLError _ReadFile(const char* pszPath)
	{
		XMLDocument xmlDoc;
		XMLError eResult = xmlDoc.LoadFile(pszPath);

		if (eResult != XML_SUCCESS) return eResult;

		XMLNode* pRoot = xmlDoc.FirstChild();
		if (pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;


		XMLElement *pActionSetElement = pRoot->FirstChildElement("ActionSet");
		if (pActionSetElement == nullptr) return XML_ERROR_PARSING_ELEMENT;

		while (pActionSetElement != nullptr)
		{
			//const char* pszValSet = pActionSetElement->Attribute("name");
			//if (pszValSet == nullptr) return XML_ERROR_PARSING_ATTRIBUTE;
			//printf("Action Set: %s\n", pszValSet);

			XMLElement *pActionElement = pActionSetElement->FirstChildElement("Action");
			if (pActionElement == nullptr) return XML_ERROR_PARSING_ELEMENT;

			while (pActionElement != nullptr)
			{
				const char* pszValName = pActionElement->Attribute("name");
				if (pszValName == nullptr) return XML_ERROR_PARSING_ATTRIBUTE;
				const char* pszValKey = pActionElement->Attribute("key");
				if (pszValKey == nullptr) return XML_ERROR_PARSING_ATTRIBUTE;
				const char* pszValButton = pActionElement->Attribute("button");
				if (pszValButton == nullptr) return XML_ERROR_PARSING_ATTRIBUTE;
				ModifyAction(pszValName, RInput_KM::GetButtonIndex(pszValKey), RInput_GamePad::GetButtonIndex(pszValButton));

				pActionElement = pActionElement->NextSiblingElement("Action");
			}

			pActionSetElement = pRoot->NextSiblingElement("ActionSet");
		}

		return XML_SUCCESS;
	}

	// TODO: Create a way to write XML documents with registered actions.
	/*
	XMLError _WriteFile(const char* pszPath)
	{
		return XML_NO_ATTRIBUTE;
	}
	*/

	//-----------------------------------------------------------------------------
	// Purpose: Bool function to call the xml phrasing.
	//-----------------------------------------------------------------------------
	bool LoadActionsFromFile(const char* pszPath)
	{
		XMLError e = _ReadFile(pszPath);
		if (e != XML_SUCCESS)
		{
			printf("Error: Failed to modify actions from XML file! XML Error: %i.\n", e);
			return false;
		}

		return true;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Call each tick to make the gamepad stick control the mouse pos.
	//-----------------------------------------------------------------------------
	Uint32 lasttickmousemove = 0;
	void UpdateGamePadStickAsMouse(const Sint32& pWhich, const Sint8& pAxis)
	{
		float rsX, rsY;

		if (pAxis == GAMEPAD_AXIS_LSTICK)
		{
			rsX = RInput_GamePad::GetAxisFloat(pWhich, SDL_CONTROLLER_AXIS_LEFTX);
			rsY = RInput_GamePad::GetAxisFloat(pWhich, SDL_CONTROLLER_AXIS_LEFTY);
		}
		else
		{
			rsX = RInput_GamePad::GetAxisFloat(pWhich, SDL_CONTROLLER_AXIS_RIGHTX);
			rsY = RInput_GamePad::GetAxisFloat(pWhich, SDL_CONTROLLER_AXIS_RIGHTY);
		}

		if ((int)rsX != 0 || (int)rsY != 0)
		{
			if (SDL_GetTicks() > lasttickmousemove)
			{
				RInput_KM::ModMousePosition((int)rsX, (int)rsY, false);
				lasttickmousemove = SDL_GetTicks();
			}
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Send a rumble command to the controller.
	//-----------------------------------------------------------------------------
	#ifndef RINPUT_NO_RUMBLE
	void RumbleGamePad(const int pPort, Uint16 iLeftMotor, Uint16 iRightMotor, Uint32 iDuration)
	{
		if (GetActiveDevice() == CONTROLLER_GAMEPAD)
		{
			if (pPort == CONTROLLER_PORT_ALL)
			{
				for (int i = 0; i < GetGamePadCount(); i++)
				{
					SDL_GameControllerRumble(RInput_GamePad::GetDeviceFromPort((RInput_GamePad::GamePadIndex)i).controller, iLeftMotor, iRightMotor, iDuration);
				}
			}
			else
			{
				SDL_GameControllerRumble(RInput_GamePad::GetDeviceFromPort((RInput_GamePad::GamePadIndex)pPort).controller, iLeftMotor, iRightMotor, iDuration);
			}
		}
	}
	#endif
}
