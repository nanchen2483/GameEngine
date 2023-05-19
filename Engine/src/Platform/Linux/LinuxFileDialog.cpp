#ifdef ENGINE_PLATFORM_LINUX
#include "enginepch.h"
#include "Engine/File/FileDialog.h"
#include "Engine/Renderer/Graphics/IGraphicsLibrary.h"

#include <gtk/gtk.h>

namespace Engine
{
	std::string FileDialog::OpenFile(const char* filter)
	{
		std::string result;

		// Initialize GTK
		gtk_init(nullptr, nullptr);

		// Create a file chooser dialog
		GtkWidget* dialog = gtk_file_chooser_dialog_new("Open File",
			GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL)),
			GTK_FILE_CHOOSER_ACTION_OPEN,
			"Cancel",
			GTK_RESPONSE_CANCEL,
			"Open",
			GTK_RESPONSE_ACCEPT,
			nullptr);

		// Set the file filter
		GtkFileFilter* file_filter = gtk_file_filter_new();
		gtk_file_filter_add_pattern(file_filter, filter);
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), file_filter);

		// Run the dialog
		if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		{
			char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			result = filename;
			g_free(filename);
		}

		// Destroy the dialog
		gtk_widget_destroy(dialog);

		// Clean up GTK
		gtk_main_quit();

		return result;
	}

	std::string FileDialog::SaveFile(const char* filter)
	{
		std::string result;

		// Initialize GTK
		gtk_init(nullptr, nullptr);

		// Create a file chooser dialog
		GtkWidget* dialog = gtk_file_chooser_dialog_new("Save File",
			GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL)),
			GTK_FILE_CHOOSER_ACTION_SAVE,
			"Cancel",
			GTK_RESPONSE_CANCEL,
			"Save",
			GTK_RESPONSE_ACCEPT,
			nullptr);

		// Set the file filter
		GtkFileFilter* file_filter = gtk_file_filter_new();
		gtk_file_filter_add_pattern(file_filter, filter);
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), file_filter);

		// Run the dialog
		if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		{
			char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			result = filename;
			g_free(filename);
		}

		// Destroy the dialog
		gtk_widget_destroy(dialog);

		// Clean up GTK
		gtk_main_quit();

		return result;
	}
}
#endif // ENGINE_PLATFORM_LINUX
