#pragma once
#include "Event.h"

#include <sstream>

namespace Engine
{
	template<EventType T>
	class ENGINE_API KeyEvent : public Event
	{
	public:
		inline int32_t GetKeyCode() const { return m_keyCode; }

		static EventType GetStaticType() { return T; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return EventUtil::ToString(T); }

		virtual int32_t GetCategoryFlags() const override
		{
			return (int32_t)EventCategory::EventCategoryKeyboard | (int32_t)EventCategory::EventCategoryInput;
		}
	protected:
		KeyEvent(int32_t keyCode)
			: m_keyCode(keyCode) {}
	private:
		int32_t m_keyCode;
	};

	class ENGINE_API KeyPressedEvent : public KeyEvent<EventType::KeyPressed>
	{
	public:
		KeyPressedEvent(int32_t keyCode, int32_t repeatCount)
			: KeyEvent(keyCode), m_repeatCount(repeatCount) {}

		int32_t GetRepeatCount() const { return m_repeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << GetKeyCode() << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}
	private:
		int32_t m_repeatCount;
	};

	class ENGINE_API KeyReleasedEvent : public KeyEvent<EventType::KeyReleased>
	{
	public:
		KeyReleasedEvent(int32_t keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << GetKeyCode();
			return ss.str();
		}
	};
	class ENGINE_API KeyTypedEvent : public KeyEvent<EventType::KeyTyped>
	{
	public:
		KeyTypedEvent(int32_t keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << GetKeyCode();
			return ss.str();
		}
	};
}
