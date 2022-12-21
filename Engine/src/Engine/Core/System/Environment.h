#pragma once

namespace Engine
{
	class Environment
	{
	public:
		static bool IsDevelopment()
		{
#ifdef ENGINE_DEBUG
			return true;
#else
			return false;
#endif // ENGINE_DEBUG
		}

		static bool IsPoduction()
		{
#ifdef ENGINE_RELEASE
			return true;
#else
			return false;
#endif // ENGINE_RELEASE

		}
	};
}
