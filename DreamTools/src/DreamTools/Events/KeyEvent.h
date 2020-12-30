#pragma once
#include "../Events/Event.h"
#include "../Core/KeyCodes.h"


namespace DreamTools
{
	class DREAMTOOLS_API KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(const KeyCode keycode) : m_KeyCode(keycode){}

		KeyCode m_KeyCode;
	};

	class DREAMTOOLS_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount){}
		uint16_t GetRepeateEvent() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)

	private:
		uint16_t m_RepeatCount;
	};

	class DREAMTOOLS_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode){}
		
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class DREAMTOOLS_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode) : KeyEvent(keycode){}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
