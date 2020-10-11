#include <imgui_app.h>

#include <imgui_console.h>

#include <reckless/severity_log.hpp>
#include <reckless/stdout_writer.hpp>

class ImGuiConsole_custom : public ImGuiConsole
{
public:
	void Draw()
	{
		///////////////////////////////////////////////////////////////////////////
		// Window and Settings ////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////

		// Begin Console Window.
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_WindowAlpha);
		if (!ImGui::Begin(m_ConsoleName.data(), nullptr, ImGuiWindowFlags_MenuBar))
		{
			ImGui::PopStyleVar();
			ImGui::End();
			return;
		}
		ImGui::PopStyleVar();

		///////////////
		// Menu bar  //
		///////////////
		MenuBar();

		////////////////
		// Filter bar //
		////////////////
		if (m_FilterBar)
		{
			FilterBar();
		}

		//////////////////
		// Console Logs //
		//////////////////
		LogWindow();

		// Section off.
		ImGui::Separator();

		///////////////////////////////////////////////////////////////////////////
		// Command-line ///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////

		InputBar();

		ImGui::End();
	}

protected:
	void MenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			// Settings menu.
			if (ImGui::BeginMenu("Settings"))
			{
				// Colored output
				ImGui::Checkbox("Colored Output", &m_ColoredOutput);
				ImGui::SameLine();
				HelpMaker("Enable colored command output");

				// Auto Scroll
				ImGui::Checkbox("Auto Scroll", &m_AutoScroll);
				ImGui::SameLine();
				HelpMaker("Automatically scroll to bottom of console log");

				// Filter bar
				ImGui::Checkbox("Filter Bar", &m_FilterBar);
				ImGui::SameLine();
				HelpMaker("Enable console filter bar");

				// Time stamp
				ImGui::Checkbox("Time Stamps", &m_TimeStamps);
				ImGui::SameLine();
				HelpMaker("Display command execution timestamps");

				// Reset to default settings
				if (ImGui::Button("Reset settings", ImVec2(ImGui::GetColumnWidth(), 0)))
					ImGui::OpenPopup("Reset Settings?");

				// Confirmation
				if (ImGui::BeginPopupModal("Reset Settings?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("All settings will be reset to default.\nThis operation cannot be undone!\n\n");
					ImGui::Separator();

					if (ImGui::Button("Reset", ImVec2(120, 0)))
					{
						DefaultSettings();
						ImGui::CloseCurrentPopup();
					}

					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0)))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				ImGui::Separator();

				// Logging Colors
				ImGuiColorEditFlags flags = ImGuiColorEditFlags_Float | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar;

				ImGui::TextUnformatted("Color Palette");
				ImGui::Indent();
				ImGui::ColorEdit4("Command##", (float*)&m_ColorPalette[COL_COMMAND], flags);
				ImGui::ColorEdit4("Log##", (float*)&m_ColorPalette[COL_LOG], flags);
				ImGui::ColorEdit4("Warning##", (float*)&m_ColorPalette[COL_WARNING], flags);
				ImGui::ColorEdit4("Error##", (float*)&m_ColorPalette[COL_ERROR], flags);
				ImGui::ColorEdit4("Info##", (float*)&m_ColorPalette[COL_INFO], flags);
				ImGui::ColorEdit4("Time Stamp##", (float*)&m_ColorPalette[COL_TIMESTAMP], flags);
				ImGui::Unindent();

				ImGui::Separator();

				// Window transparency.
				ImGui::TextUnformatted("Background");
				ImGui::SliderFloat("Transparency##", &m_WindowAlpha, 0.1f, 1);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}
};

template<csys::ItemType T_item_type>
class ImGuiConsole_writer : public reckless::writer
{
public:
	ImGuiConsole* m_console;

	ImGuiConsole_writer(ImGuiConsole* console) : m_console(console) {}

	std::size_t write(void const* pbuffer, std::size_t count, std::error_code& ec) noexcept override
	{
		m_console->System().Log(T_item_type) << std::string_view((const char*)pbuffer, count);
		return count;
	}
};

// Main code
int main(int, char**)
{
	if (imgui_app_fw::select_platform(imgui_app_fw::platform::win32_dx12))
	{
		if (imgui_app_fw::init())
		{
			ImGuiConsole_custom						 console;
			ImGuiConsole_writer<csys::ItemType::LOG> writer(&console);
			reckless::policy_log<>					 g_log(&writer);

			g_log.write("Welcome to the imgui-console example!");
			g_log.write("The following variables have been exposed to the console:");
			g_log.write("\tbackground_color - set: [int int int int]");
			g_log.write("Try running the following command:");
			g_log.write("\tset background_color [255 0 0 255]");

			bool   show_demo_window = true;
			ImVec4 clear_color		= ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			while (imgui_app_fw::pump())
			{
				imgui_app_fw::begin_frame();

				if (show_demo_window)
					ImGui::ShowDemoWindow(&show_demo_window);

				console.Draw();

				imgui_app_fw::end_frame(clear_color);
			}

			imgui_app_fw::destroy();
		}
	}

	return 0;
}
