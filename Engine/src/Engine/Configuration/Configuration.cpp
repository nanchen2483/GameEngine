#include "enginepch.h"
#include "Configuration.h"

namespace Engine
{
	Configuration* Configuration::s_instance = nullptr;

	void Configuration::SetShowBoundingBox(bool show)
	{
		m_showBoundingBox = show;
	}

	Configuration* Configuration::GetInstance()
	{
		if (!s_instance)
		{
			s_instance = new Configuration();
		}

		return s_instance;
	}
}
