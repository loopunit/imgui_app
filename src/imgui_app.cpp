#include "imgui_app_internal.h"

#include <imgui_console.h>

#include <reckless/severity_log.hpp>
#include <reckless/stdout_writer.hpp>
#include <reckless/crash_handler.hpp>

#include <atomic_queue/atomic_queue.h>

#include <nfd.h>

namespace imgui_app
{
	namespace details
	{
		// indirect call required to copy the string views
		static std::optional<std::string> async_file_open_dialog(std::string filter, std::string loc)
		{
			std::string result;
			char*		nfd_path = nullptr;

			auto nfd_result = NFD_OpenDialog(filter.data(), loc.data(), &nfd_path);
			if (nfd_result == NFD_OKAY)
			{
				result = nfd_path;
				::free(nfd_path);
				return result;
			}
			else if (nfd_result == NFD_CANCEL)
			{
				// nothing
			}
			else
			{
				// printf("Error: %s\n", NFD_GetError());
			}
			return std::nullopt;
		}

		static std::optional<std::vector<std::string>> async_file_open_multiple_dialog(std::string filter, std::string loc)
		{
			std::vector<std::string> results;
			nfdpathset_t			 path_set;

			auto nfd_result = NFD_OpenDialogMultiple(filter.data(), loc.data(), &path_set);

			if (nfd_result == NFD_OKAY)
			{
				const auto num_paths = NFD_PathSet_GetCount(&path_set);
				results.resize(num_paths);
				for (auto i = num_paths - 1; i >= 0; --i)
				{
					results[i] = NFD_PathSet_GetPath(&path_set, i);
				}

				NFD_PathSet_Free(&path_set);

				return results;
			}
			else if (nfd_result == NFD_CANCEL)
			{
				// nothing
			}
			else
			{
				//::imgui_app::error("%s", NFD_GetError());
				// printf("Error: );
			}
			return std::nullopt;
		}

		static std::optional<std::string> async_file_save_dialog(std::string filter, std::string loc)
		{
			std::string result;
			nfdchar_t*	nfd_path = nullptr;

			auto nfd_result = NFD_SaveDialog(filter.data(), loc.data(), &nfd_path);

			if (nfd_result == NFD_OKAY)
			{
				result = nfd_path;
				::free(nfd_path);
				return result;
			}
			else if (nfd_result == NFD_CANCEL)
			{
				// nothing
			}
			else
			{
				// printf("Error: %s\n", NFD_GetError());
			}
			return std::nullopt;
		}

		static std::optional<std::string> async_show_choose_path_dialog(std::string loc)
		{
			std::string result;
			nfdchar_t*	nfd_path = nullptr;

			auto nfd_result = NFD_OpenDirectoryDialog(nullptr, loc.data(), &nfd_path);

			if (nfd_result == NFD_OKAY)
			{
				result = nfd_path;
				::free(nfd_path);
				return result;
			}
			else if (nfd_result == NFD_CANCEL)
			{
				// nothing
			}
			else
			{
				// printf("Error: %s\n", NFD_GetError());
			}
			return std::nullopt;
		}

		optional_future<std::string> show_file_open_dialog(std::string_view origin, std::string_view filter) noexcept
		{
			return std::async(std::launch::async, async_file_open_dialog, std::string(filter), std::string(origin));
		}

		optional_future<std::vector<std::string>> show_file_open_multiple_dialog(std::string_view origin, std::string_view filter) noexcept
		{
			return std::async(std::launch::async, async_file_open_multiple_dialog, std::string(filter), std::string(origin));
		}

		optional_future<std::string> show_file_save_dialog(std::string_view origin, std::string_view filter) noexcept
		{
			return std::async(std::launch::async, async_file_save_dialog, std::string(filter), std::string(origin));
		}

		optional_future<std::string> show_path_dialog(std::string_view origin, std::string_view filter) noexcept
		{
			return std::async(std::launch::async, async_show_choose_path_dialog, std::string(origin));
		}
	} // namespace details
} // namespace imgui_app

namespace imgui_app
{
	namespace details
	{
		class ImGuiConsole_custom : public ImGuiConsole
		{
		public:
			void Draw()
			{
				// Begin Console Window.
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_WindowAlpha);
				if (!ImGui::Begin(m_ConsoleName.data(), nullptr, ImGuiWindowFlags_MenuBar))
				{
					ImGui::PopStyleVar();
					ImGui::End();
					return;
				}
				ImGui::PopStyleVar();

				MenuBar();

				if (m_FilterBar)
				{
					FilterBar();
				}

				LogWindow();

				ImGui::Separator();

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

		struct logger
		{
			struct pending_message
			{
				std::string	   message;
				csys::ItemType type;
			};

			using message_queue = atomic_queue::AtomicQueueB<pending_message*>;
			message_queue					  _message_queue_free;
			message_queue					  _message_queue_pending;
			std::array<pending_message, 1024> _message_pool;
			ImGuiConsole_custom				  _console;

			reckless::stderr_writer _default_writer;
			using logger_type = reckless::policy_log<>;
			logger_type _default_logger;

			static inline constexpr int			   _num_loggers = underlying_cast(csys::ItemType::NONE);
			std::array<logger_type*, _num_loggers> _writers{nullptr};

			logger() : _message_queue_free{1024}, _message_queue_pending{1024}, _default_logger(&_default_writer)
			{
				for (auto& msg : _message_pool)
				{
					_message_queue_free.push(&msg);
				}

				for (auto& w : _writers)
				{
					w = &_default_logger;
				}
			}

			~logger() {}

			void log_impl(std::string_view text, csys::ItemType type) noexcept
			{
				pending_message* msg;
				if (_message_queue_free.try_pop(msg))
				{
					msg->type	 = type;
					msg->message = text;
					_message_queue_pending.push(msg);
				}

				auto& writer = _writers[underlying_cast(type)];
				if (writer)
				{
					std::error_code ec;
					writer->write(text.data());
				}
			}

			void log(const char* text) noexcept
			{
				log_impl(text, csys::ItemType::LOG);
			}

			void warning(const char* text) noexcept
			{
				log_impl(text, csys::ItemType::WARNING);
			}

			void error(const char* text) noexcept
			{
				log_impl(text, csys::ItemType::ERROR);
			}

			void info(const char* text) noexcept
			{
				log_impl(text, csys::ItemType::INFO);
			}

			void draw() noexcept
			{
				pending_message* msg;
				while (_message_queue_pending.try_pop(msg))
				{
					_console.System().Log(msg->type) << std::string_view(msg->message);
					_message_queue_free.push(msg);
				}
				_console.Draw();
			}

			void init()
			{
				if (_writers[underlying_cast(csys::ItemType::ERROR)])
				{
					reckless::install_crash_handler(_writers[underlying_cast(csys::ItemType::ERROR)]);
				}
			}

			void destroy() noexcept
			{
				reckless::uninstall_crash_handler();
			}
		};
	} // namespace details
} // namespace imgui_app

namespace imgui_app
{
	using logger = singleton<details::logger>;

	bool select_platform(imgui_app_fw::platform p)
	{
		return imgui_app_fw::select_platform(p);
	}

	void set_window_title(const char* title)
	{
		return imgui_app_fw::set_window_title(title);
	}

	bool init()
	{
		if (imgui_app_fw::init())
		{
			logger()->init();
			return true;
		}
		return false;
	}

	bool pump()
	{
		return imgui_app_fw::pump();
	}

	void begin_frame()
	{
		imgui_app_fw::begin_frame();
	}

	void end_frame(ImVec4 clear_color)
	{
		logger()->draw();
		imgui_app_fw::end_frame(clear_color);
	}

	void destroy()
	{
		logger()->destroy();
		imgui_app_fw::destroy();
	}

	void log(const char* text) noexcept
	{
		logger()->log(text);
	}

	void warning(const char* text) noexcept
	{
		logger()->warning(text);
	}

	void error(const char* text) noexcept
	{
		logger()->error(text);
	}

	void info(const char* text) noexcept
	{
		logger()->info(text);
	}

} // namespace imgui_app

// imgui_app_DEFINE_VIRTUAL_SINGLETON(imgui_app::details::logger, imgui_app::logger_impl);