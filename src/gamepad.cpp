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

#include <map>
#include <string>
#include "rinput.h"

namespace RInput_GamePad
{
	gamepad_t m_arrayControllers[GamePadIndex::ENUM_GAMEPAD_MAX];

	//-----------------------------------------------------------------------------
	// Purpose: Assign each device to a port.
	//-----------------------------------------------------------------------------
	void Connect(const Sint32& pWhich)
	{
		if (pWhich <= ENUM_GAMEPAD_MAX)
		{
			if (m_arrayControllers[pWhich].controller == nullptr)
			{
				m_arrayControllers[pWhich].controller = SDL_GameControllerOpen(pWhich);
				m_arrayControllers[pWhich].pszDeviceName = SDL_GameControllerName(m_arrayControllers[pWhich].controller);


				m_arrayControllers[pWhich].bEnabled = true;
				Flush(pWhich);

				printf("Connected GamePad '%s' into port %d\n", m_arrayControllers[pWhich].pszDeviceName, pWhich);
			}
			else
			{
				// Error
				printf("Failed to connect GamePad to port %d as port is already in use.\n", pWhich);
			}
		}
		else
		{
			// Error
			printf("Failed to connect GamePad to port %d as the index is out of range (Max Controllers: %d).\n", pWhich, ENUM_GAMEPAD_MAX);
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Disconnect the device from a port.
	//-----------------------------------------------------------------------------
	void Disconnect(const Sint32& pWhich)
	{
		if (m_arrayControllers[pWhich].controller != nullptr)
		{
			printf("Disconnecting GamePad %d.\n", pWhich);
			Flush(pWhich);

			SDL_GameControllerClose(m_arrayControllers[pWhich].controller);
			m_arrayControllers[pWhich].pszDeviceName = "";
			m_arrayControllers[pWhich].bEnabled = false;
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	gamepad_t GetDeviceFromPort(const GamePadIndex& pPort)
	{
		if (m_arrayControllers[(Sint32)pPort].controller != NULL)
		{
			return m_arrayControllers[(Sint32)pPort];
		}

		return m_arrayControllers[0];
	}

	//-----------------------------------------------------------------------------
	// Purpose: Record the state of the button.
	//-----------------------------------------------------------------------------
	void SimulateButton(const Sint32& pWhich, const Uint8& pButton, bool bDown)
	{
		if (m_arrayControllers[pWhich].controller != nullptr && m_arrayControllers[pWhich].bEnabled == true)
		{
			m_arrayControllers[pWhich].mControllerButtons[pButton] = bDown;
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Return if the button is currently down.
	//-----------------------------------------------------------------------------
	float ButtonDown(const Uint8& pButton, const GamePadIndex& iIndex)
	{
		float val = 0.0f;
		if (m_arrayControllers[(Sint32)iIndex].bEnabled == false) return val;

		if (pButton >= SDL_CONTROLLER_BUTTON_MAX)
		{
			switch (pButton)
			{
			case GAMEPAD_BUTTON_LTRIGGER:
				val = GetAxisFloat((Sint32)iIndex, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
				break;

			case GAMEPAD_BUTTON_RTRIGGER:
				val = GetAxisFloat((Sint32)iIndex, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
				break;

			case GAMEPAD_BUTTON_LSTICK_UP:
				val = -GetAxisFloat((Sint32)iIndex, SDL_CONTROLLER_AXIS_LEFTY);
				break;

			case GAMEPAD_BUTTON_LSTICK_DOWN:
				val = GetAxisFloat((Sint32)iIndex, SDL_CONTROLLER_AXIS_LEFTY, true);
				break;

			case GAMEPAD_BUTTON_LSTICK_LEFT:
				val = -GetAxisFloat((Sint32)iIndex, SDL_CONTROLLER_AXIS_LEFTX);
				break;

			case GAMEPAD_BUTTON_LSTICK_RIGHT:
				val = GetAxisFloat((Sint32)iIndex, SDL_CONTROLLER_AXIS_LEFTX, true);
				break;

			case GAMEPAD_BUTTON_RSTICK_UP:
				val = -GetAxisFloat((Sint32)iIndex, SDL_CONTROLLER_AXIS_RIGHTY);
				break;

			case GAMEPAD_BUTTON_RSTICK_DOWN:
				val = GetAxisFloat((Sint32)iIndex, SDL_CONTROLLER_AXIS_RIGHTY, true);
				break;

			case GAMEPAD_BUTTON_RSTICK_LEFT:
				val = -GetAxisFloat((Sint32)iIndex, SDL_CONTROLLER_AXIS_RIGHTX);
				break;

			case GAMEPAD_BUTTON_RSTICK_RIGHT:
				val = GetAxisFloat((Sint32)iIndex, SDL_CONTROLLER_AXIS_RIGHTX, true);
				break;

			default:
				break;
			}
		}
		else
		{
			val = (float)m_arrayControllers[(Sint32)iIndex].mControllerButtons[pButton];
		}

		return val;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Get the string name of the button.
	//-----------------------------------------------------------------------------
	const char* GetButtonName(const Uint8& pButton)
	{
		if (pButton == GAMEPAD_BUTTON_LTRIGGER) return GAMEPAD_BUTTON_LTRIGGER_NAME;
		if (pButton == GAMEPAD_BUTTON_RTRIGGER) return GAMEPAD_BUTTON_RTRIGGER_NAME;

		if (pButton == GAMEPAD_BUTTON_LSTICK_UP) return GAMEPAD_BUTTON_LSTICK_UP_NAME;
		if (pButton == GAMEPAD_BUTTON_LSTICK_DOWN) return GAMEPAD_BUTTON_LSTICK_DOWN_NAME;
		if (pButton == GAMEPAD_BUTTON_LSTICK_LEFT) return GAMEPAD_BUTTON_LSTICK_LEFT_NAME;
		if (pButton == GAMEPAD_BUTTON_LSTICK_RIGHT) return GAMEPAD_BUTTON_LSTICK_RIGHT_NAME;

		if (pButton == GAMEPAD_BUTTON_RSTICK_UP) return GAMEPAD_BUTTON_RSTICK_UP_NAME;
		if (pButton == GAMEPAD_BUTTON_RSTICK_DOWN) return GAMEPAD_BUTTON_RSTICK_DOWN_NAME;
		if (pButton == GAMEPAD_BUTTON_RSTICK_LEFT) return GAMEPAD_BUTTON_RSTICK_LEFT_NAME;
		if (pButton == GAMEPAD_BUTTON_RSTICK_RIGHT) return GAMEPAD_BUTTON_RSTICK_RIGHT_NAME;

		return SDL_GameControllerGetStringForButton((SDL_GameControllerButton)pButton);
	}

	//-----------------------------------------------------------------------------
	// Purpose: Get the index of the button.
	//-----------------------------------------------------------------------------
	Uint8 GetButtonIndex(const char* pButton)
	{
		std::string s(pButton);
		if (s == GAMEPAD_BUTTON_LTRIGGER_NAME) return GAMEPAD_BUTTON_LTRIGGER;
		if (s == GAMEPAD_BUTTON_RTRIGGER_NAME) return GAMEPAD_BUTTON_RTRIGGER;

		if (s == GAMEPAD_BUTTON_LSTICK_UP_NAME) return GAMEPAD_BUTTON_LSTICK_UP;
		if (s == GAMEPAD_BUTTON_LSTICK_DOWN_NAME) return GAMEPAD_BUTTON_LSTICK_DOWN;
		if (s == GAMEPAD_BUTTON_LSTICK_LEFT_NAME) return GAMEPAD_BUTTON_LSTICK_LEFT;
		if (s == GAMEPAD_BUTTON_LSTICK_RIGHT_NAME) return GAMEPAD_BUTTON_LSTICK_RIGHT;

		if (s == GAMEPAD_BUTTON_RSTICK_UP_NAME) return GAMEPAD_BUTTON_RSTICK_UP;
		if (s == GAMEPAD_BUTTON_RSTICK_DOWN_NAME) return GAMEPAD_BUTTON_RSTICK_DOWN;
		if (s == GAMEPAD_BUTTON_RSTICK_LEFT_NAME) return GAMEPAD_BUTTON_RSTICK_LEFT;
		if (s == GAMEPAD_BUTTON_RSTICK_RIGHT_NAME) return GAMEPAD_BUTTON_RSTICK_RIGHT;
		s.clear();

		return (Uint8)SDL_GameControllerGetButtonFromString(pButton);
	}

	/*
	const double GetAnalogThreshold(const Sint32& pWhich, const SDL_GameControllerAxis& pAxis)
	{
	Sint16 ltv = SDL_GameControllerGetAxis(m_arrayControllers[pWhich].controller, pAxis);
	return fmin(1.0, fmax(ltv, 0.0));
	}
	*/

	//-----------------------------------------------------------------------------
	// Purpose: Get the axis value aiding in turning the axis' into buttons.
	//-----------------------------------------------------------------------------
	Sint16 GetAxisValue(const Sint32& pWhich, const Uint32& pAxis, bool bFlip)
	{
		Sint16 val = SDL_GameControllerGetAxis(m_arrayControllers[pWhich].controller, (SDL_GameControllerAxis)pAxis);
		if (pAxis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
		{
			if (val < GAMEPAD_TRIGGER_THRESHOLD)
			{
				val = 0;
			}
		}

		if (pAxis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
		{
			if (val < GAMEPAD_TRIGGER_THRESHOLD)
			{
				val = 0;
			}
		}

		if (!bFlip)
		{
			if (pAxis == SDL_CONTROLLER_AXIS_LEFTY)
			{
				if (val > -GAMEPAD_THUMB_DEADZONE)
				{
					val = 0;
				}
			}

			if (pAxis == SDL_CONTROLLER_AXIS_LEFTX)
			{
				if (val > -GAMEPAD_THUMB_DEADZONE)
				{
					val = 0;
				}
			}
		}
		else
		{
			if (pAxis == SDL_CONTROLLER_AXIS_LEFTY)
			{
				if (val < GAMEPAD_THUMB_DEADZONE)
				{
					val = 0;
				}
			}

			if (pAxis == SDL_CONTROLLER_AXIS_LEFTX)
			{
				if (val < GAMEPAD_THUMB_DEADZONE)
				{
					val = 0;
				}
			}
		}

		return val;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Returns a min of -1.0 or max of 1.0.
	//-----------------------------------------------------------------------------
	const float GetAxisFloat(const Sint32& pWhich, const Uint32& pAxis, bool bFlip)
	{
		Sint16 ltv = GetAxisValue(pWhich, pAxis, bFlip);
		float perc = (float)ltv / (float)SDL_MAX_SINT16;
		if (perc < -0.9) perc = -1.0;
		if (perc > 0.9) perc = 1.0;
		//printf("%f\n", perc);
		return perc;
	}

	//-----------------------------------------------------------------------------
	// Purpose: Flush a controller's inputs.
	//-----------------------------------------------------------------------------
	void Flush(const Sint32& pWhich)
	{
		if (pWhich <= ENUM_GAMEPAD_MAX)
		{
			if (m_arrayControllers[pWhich].controller != nullptr)
			{
				m_arrayControllers[pWhich].mControllerButtons.clear();
			}
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: Flush all controller input.
	//-----------------------------------------------------------------------------
	void FlushAll()
	{
		SDL_FlushEvent(actioncontroller_event.type == SDL_CONTROLLERBUTTONDOWN);
		SDL_FlushEvent(actioncontroller_event.type == SDL_CONTROLLERBUTTONUP);
		SDL_FlushEvent(actioncontroller_event.type == SDL_JOYAXISMOTION);

		for (Sint32 i = 0; i < ENUM_GAMEPAD_MAX; i++)
		{
			Flush(i);
		}
	}
}