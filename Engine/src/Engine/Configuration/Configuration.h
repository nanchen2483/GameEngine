#pragma once

namespace Engine
{
	class Configuration
	{
	public:
		bool ShowBoundingBox() { return m_showBoundingBox; }
		void SetShowBoundingBox(bool show);

		static Configuration* GetInstance();
	private:
		bool m_showBoundingBox = false;
		static Configuration* s_instance;
	};
}
