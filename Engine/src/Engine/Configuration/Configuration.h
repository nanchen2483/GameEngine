#pragma once

namespace Engine
{
	class Configuration
	{
	public:
		static bool ShowBoundingBox() { return GetInstance().m_showBoundingBox; }
		static void SetShowBoundingBox(bool show);
	private:
		static Configuration& GetInstance();

		bool m_showBoundingBox = false;
	};
}
