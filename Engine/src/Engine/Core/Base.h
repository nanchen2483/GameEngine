#pragma once
#include <memory>

#ifdef ENGINE_PLATFORM_WINDOWS
	#if ENGINE_DYNAMIC_LINK
		#ifdef ENGINE_BUILD_DLL
			#define ENGINE_API __declspec(dllexport)
			#define ENGINE_TEST_API __declspec(dllexport)
		#else
			#define ENGINE_API __declspec(dllimport)
			#ifdef ENGINE_TEST_DLL
				#define ENGINE_TEST_API __declspec(dllimport)
			#else
				#define ENGINE_TEST_API
			#endif // ENGINE_TEST_DLL
		#endif // ENGINE_BUILD_DLL
	#else
		#define ENGINE_API
		#define ENGINE_TEST_API
	#endif // ENGINE_DYNAMIC_LINK
#else
	#error Engine only support windows
#endif // ENGINE_PLATFORM_WINDOWS

#ifdef ENGINE_DEBUG
	#define ENGINE_ENABLE_ASSERTS
#endif // ENGINE_DEBUG


#ifdef ENGINE_ENABLE_ASSERTS
	#define ENGINE_ASSERT(x, ...) { if(!(x)) { ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define ENGINE_CORE_ASSERT(x, ...) { if(!(x)) { ENGINE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ENGINE_ASSERT(x, ...)
	#define ENGINE_CORE_ASSERT(x, ...)
#endif // ENGINE_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define ENGINE_BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Engine
{
	template<typename T>
	using Uniq = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Uniq<T> CreateUniq(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ptr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ptr<T> CreatePtr(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Weak = std::weak_ptr<T>;

	template<typename T>
	struct is_smart_pointer : std::false_type {};

	template<typename T>
	struct is_smart_pointer<Uniq<T>> : std::true_type {};

	template<typename T>
	struct is_smart_pointer<Ptr<T>> : std::true_type {};

	template<typename T>
	struct is_smart_pointer<Weak<T>> : std::true_type {};

	template<typename T>
	inline constexpr bool is_smart_pointer_v = is_smart_pointer<std::remove_cv_t<T>>::value;
}
