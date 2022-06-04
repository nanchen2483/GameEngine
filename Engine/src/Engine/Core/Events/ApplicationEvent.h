#pragma once
#include "Event.h"

#include <sstream>

namespace Engine
{
	template<EventType T>
	class ENGINE_API ApplicationEvent : public Event
	{
	public:
		static EventType GetStaticType() { return T; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return EventUtil::ToString(T); }

		virtual int32_t GetCategoryFlags() const override
		{
			return (int32_t)EventCategory::EventCategoryApplication;
		}
	};

	class ENGINE_API WindowResizeEvent : public ApplicationEvent<EventType::WindowResize>
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_width(width), m_height(height) {}

		unsigned int GetWidth() const { return m_width; }
		unsigned int GetHeight() const { return m_height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_width << ", " << m_height;
			return ss.str();
		}
	private:
		unsigned int m_width, m_height;
	};

	class ENGINE_API WindowCloseEvent : public ApplicationEvent<EventType::WindowClose>
	{
	public:
		WindowCloseEvent() = default;
	};

	class ENGINE_API ApplicationTickEvent : public ApplicationEvent<EventType::ApplicationTick>
	{
	public:
		ApplicationTickEvent() = default;
	};

	class ENGINE_API ApplicationUpdateEvent : public ApplicationEvent<EventType::ApplicationUpdate>
	{
	public:
		ApplicationUpdateEvent() = default;
	};

	class ENGINE_API ApplicationRenderEvent : public ApplicationEvent<EventType::ApplicationRender>
	{
	public:
		ApplicationRenderEvent() = default;
	};
}
