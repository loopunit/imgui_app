#include <imgui_app.h>


// Main code
int main(int, char**)
{
	if (imgui_app_fw::select_platform(imgui_app_fw::platform::win32_dx12))
	{
		if (imgui_app_fw::init())
		{
			
			imgui_app::logger()->info("Welcome to the imgui-console example!");
			imgui_app::logger()->info("The following variables have been exposed to the console:");
			imgui_app::logger()->info("\tbackground_color - set: [int int int int]");
			imgui_app::logger()->info("Try running the following command:");
			imgui_app::logger()->error("\tset background_color [255 0 0 255]");

			bool   show_demo_window = true;
			ImVec4 clear_color		= ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			while (imgui_app_fw::pump())
			{
				imgui_app_fw::begin_frame();

				if (show_demo_window)
					ImGui::ShowDemoWindow(&show_demo_window);

				imgui_app::logger()->draw();

				imgui_app_fw::end_frame(clear_color);
			}

			imgui_app_fw::destroy();
		}
	}

	return 0;
}
