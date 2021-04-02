#pragma once


namespace Engine
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1,
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return m_rendererApi; };
	private:
		static RendererAPI m_rendererApi;
	};
}