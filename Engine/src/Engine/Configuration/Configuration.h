#pragma once

namespace Engine
{
	class Configuration
	{
	public:
		bool ShowBoudingBox() { return m_showBoudingBox; }
		void SetShowBoudingBox(bool show);

		static Configuration* GetInstance();
	private:
		bool m_showBoudingBox = false;
		static Configuration* s_instance;
	};
}
