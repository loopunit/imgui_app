#include "imgui_app_logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/msvc_sink.h>

namespace imgui_app
{
	std::shared_ptr<spdlog::logger> create_console_logger()
	{
		return std::make_shared<spdlog::logger>("console", std::make_shared<spdlog::sinks::msvc_sink_mt>());
	}
}