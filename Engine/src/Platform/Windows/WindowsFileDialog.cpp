#include "enginepch.h"
#include "Engine/File/FileDialog.h"
#include "Engine/Renderer/Graphics/IGraphicsLibrary.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include <Windows.h>
#include <commdlg.h>

namespace Engine
{
	std::string FileDialog::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = static_cast<HWND>(IGraphicsLibrary::GetInstance().GetWin32Window());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}

	std::string FileDialog::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = static_cast<HWND>(IGraphicsLibrary::GetInstance().GetWin32Window());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}
}
#else
namespace Engine
{
	std::string FileDialog::OpenFile(const char* filter)
	{
		return std::string();
	}

	std::string FileDialog::SaveFile(const char* filter)
	{
		return std::string();
	}
}

#endif // ENGINE_PLATFORM_WINDOWS
