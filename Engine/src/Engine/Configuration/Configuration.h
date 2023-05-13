#pragma once
#include "Engine/Core/Base.h"

namespace Engine
{
	class ENGINE_API Configuration
	{
	public:
		static bool ShowBoundingBox() { return GetInstance().m_showBoundingBox; }
		static void SetShowBoundingBox(bool show);
	private:
		static Configuration& GetInstance();

		bool m_showBoundingBox = false;
	};
}
