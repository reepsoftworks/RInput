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

namespace RInput_KM
{
	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	std::map<Sint32, bool> mButton;
	bool bEnabled;

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	void SimulateButton(const Sint32& pKey, bool bDown)
	{
		if (bEnabled == false)
			return;

		mButton[pKey] = bDown;
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	const float ButtonDown(const Sint32& pKey)
	{
		if (bEnabled == false) 0.0f;

		if (pKey == MOUSE_BUTTON_WHEELUP) return (float)OnMouseWheelUp();
		if (pKey == MOUSE_BUTTON_WHEELDOWN) return (float)OnMouseWheelDown();
		return (float)mButton[pKey];
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	int mouseX = 0;
	int mouseY = 0;
	void SetMousePosition(const int x, const int y, bool bGlobal)
	{
		mouseX = x;
		mouseY = y;

		if (!bGlobal)
		{
			SDL_WarpMouseInWindow(actioncontroller_window, mouseX, mouseY);
		}
		else
		{
			SDL_WarpMouseGlobal(mouseX, mouseY);
		}
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	void ModMousePosition(const int x, const int y, bool bGlobal)
	{
		int newX = GetMouseX() + x;
		int newY = GetMouseY() + y;
		SetMousePosition(newX, newY, bGlobal);
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	void UpdateMousePosition()
	{
		SDL_GetMouseState(&mouseX, &mouseY);
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	void SimulateMouse(const int x, const int y)
	{
		mouseX = x;
		mouseY = y;
	}

	const int GetMouseX() { return mouseX; }
	const int GetMouseY() { return mouseY; }

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	void ShowMouse()
	{
		SDL_ShowCursor(SDL_ENABLE);
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	void HideMouse()
	{
		SDL_ShowCursor(SDL_DISABLE);
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	int iMouseWheelPos = 0;
	void SetMouseWheelPosition(const int y)
	{
		iMouseWheelPos = y;
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	const bool OnMouseWheelUp()
	{
		bool b = false;
		if (iMouseWheelPos >= 1)
		{
			b = true;
			iMouseWheelPos = 0;
		}

		return b;
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	const bool OnMouseWheelDown()
	{
		bool b = false;
		if (iMouseWheelPos < 0)
		{
			b = true;
			iMouseWheelPos = 0;
		}

		return b;
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	const char* GetButtonName(const Sint32& pButton)
	{
		if (pButton == MOUSE_BUTTON_LEFT) return MOUSE_BUTTON_LEFT_NAME;
		if (pButton == MOUSE_BUTTON_MIDDLE) return MOUSE_BUTTON_MIDDLE_NAME;
		if (pButton == MOUSE_BUTTON_RIGHT) return MOUSE_BUTTON_RIGHT_NAME;

		if (pButton == MOUSE_BUTTON_WHEELUP) return MOUSE_BUTTON_WHEELUP_NAME;
		if (pButton == MOUSE_BUTTON_WHEELDOWN) return MOUSE_BUTTON_WHEELDOWN_NAME;

		return SDL_GetKeyName(pButton);
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	const Sint32 GetButtonIndex(const char* pButton)
	{
		std::string s(pButton);
		if (s == MOUSE_BUTTON_LEFT_NAME) return MOUSE_BUTTON_LEFT;
		if (s == MOUSE_BUTTON_MIDDLE_NAME) return MOUSE_BUTTON_MIDDLE;
		if (s == MOUSE_BUTTON_RIGHT_NAME) return MOUSE_BUTTON_RIGHT;

		if (s == MOUSE_BUTTON_WHEELUP_NAME) return MOUSE_BUTTON_WHEELUP;
		if (s == MOUSE_BUTTON_WHEELDOWN_NAME) return MOUSE_BUTTON_WHEELDOWN;
		s.clear();

		return SDL_GetKeyFromName(pButton);
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	void Enable()	{ bEnabled = true;  }
	void Disable()	{ bEnabled = false; }

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	void FlushKeyboard()
	{
		SDL_FlushEvent(actioncontroller_event.type == SDL_MOUSEBUTTONDOWN);
		SDL_FlushEvent(actioncontroller_event.type == SDL_MOUSEBUTTONUP);
		mButton.clear();
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	void FlushMouse()
	{
		SDL_FlushEvent(actioncontroller_event.type == SDL_KEYDOWN);
		SDL_FlushEvent(actioncontroller_event.type == SDL_KEYUP);
		mButton.clear();
	}


	//-----------------------------------------------------------------------------
	// Purpose: 
	//-----------------------------------------------------------------------------
	void Flush()
	{
		FlushMouse();
		FlushKeyboard();
	}
}