#pragma once
#include "Event.h"

#include <sstream>

namespace Engine
{
	template<EventType T>
	class ENGINE_API MouseEvent : public Event
	{
	public:
		static EventType GetStaticType() { return T; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return EventUtil::ToString(T); }

		virtual int32_t GetCategoryFlags() const override
		{
			return (int32_t)EventCategory::EventCategoryMouse | (int32_t)EventCategory::EventCategoryInput;
		}
	};

	class ENGINE_API MouseMovedEvent : public MouseEvent<EventType::MouseMoved>
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_mouseX(x), m_mouseY(y) {}

		float GetX() const { return m_mouseX; }
		float GetY() const { return m_mouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
			return ss.str();
		}
	private:
		float m_mouseX, m_mouseY;
	};

	class ENGINE_API MouseScrolledEvent : public MouseEvent<EventType::MouseScrolled>
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_xOffset(xOffset), m_yOffset(yOffset) {}

		float GetXOffset() const { return m_xOffset; }
		float GetYOffset() const { return m_yOffset; }
	private:
		float m_xOffset, m_yOffset;
	};

	template<EventType T>
	class ENGINE_API MouseButtonEvent : public MouseEvent<T>
	{
	public:
		int GetMouseButton() const { return m_button; }

	protected:
		MouseButtonEvent(int button)
			: m_button(button) {}

		int m_button;
	};

	class ENGINE_API MouseButtonPressedEvent : public MouseButtonEvent<EventType::MouseButtonPressed>
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button;
			return ss.str();
		}
	};

	class ENGINE_API MouseButtonReleasedEvent : public MouseButtonEvent<EventType::MouseButtonReleased>
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button;
			return ss.str();
		}
	};
}
