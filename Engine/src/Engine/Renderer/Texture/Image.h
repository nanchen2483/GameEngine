#pragma once
#include <string>

namespace Engine
{
	class Image
	{
	public:
		Image(const std::string& filePath, const bool flipVertically = true);
		~Image();

		const std::string GetFilePath() const { return m_filePath; }
		uint8_t* GetData() const { return m_data; }
		const int32_t GetWidth() const { return m_width; }
		const int32_t GetHeight() const { return m_height; }
		const int32_t GetChannels() const { return m_channels; }
		const std::string& GetName() const { return m_imageName; }
	private:
		const std::string m_filePath;
		uint8_t* m_data;
		int32_t m_width, m_height;
		int32_t m_channels;
		std::string m_imageName;
	};
}
