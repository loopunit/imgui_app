#pragma once

#include <imgui_app.h>

#include <tinyfsm.hpp>
#include <fmt/format.h>

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
};

//

template<typename T_DOCUMENT, typename T_MODEL_EVENTS>
struct app_model
{
	using model_events = typename T_MODEL_EVENTS;

	enum operation_async_result
	{
		success,
		fail
	};

	struct application_model_utils
	{
		template<typename T_ON_FAIL, typename T_ON_SUCCESS, typename T_FSM>
		struct async : T_FSM
		{
			using T_FSM::T_FSM;

			using self_impl = async<T_ON_FAIL, T_ON_SUCCESS, T_FSM>;

			using operation_future = imgui_app::details::future_helper<std::future<operation_async_result>>;
			operation_future m_operation_future;

			template<typename T_FUNC>
			void set_impl_call(T_FUNC func)
			{
				m_operation_future = operation_future{true, std::async(std::launch::async, [func]() { return func(); })};
			}

			virtual void react(typename model_events::update const& evt)
			{
				T_FSM::react(evt);

				if (m_operation_future.is_active())
				{
					if (m_operation_future.is_ready())
					{
						auto val = m_operation_future.acquire_value();
						if (val != operation_async_result::success)
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

			virtual void react(typename model_events::update const& evt)
			{
				T_FSM::react(evt);

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

			virtual void react(typename model_events::update const& evt)
			{
				T_FSM::react(evt);

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

			virtual void react(typename model_events::update const& evt)
			{
				T_FSM::react(evt);

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

			virtual void react(typename model_events::update const& evt)
			{
				T_FSM::react(evt);

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

			virtual void react(typename model_events::update const& evt)
			{
				T_FSM::react(evt);

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

			virtual void react(typename model_events::update const& evt)
			{
				T_FSM::react(evt);

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

			virtual void react(typename model_events::update const& evt)
			{
				T_FSM::react(evt);

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

	struct single_document_model : model_events
	{
		struct fsm : tinyfsm::Fsm<fsm>
		{
			static inline bool m_running = false;

			static inline std::unique_ptr<T_DOCUMENT> m_document;
			static inline std::unique_ptr<T_DOCUMENT> m_pending_document;
			static inline std::unique_ptr<T_DOCUMENT> m_outgoing_document;
			static inline std::string				  m_filename;
			static inline std::string				  m_pending_filename;

			static inline operation_async_result async_bootstrap()
			{
				return operation_async_result::success;
			}

			static inline operation_async_result async_shutdown()
			{
				return operation_async_result::success;
			}

			static inline operation_async_result async_init_empty()
			{
				m_filename.clear();
				return operation_async_result::success;
			}

			static inline operation_async_result async_destroy_empty()
			{
				return operation_async_result::success;
			}

			static inline void prepare_destroy_document()
			{
				m_outgoing_document = std::move(m_document);
			}

			static inline operation_async_result async_destroy_document()
			{
				if (m_outgoing_document)
				{
					m_outgoing_document.reset();
				}
				return operation_async_result::success;
			}

			static inline operation_async_result async_create_new_document()
			{
				try
				{
					m_pending_document = std::make_unique<T_DOCUMENT>();
				}
				catch (...)
				{
					// TODO: handle error/details
					return operation_async_result::fail;
				}
				return operation_async_result::success;
			}

			static inline operation_async_result async_open_pending_document()
			{
				imgui_app::log_info(fmt::format("Opening pending document: {}", m_pending_filename).c_str());
				try
				{
					m_pending_document = std::make_unique<T_DOCUMENT>(m_pending_filename);
				}
				catch (...)
				{
					// TODO: handle error/details
					return operation_async_result::fail;
				}
				return operation_async_result::success;
			}

			static inline operation_async_result async_save_pending_document()
			{
				imgui_app::log_info(fmt::format("Saving pending document: {}", m_pending_filename).c_str());
				m_filename = std::move(m_pending_filename);
				try
				{
					if (!m_document->save(m_filename))
					{
						return operation_async_result::fail;
					}
				}
				catch (...)
				{
					return operation_async_result::fail;
				}
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
				imgui_app::log_info(fmt::format("Pending document is: {}", m_pending_filename).c_str());
			}

			static inline bool needs_filename()
			{
				return m_filename.length() == 0;
			}

			static inline bool needs_to_save()
			{
				return m_document && m_document->needs_to_save();
			}

			virtual void react(typename model_events::update const& evt)
			{
				if (m_pending_document && m_document != m_pending_document)
				{
					m_document = std::move(m_pending_document);
					m_filename = std::move(m_pending_filename);
				}

				if (m_document)
				{
					m_document->react(evt);
				}
			}

			virtual void react(typename model_events::draw_menu const& evt)
			{
				T_DOCUMENT::draw_menu(m_document.get(), document_file_menu::mode::locked);
			}

			virtual void react(typename model_events::draw_content const& evt)
			{
				if (m_document)
				{
					m_document->react(evt);
				}
			}

			virtual void react(typename model_events::quit const& evt)
			{
				// TODO
			}

			virtual void entry() {}

			virtual void exit() {}
		};

		template<typename T_SHUTDOWN, typename T_ON_NEW, typename T_ON_OPEN>
		struct empty_substate
		{
			struct main_state;

			struct report_error : application_model_utils::messagebox_quit<T_SHUTDOWN, fsm>
			{
				virtual void entry()
				{
					impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
				}
			};

			struct init : application_model_utils::async<report_error, main_state, fsm>
			{
				virtual void entry()
				{
					set_impl_call(fsm::async_init_empty);
				}
			};

			struct destroy : application_model_utils::async<report_error, T_SHUTDOWN, fsm>
			{
				virtual void entry()
				{
					set_impl_call(fsm::async_destroy_empty);
				}
			};

			struct main_state : fsm
			{
				virtual void react(typename model_events::draw_menu const& evt)
				{
					switch (T_DOCUMENT::draw_menu(m_document.get(), document_file_menu::mode::empty))
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
				virtual void entry()
				{
					impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
				}
			};

			struct init : fsm
			{
				virtual void entry()
				{
					transit<main_state>();
				}
			};

			struct main_state : application_model_utils::async<report_error, T_SUCCESS, fsm>
			{
				virtual void entry()
				{
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
				virtual void entry()
				{
					impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
				}
			};

			struct init : application_model_utils::file_open_dlg<T_EXIT, main_state, fsm>
			{
				virtual void entry()
				{
					impl_show_dialog(fsm::current_filename(), "");
				}
			};

			struct main_state : application_model_utils::async<report_error, T_SUCCESS, fsm>
			{
				virtual void entry()
				{
					set_impl_call(fsm::async_open_pending_document);
				}
			};
		};

		template<typename T_CLOSE, typename T_EXIT, typename T_NEW, typename T_OPEN>
		struct ready_substate
		{
			struct main_state;

			template<typename T_SUCCESS, typename T_FAIL>
			struct save_substate
			{
				struct main_state;

				struct report_error : application_model_utils::messagebox_quit<T_FAIL, fsm>
				{
					virtual void entry()
					{
						impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
					}
				};

				struct init_needs_filename : application_model_utils::file_save_dlg<T_FAIL, main_state, fsm>
				{
					virtual void entry()
					{
						impl_show_dialog(fsm::current_filename(), "");
					}
				};

				struct init : fsm
				{
					virtual void entry()
					{
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
					virtual void entry()
					{
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
					virtual void entry()
					{
						impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
					}
				};

				struct init : application_model_utils::file_save_dlg<T_FAIL, main_state, fsm>
				{
					virtual void entry()
					{
						impl_show_dialog(fsm::current_filename(), "");
					}
				};

				struct main_state : application_model_utils::async<report_error, T_SUCCESS, fsm>
				{
					virtual void entry()
					{
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
					virtual void entry()
					{
						impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
					}
				};

				struct init_save : application_model_utils::file_save_dlg<T_FAIL, main_state, fsm>
				{
					virtual void entry()
					{
						impl_show_dialog(fsm::current_filename(), "");
					}
				};

				struct cleanup_document : application_model_utils::async<T_SUCCESS /*NOTE: Can't realistically fail & not be fatal*/, T_SUCCESS, fsm>
				{
					virtual void entry()
					{
						fsm::prepare_destroy_document();
						set_impl_call(fsm::async_destroy_document);
					}
				};

				struct init : application_model_utils::messagebox_yes_no<init_save, cleanup_document, fsm>
				{
					virtual void entry()
					{
						if (fsm::needs_to_save())
						{
							impl_show_dialog("You have unsaved changes, would you like to save?", "Document has changes", imgui_app::messagebox_style::warning);
						}
						else
						{
							transit<cleanup_document>();
						}
					}
				};

				struct main_state : application_model_utils::async<report_error, cleanup_document, fsm>
				{
					virtual void entry()
					{
						set_impl_call(fsm::async_save_pending_document);
					}
				};
			};

			struct report_error : application_model_utils::messagebox_quit<T_EXIT, fsm>
			{
				report_error() {}

				virtual void entry()
				{
					impl_show_dialog("Error", "An error happened.", imgui_app::messagebox_style::error);
				}
			};

			struct init : fsm
			{
				virtual void entry()
				{
					transit<main_state>();
				}
			};

			struct main_state : fsm
			{
				virtual void react(typename model_events::draw_menu const& evt)
				{
					switch (T_DOCUMENT::draw_menu(m_document.get(), document_file_menu::mode::active))
					{
					case document_file_menu::result::on_new:
						transit<close_substate<T_NEW, main_state>::init>();
						break;
					case document_file_menu::result::on_open:
						transit<close_substate<T_OPEN, main_state>::init>();
						break;
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
		using ready			  = ready_substate<substate_empty, shutdown, substate_new, substate_open>;

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

		struct bootstrap : application_model_utils::async<shutdown, substate_empty, fsm>
		{
			bootstrap() {}

			virtual void entry()
			{
				set_impl_call(fsm::async_bootstrap);
				m_running = true;
			}
		};

		// terminal
		struct terminated : fsm
		{
			terminated() {}

			virtual void entry()
			{
				m_running = false;
			}
		};

		struct shutdown : application_model_utils::async<terminated, terminated, fsm>
		{
			shutdown() {}

			virtual void entry()
			{
				set_impl_call(fsm::async_shutdown);
			}
		};

		using fsm_handle = tinyfsm::FsmList<fsm>;
	};
};

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
