#pragma once
#include "DreamTools/Core/Core.h"
#include "DreamTools/Core/MouseCodes.h"
#include "DreamTools/Core/KeyCodes.h"

namespace DreamTools
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
