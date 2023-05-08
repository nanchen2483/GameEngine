#include "enginepch.h"
#include "Configuration.h"

namespace Engine
{
	void Configuration::SetShowBoundingBox(bool show)
	{
		GetInstance().m_showBoundingBox = show;
	}

	Configuration& Configuration::GetInstance()
	{
		static Configuration instance;
		return instance;
	}
}
