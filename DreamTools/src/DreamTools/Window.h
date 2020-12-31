#pragma once

#include "dtpch.h"
#include "DreamTools/Core.h"
#include "DreamTools/Events/Event.h"

namespace DreamTools
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Dream Tools Engine", const unsigned width = 1024, const unsigned height = 512) : Title(title), Width(width), Height(height)
		{
			
		}
	};

	//Interface representing a desktop system based window
	class DREAMTOOLS_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {};
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		 
		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}