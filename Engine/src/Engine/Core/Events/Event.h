#pragma once
#include "Engine/Core/Log/Log.h"

#include <functional>
#include <string>

namespace Engine
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		ApplicationTick, ApplicationUpdate, ApplicationRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum class EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4),
	};

	class ENGINE_API Event
	{
	public:
		virtual ~Event() = default;
		bool handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int32_t GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsIncategory(EventCategory category)
		{
			return GetCategoryFlags() && category != EventCategory::None;
		}
	};

	class EventDispatcher
	{
		template<typename T, typename std::enable_if<std::is_base_of<Event, T>::value>::type* = nullptr>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_event(event) {}

		template<typename T, typename std::enable_if<std::is_base_of<Event, T>::value>::type* = nullptr>
		bool Dispatch(EventFn<T> func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.handled |= func(*(T*)&m_event);
				return true;
			}

			return false;
		}
	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

	namespace EventUtil
	{
		static const char* ToString(EventType eventType)
		{
			switch (eventType)
			{
			case EventType::None:					return "None";
			case EventType::WindowClose:			return "WindowClose";
			case EventType::WindowResize:			return "WindowResize";
			case EventType::WindowFocus:			return "WindowFocus";
			case EventType::WindowLostFocus:		return "WindowLostFocus";
			case EventType::WindowMoved:			return "WindowMoved";
			case EventType::ApplicationTick:		return "ApplicationTick";
			case EventType::ApplicationUpdate:		return "ApplicationUpdate";
			case EventType::ApplicationRender:		return "ApplicationRender";
			case EventType::KeyPressed:				return "KeyPressed";
			case EventType::KeyReleased:			return "KeyReleased";
			case EventType::KeyTyped:				return "KeyTyped";
			case EventType::MouseButtonPressed:		return "MouseButtonPressed";
			case EventType::MouseButtonReleased:	return "MouseButtonReleased";
			case EventType::MouseMoved:				return "MouseMoved";
			case EventType::MouseScrolled:			return "MouseScrolled";
			default:
				ENGINE_CORE_ASSERT(false, "Unsupported EventType")
				return "Unknown EventType";
			}
		}
	}
}
