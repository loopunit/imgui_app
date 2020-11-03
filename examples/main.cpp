#include <imgui_app.h>

#include <tinyfsm.hpp>

#include <fmt/format.h>

struct application_model_events
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

	enum operation_async_result
	{
		success,
		fail
	};
};

struct application_model_utils
{
	template<typename T_ON_FAIL, typename T_ON_SUCCESS, typename T_FSM>
	struct async : T_FSM
	{
		using T_FSM::T_FSM;

		using self_impl = async<T_ON_FAIL, T_ON_SUCCESS, T_FSM>;

		using operation_future = imgui_app::details::future_helper<std::future<application_model_events::operation_async_result>>;
		operation_future m_operation_future;

		template<typename T_FUNC>
		void set_impl_call(T_FUNC func)
		{
			m_operation_future = operation_future{true, std::async(std::launch::async, [func]() { return func(); })};
		}

		virtual void react(application_model_events::update const&)
		{
			if (m_operation_future.is_active())
			{
				if (m_operation_future.is_ready())
				{
					auto val = m_operation_future.acquire_value();
					if (val != application_model_events::operation_async_result::success)
					{
						transit<T_ON_FAIL>();
					}
					else
					{
						transit<T_ON_SUCCESS>();
					}
				}
			}
			else
			{
				transit<T_ON_FAIL>();
			}
		}
	};

	//

	template<typename T_ON_FAIL, typename T_ON_SUCCESS, typename T_FSM>
	struct countdown : T_FSM
	{
		using T_FSM::T_FSM;

		using self_impl = countdown<T_ON_FAIL, T_ON_SUCCESS, T_FSM>;
		imgui_app::timer_future m_timer_future;

		void set_impl_timer(int msecs)
		{
			m_timer_future = imgui_app::set_timer_for_msecs(msecs);
		}

		void on_fail()
		{
			transit<T_ON_FAIL>();
		}

		void on_success()
		{
			transit<T_ON_SUCCESS>();
		}

		virtual void react(application_model_events::update const&)
		{
			if (m_timer_future.is_active())
			{
				if (m_timer_future.is_ready())
				{
					auto val = m_timer_future.acquire_value();
					if (val != 0)
					{
						on_fail();
					}
					else
					{
						on_success();
					}
				}
			}
			else
			{
				on_fail();
			}
		}
	};

	//

	template<typename T_ON_CANCEL, typename T_ON_SUCCESS, typename T_FSM>
	struct file_open_dlg : T_FSM
	{
		using T_FSM::T_FSM;

		using self_impl = file_open_dlg<T_ON_CANCEL, T_ON_SUCCESS, T_FSM>;
		imgui_app::file_open_dialog_future m_dialog_future;

		void impl_show_dialog(std::string_view origin, std::string_view filter)
		{
			m_dialog_future = imgui_app::show_file_open_dialog(origin, filter);
		}

		virtual void react(application_model_events::update const&)
		{
			if (m_dialog_future.is_active())
			{
				if (m_dialog_future.is_ready())
				{
					if (auto val = m_dialog_future.acquire_value())
					{
						fsm::emplace_pending_filename(std::move(*val));
						transit<T_ON_SUCCESS>();
					}
					else
					{
						transit<T_ON_CANCEL>();
					}
				}
			}
			else
			{
				transit<T_ON_CANCEL>();
			}
		}
	};

	//

	template<typename T_ON_CANCEL, typename T_ON_SUCCESS, typename T_FSM>
	struct file_save_dlg : T_FSM
	{
		using T_FSM::T_FSM;

		using self_impl = file_save_dlg<T_ON_CANCEL, T_ON_SUCCESS, T_FSM>;
		imgui_app::file_save_dialog_future m_dialog_future;

		void impl_show_dialog(std::string_view origin, std::string_view filter)
		{
			m_dialog_future = imgui_app::show_file_save_dialog(origin, filter);
		}

		virtual void react(application_model_events::update const&)
		{
			if (m_dialog_future.is_active())
			{
				if (m_dialog_future.is_ready())
				{
					if (auto val = m_dialog_future.acquire_value())
					{
						fsm::emplace_pending_filename(std::move(*val));
						transit<T_ON_SUCCESS>();
					}
					else
					{
						transit<T_ON_CANCEL>();
					}
				}
			}
			else
			{
				transit<T_ON_CANCEL>();
			}
		}
	};

	//

	template<typename T_ON_DONE, typename T_FSM>
	struct messagebox_ok : T_FSM
	{
		using T_FSM::T_FSM;

		using self_impl = messagebox_ok<T_ON_DONE, T_FSM>;
		imgui_app::messagebox_future m_dialog_future;

		void impl_show_dialog(const char* message, const char* title, imgui_app::messagebox_style style)
		{
			m_dialog_future = imgui_app::show_messagebox(message, title, style, imgui_app::messagebox_buttons::ok);
		}

		virtual void react(application_model_events::update const&)
		{
			if (m_dialog_future.is_active())
			{
				if (m_dialog_future.is_ready())
				{
					if (auto val = m_dialog_future.acquire_value())
					{
						transit<T_ON_DONE>();
					}
					else
					{
						transit<T_ON_DONE>();
					}
				}
			}
			else
			{
				transit<T_ON_DONE>();
			}
		}
	};

	//

	template<typename T_ON_DONE, typename T_FSM>
	struct messagebox_quit : T_FSM
	{
		using T_FSM::T_FSM;

		using self_impl = messagebox_quit<T_ON_DONE, T_FSM>;
		imgui_app::messagebox_future m_dialog_future;

		void impl_show_dialog(const char* message, const char* title, imgui_app::messagebox_style style)
		{
			m_dialog_future = imgui_app::show_messagebox(message, title, style, imgui_app::messagebox_buttons::quit);
		}

		virtual void react(application_model_events::update const&)
		{
			if (m_dialog_future.is_active())
			{
				if (m_dialog_future.is_ready())
				{
					m_dialog_future.acquire_value();
					transit<T_ON_DONE>();
				}
			}
			else
			{
				transit<T_ON_DONE>();
			}
		}
	};

	//

	template<typename T_ON_OK, typename T_ON_CANCEL, typename T_FSM>
	struct messagebox_ok_cancel : T_FSM
	{
		using T_FSM::T_FSM;

		using self_impl = messagebox_ok_cancel<T_ON_OK, T_ON_CANCEL, T_FSM>;
		imgui_app::messagebox_future m_dialog_future;

		void impl_show_dialog(const char* message, const char* title, imgui_app::messagebox_style style)
		{
			m_dialog_future = imgui_app::show_messagebox(message, title, style, imgui_app::messagebox_buttons::okcancel);
		}

		virtual void react(application_model_events::update const&)
		{
			if (m_dialog_future.is_active())
			{
				if (m_dialog_future.is_ready())
				{
					if (auto val = m_dialog_future.acquire_value())
					{
						if (*val == imgui_app::messagebox_result::ok)
						{
							transit<T_ON_OK>();
						}
						else
						{
							transit<T_ON_CANCEL>();
						}
					}
					else
					{
						transit<T_ON_CANCEL>();
					}
				}
			}
			else
			{
				transit<T_ON_CANCEL>();
			}
		}
	};

	//

	template<typename T_ON_YES, typename T_ON_NO, typename T_FSM>
	struct messagebox_yes_no : T_FSM
	{
		using T_FSM::T_FSM;

		using self_impl = messagebox_yes_no<T_ON_YES, T_ON_NO, T_FSM>;
		imgui_app::messagebox_future m_dialog_future;

		void impl_show_dialog(const char* message, const char* title, imgui_app::messagebox_style style)
		{
			m_dialog_future = imgui_app::show_messagebox(message, title, style, imgui_app::messagebox_buttons::yesno);
		}

		virtual void react(application_model_events::update const&)
		{
			if (m_dialog_future.is_active())
			{
				if (m_dialog_future.is_ready())
				{
					auto val = m_dialog_future.acquire_value();
					if (val == imgui_app::messagebox_result::yes)
					{
						transit<T_ON_YES>();
					}
					else
					{
						transit<T_ON_NO>();
					}
				}
			}
			else
			{
				transit<T_ON_NO>();
			}
		}
	};
};

//

struct document_file_menu
{
	enum class mode
	{
		locked,
		empty,
		active,
	};

	enum class result
	{
		on_new,
		on_close,
		on_open,
		on_save,
		on_save_as,
		on_quit,
		nothing
	};

	static result draw(mode m)
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

				ImGui::MenuItem("Save", "Ctrl+S", nullptr, false);

				ImGui::MenuItem("Save As", "Ctrl+Shift+S", nullptr, false);

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
				ImGui::MenuItem("New", nullptr, nullptr, false);

				if (ImGui::MenuItem("Open"))
				{
					r = result::on_open;
				}

				if (ImGui::MenuItem("Close"))
				{
					r = result::on_close;
				}

				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					r = result::on_save;
				}

				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
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

//

template<typename T_MODEL>
struct implement_application : T_MODEL
{
	using fsm_handle = typename T_MODEL::fsm_handle;

	static inline void start()
	{
		fsm_handle::start();
	}

	static inline void reset()
	{
		fsm_handle::reset();
	}

	template<typename... T>
	static inline void dispatch(T... args)
	{
		fsm_handle::dispatch(args...);
	}

	static inline bool is_running() noexcept
	{
		return fsm::m_running;
	}
};

//

struct single_document_model : application_model_events
{
	// struct document
	//{
	//};

	struct fsm : tinyfsm::Fsm<fsm>
	{
		// static inline std::unique_ptr<document> m_document;

		static inline std::string m_filename;
		static inline std::string m_pending_filename;

		static inline constexpr uint32_t invalid_hash	= 0xffffffff;
		static inline constexpr uint32_t first_hash		= 0x00000000;
		static inline uint32_t			 m_saved_hash	= invalid_hash;
		static inline uint32_t			 m_pending_hash = invalid_hash;

		// static inline int create_document()
		//{
		//	m_document.reset(new document());
		//}
		//
		// static inline bool destroy_document()
		//{
		//	m_document.reset();
		//}
		//
		// static inline document* get_document()
		//{
		//	return m_document.get();
		//}

		static inline operation_async_result async_init_empty()
		{
			return operation_async_result::success;
		}

		static inline operation_async_result async_destroy_empty()
		{
			return operation_async_result::success;
		}

		static inline operation_async_result async_create_new_document()
		{
			m_saved_hash   = invalid_hash;
			m_pending_hash = first_hash;
			return operation_async_result::success;
		}

		static inline operation_async_result async_open_pending_document()
		{
			m_saved_hash   = first_hash;
			m_pending_hash = first_hash;
			return operation_async_result::success;
		}

		static inline void mark_document_changed()
		{
			while (m_pending_hash != invalid_hash)
			{
				++m_pending_hash;
			}
		}

		static inline operation_async_result async_save_pending_document()
		{
			m_saved_hash = m_pending_hash;
			return operation_async_result::success;
		}

		static inline operation_async_result async_init_pending_document()
		{
			return operation_async_result::success;
		}

		static inline const std::string& current_filename()
		{
			return m_filename;
		}

		static inline void emplace_pending_filename(std::string&& str)
		{
			m_pending_filename = std::move(str);
			imgui_app::info(fmt::format("Pending document is: {}", m_pending_filename).c_str());
		}

		static inline bool needs_filename()
		{
			return m_filename.length() == 0;
		}

		static inline bool needs_to_save()
		{
			return m_saved_hash != m_pending_hash;
		}

		const std::string m_name;
		fsm(const char* name) : m_name(name) {}

		static inline bool m_running = false;

		virtual void react(update const&) {}

		virtual void react(draw_menu const&)
		{
			document_file_menu::draw(document_file_menu::mode::locked);
		}

		virtual void react(draw_content const&) {}

		virtual void react(quit const&) {}

		virtual void entry()
		{
			imgui_app::info(fmt::format("single_document_model: {}", m_name).c_str());
		}

		virtual void exit() {}
	};

	template<typename T_SHUTDOWN, typename T_ON_NEW, typename T_ON_OPEN>
	struct empty_substate
	{
		struct main_state;

		struct report_error : application_model_utils::messagebox_quit<T_SHUTDOWN, fsm>
		{
			report_error() : self_impl("empty_substate::report_error") {}

			virtual void entry()
			{
				self_impl::entry();
				impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
			}
		};

		struct init : application_model_utils::async<report_error, main_state, fsm>
		{
			init() : self_impl("empty_substate::init") {}

			virtual void entry()
			{
				self_impl::entry();
				set_impl_call(fsm::async_init_empty);
			}
		};

		struct destroy : application_model_utils::async<report_error, T_SHUTDOWN, fsm>
		{
			destroy() : self_impl("empty_substate::destroy") {}

			virtual void entry()
			{
				self_impl::entry();
				set_impl_call(fsm::async_destroy_empty);
			}
		};

		struct main_state : fsm
		{
			main_state() : fsm("empty_substate::main_state") {}

			virtual void entry()
			{
				fsm::entry();
			}

			virtual void react(application_model_events::draw_menu const&)
			{
				switch (document_file_menu::draw(document_file_menu::mode::empty))
				{
				case document_file_menu::result::on_new:
					transit<T_ON_NEW>();
					break;
				case document_file_menu::result::on_open:
					transit<T_ON_OPEN>();
					break;
				case document_file_menu::result::on_quit:
					transit<destroy>();
					break;
				default:
					break;
				};
			}
		};
	};

	template<typename T_EXIT, typename T_SUCCESS>
	struct new_from_empty_substate
	{
		struct main_state;

		struct report_error : application_model_utils::messagebox_quit<T_EXIT, fsm>
		{
			report_error() : self_impl("new_from_empty_substate::report_error") {}

			virtual void entry()
			{
				self_impl::entry();
				impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
			}
		};

		struct init : fsm
		{
			init() : fsm("new_from_empty_substate::init") {}

			virtual void entry()
			{
				fsm::entry();
				transit<main_state>();
			}
		};

		struct main_state : application_model_utils::async<report_error, T_SUCCESS, fsm>
		{
			main_state() : self_impl("new_from_empty_substate::main_state") {}

			virtual void entry()
			{
				self_impl::entry();
				set_impl_call(fsm::async_create_new_document);
			}
		};
	};

	template<typename T_EXIT, typename T_SUCCESS>
	struct open_from_empty_substate
	{
		struct main_state;

		struct report_error : application_model_utils::messagebox_quit<T_EXIT, fsm>
		{
			report_error() : self_impl("open_from_empty_substate::report_error") {}

			virtual void entry()
			{
				self_impl::entry();
				impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
			}
		};

		struct init : application_model_utils::file_open_dlg<T_EXIT, main_state, fsm>
		{
			init() : self_impl("open_from_empty_substate::init") {}

			virtual void entry()
			{
				self_impl::entry();
				impl_show_dialog(fsm::current_filename(), "");
			}
		};

		struct main_state : application_model_utils::async<report_error, T_SUCCESS, fsm>
		{
			main_state() : self_impl("open_from_empty_substate::main_state") {}

			virtual void entry()
			{
				self_impl::entry();
				set_impl_call(fsm::async_open_pending_document);
			}
		};
	};

	template<typename T_CLOSE, typename T_EXIT>
	struct ready_substate
	{
		struct main_state;

		template<typename T_SUCCESS, typename T_FAIL>
		struct save_substate
		{
			struct main_state;

			struct report_error : application_model_utils::messagebox_quit<T_FAIL, fsm>
			{
				report_error() : self_impl("save_substate::report_error") {}

				virtual void entry()
				{
					self_impl::entry();
					impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
				}
			};

			struct init_needs_filename : application_model_utils::file_save_dlg<T_FAIL, main_state, fsm>
			{
				init_needs_filename() : self_impl("save_substate::init") {}

				virtual void entry()
				{
					self_impl::entry();
					impl_show_dialog(fsm::current_filename(), "");
				}
			};

			struct init : fsm
			{
				init() : fsm("save_substate::init") {}

				virtual void entry()
				{
					fsm::entry();

					if (fsm::needs_filename())
					{
						transit<init_needs_filename>();
					}
					else
					{
						fsm::emplace_pending_filename(std::string(fsm::current_filename()));
						transit<main_state>();
					}
				}
			};

			struct main_state : application_model_utils::async<report_error, T_SUCCESS, fsm>
			{
				main_state() : self_impl("save_substate::main_state") {}

				virtual void entry()
				{
					self_impl::entry();
					set_impl_call(fsm::async_save_pending_document);
				}
			};
		};

		template<typename T_SUCCESS, typename T_FAIL>
		struct save_as_substate
		{
			struct main_state;

			struct report_error : application_model_utils::messagebox_quit<T_FAIL, fsm>
			{
				report_error() : self_impl("save_as_substate::report_error") {}

				virtual void entry()
				{
					self_impl::entry();
					impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
				}
			};

			struct init : application_model_utils::file_save_dlg<T_FAIL, main_state, fsm>
			{
				init() : self_impl("save_as_substate::init") {}

				virtual void entry()
				{
					self_impl::entry();
					impl_show_dialog(fsm::current_filename(), "");
				}
			};

			struct main_state : application_model_utils::async<report_error, T_SUCCESS, fsm>
			{
				main_state() : self_impl("save_as_substate::main_state") {}

				virtual void entry()
				{
					self_impl::entry();
					set_impl_call(fsm::async_save_pending_document);
				}
			};
		};

		template<typename T_SUCCESS, typename T_FAIL>
		struct close_substate
		{
			struct main_state;

			struct report_error : application_model_utils::messagebox_quit<T_FAIL, fsm>
			{
				report_error() : self_impl("close_substate::report_error") {}

				virtual void entry()
				{
					self_impl::entry();
					impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
				}
			};

			struct init_save : application_model_utils::file_save_dlg<T_FAIL, main_state, fsm>
			{
				init_save() : self_impl("close_substate::init") {}

				virtual void entry()
				{
					self_impl::entry();
					impl_show_dialog(fsm::current_filename(), "");
				}
			};

			struct init : application_model_utils::messagebox_yes_no<init_save, T_SUCCESS, fsm>
			{
				init() : self_impl("close_substate::init") {}

				virtual void entry()
				{
					self_impl::entry();
					if (fsm::needs_to_save())
					{
						impl_show_dialog("You have unsaved changes, would you like to save?", "Document has changes", imgui_app::messagebox_style::warning);
					}
					else
					{
						transit<T_SUCCESS>();
					}
				}
			};

			struct main_state : application_model_utils::async<report_error, T_SUCCESS, fsm>
			{
				main_state() : self_impl("close_substate::main_state") {}

				virtual void entry()
				{
					self_impl::entry();
					set_impl_call(fsm::async_save_pending_document);
				}
			};
		};

		struct report_error : application_model_utils::messagebox_quit<T_EXIT, fsm>
		{
			report_error() : self_impl("ready_substate::report_error") {}

			virtual void entry()
			{
				self_impl::entry();
				impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
			}
		};

		struct init : application_model_utils::async<report_error, main_state, fsm>
		{
			init() : self_impl("ready_substate::init") {}

			virtual void entry()
			{
				self_impl::entry();
				set_impl_call(fsm::async_init_pending_document);
			}
		};

		struct main_state : fsm
		{
			main_state() : fsm("ready_substate::main_state") {}

			virtual void entry()
			{
				fsm::entry();
			}

			virtual void react(application_model_events::draw_menu const&)
			{
				switch (document_file_menu::draw(document_file_menu::mode::active))
				{
				case document_file_menu::result::on_close:
					transit<close_substate<T_CLOSE, main_state>::init>();
					break;
				case document_file_menu::result::on_save:
					transit<save_substate<main_state, main_state>::init>();
					break;
				case document_file_menu::result::on_save_as:
					transit<save_as_substate<main_state, main_state>::init>();
					break;
				case document_file_menu::result::on_quit:
					transit<close_substate<T_EXIT, main_state>::init>();
					break;
				default:
					break;
				};
			}
		};
	};

	struct shutdown;
	struct substate_new;
	struct substate_open;
	struct substate_ready_from_new;
	struct substate_ready_from_open;
	struct substate_empty;
	struct substate_ready;

	using empty			  = empty_substate<shutdown, substate_new, substate_open>;
	using new_from_empty  = new_from_empty_substate<substate_empty, substate_ready_from_new>;
	using open_from_empty = open_from_empty_substate<substate_empty, substate_ready_from_open>;
	using ready			  = ready_substate<substate_empty, shutdown>;

	struct substate_empty : empty::init
	{
	};
	struct substate_new : new_from_empty::init
	{
	};
	struct substate_open : open_from_empty::init
	{
	};
	struct substate_ready_from_new : ready::init
	{
	};
	struct substate_ready_from_open : ready::init
	{
	};
	struct substate_ready : ready
	{
	};

	struct bootstrap : application_model_utils::countdown<shutdown, substate_empty, fsm>
	{
		bootstrap() : self_impl("bootstrap") {}

		virtual void entry()
		{
			m_running = true;
			set_impl_timer(1000);
		}
	};

	// terminal
	struct shutdown : fsm
	{
		shutdown() : fsm("shutdown") {}

		virtual void entry()
		{
			m_running = false;
		}
	};

	using fsm_handle = tinyfsm::FsmList<fsm>;
};

// using application_model	 = nested_application_model<single_document_model>;

using application_model	 = single_document_model;
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
