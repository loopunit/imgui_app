#include <imgui_app_model.h>

struct document : document_file_menu
{
	struct app_model_events
	{
		struct update : tinyfsm::Event
		{
		};

		struct draw_menu : tinyfsm::Event
		{
		};

		struct draw_content : tinyfsm::Event
		{
		};

		struct quit : tinyfsm::Event
		{
		};
	};

	static inline constexpr uint32_t invalid_hash	= 0xffffffff;
	static inline constexpr uint32_t first_hash		= 0x00000000;
	uint32_t						 m_saved_hash	= invalid_hash;
	uint32_t						 m_pending_hash = invalid_hash;

	document()
	{
		m_saved_hash   = invalid_hash;
		m_pending_hash = first_hash;
	}

	document(const std::string& path)
	{
		m_saved_hash   = first_hash;
		m_pending_hash = first_hash;
	}

	bool save(const std::string& path)
	{
		m_saved_hash = m_pending_hash;
		return true;
	}

	bool needs_to_save()
	{
		return m_saved_hash != m_pending_hash;
	}

	void react(app_model_events::update const&) {}

	void react(app_model_events::draw_content const&)
	{
		ImGui::Begin("Dear ImGui Style Editor");
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	static result draw_menu(document* self, mode m)
	{
		auto r = result::nothing;
		if (m == mode::empty)
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					r = result::on_new;
				}

				ImGui::MenuItem("Close", nullptr, nullptr, false);

				if (ImGui::MenuItem("Open"))
				{
					r = result::on_open;
				}

				ImGui::MenuItem("Save", "Ctrl S", nullptr, false);

				ImGui::MenuItem("Save As", "Ctrl Shift S", nullptr, false);

				if (ImGui::MenuItem("Quit"))
				{
					r = result::on_quit;
				}
				ImGui::EndMenu();
			}
		}
		else if (m == mode::locked)
		{
			if (ImGui::BeginMenu("File", false))
			{
				ImGui::MenuItem("New", nullptr, false);

				ImGui::MenuItem("Open", nullptr, false);

				ImGui::MenuItem("Save", "Ctrl S", nullptr, false);

				ImGui::MenuItem("Save As", "Ctrl Shift S", nullptr, false);

				ImGui::EndMenu();
			}
		}
		else
		{
			assert(m == mode::active);
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					r = result::on_new;
				}

				if (ImGui::MenuItem("Open"))
				{
					r = result::on_open;
				}

				if (ImGui::MenuItem("Close"))
				{
					r = result::on_close;
				}

				if (ImGui::MenuItem("Save", "Ctrl S"))
				{
					r = result::on_save;
				}

				if (ImGui::MenuItem("Save As", "Ctrl Shift S"))
				{
					r = result::on_save_as;
				}

				if (ImGui::MenuItem("Quit"))
				{
					r = result::on_quit;
				}
				ImGui::EndMenu();
			}
		}

		return r;
	}
};

using application_model	 = app_model<document, document::app_model_events>::single_document_model;
using simple_application = implement_application<application_model>;
FSM_INITIAL_STATE(application_model::fsm, application_model::bootstrap);

int main(int, char**)
{
	if (imgui_app::select_platform(imgui_app_fw::platform::win32_dx11))
	{
		if (imgui_app::init())
		{
			imgui_app::set_window_title("Hello?");

			bool   show_demo_window = true;
			ImVec4 clear_color		= ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			simple_application::start();

			while (imgui_app::pump() && simple_application::is_running())
			{
				imgui_app::begin_frame();
				simple_application::dispatch(simple_application::update{});

				if (ImGui::BeginMainMenuBar())
				{
					simple_application::dispatch(simple_application::draw_menu{});
					ImGui::EndMainMenuBar();
				}
				simple_application::dispatch(simple_application::draw_content{});

				imgui_app::end_frame(clear_color);
			}

			imgui_app::destroy();
		}
	}

	return 0;
}
