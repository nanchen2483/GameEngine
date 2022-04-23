#pragma once
#include <string>

namespace Engine
{
	class Image
	{
	public:
		Image(const std::string& filePath, const bool flipVertically = false);
		~Image();

		const std::string GetFilePath() const { return m_filePath; }
		uint8_t* GetData() const { return m_data; }
		const int32_t GetWidth() const { return m_width; }
		const int32_t GetHeight() const { return m_height; }
		const uint32_t GetInternalFormat() const { return m_internalFormat; }
		const uint32_t GetDataFormat() const { return m_dataFormat; }
	private:
		const std::string m_filePath;
		uint8_t* m_data;
		int32_t m_width, m_height;
		uint32_t m_internalFormat, m_dataFormat;
	};
}
