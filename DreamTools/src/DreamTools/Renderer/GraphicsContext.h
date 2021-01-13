#pragma once

namespace DreamTools
{
	class GraphicsContext
	{
	public:
		//Pure virtual
		virtual ~GraphicsContext();
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}
