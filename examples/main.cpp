#include <imgui_app_model.h>

struct document
{
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

	void react(app_model_events::draw_menu const&) {}

	void react(app_model_events::draw_content const&) {}

	void react(app_model_events::quit const&) {}
};

using application_model	 = app_model<document>::single_document_model;
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
