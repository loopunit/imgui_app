#include <imgui_app.h>

// Main code
int main(int, char**)
{
	if (imgui_app::select_platform(imgui_app_fw::platform::win32_dx12))
	{
		if (imgui_app::init())
		{
			imgui_app::info("Welcome to the imgui-console example!");
			imgui_app::info("The following variables have been exposed to the console:");
			imgui_app::info("\tbackground_color - set: [int int int int]");
			imgui_app::info("Try running the following command:");
			imgui_app::error("\tset background_color [255 0 0 255]");

			bool   show_demo_window = true;
			ImVec4 clear_color		= ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			while (imgui_app::pump())
			{
				imgui_app::begin_frame();

				if (show_demo_window)
				{
					ImGui::ShowDemoWindow(&show_demo_window);
				}

				imgui_app::end_frame(clear_color);
			}

			imgui_app::destroy();
		}
	}

	return 0;
}
