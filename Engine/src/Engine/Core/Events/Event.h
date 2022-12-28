#pragma once
#include "Engine/core/Log/Log.h"

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
		static char* ToString(EventType eventType)
		{
			switch (eventType)
			{
			case Engine::EventType::None:					return "None";
			case Engine::EventType::WindowClose:			return "WindowClose";
			case Engine::EventType::WindowResize:			return "WindowResize";
			case Engine::EventType::WindowFocus:			return "WindowFocus";
			case Engine::EventType::WindowLostFocus:		return "WindowLostFocus";
			case Engine::EventType::WindowMoved:			return "WindowMoved";
			case Engine::EventType::ApplicationTick:		return "ApplicationTick";
			case Engine::EventType::ApplicationUpdate:		return "ApplicationUpdate";
			case Engine::EventType::ApplicationRender:		return "ApplicationRender";
			case Engine::EventType::KeyPressed:				return "KeyPressed";
			case Engine::EventType::KeyReleased:			return "KeyReleased";
			case Engine::EventType::KeyTyped:				return "KeyTyped";
			case Engine::EventType::MouseButtonPressed:		return "MouseButtonPressed";
			case Engine::EventType::MouseButtonReleased:	return "MouseButtonReleased";
			case Engine::EventType::MouseMoved:				return "MouseMoved";
			case Engine::EventType::MouseScrolled:			return "MouseScrolled";
			default:
				ENGINE_CORE_ASSERT(false, "Unsupported EventType")
				return "Unknown EventType";
			}
		}
	}
}
