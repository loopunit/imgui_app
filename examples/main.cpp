#include <imgui_app.h>
#include <taskflow/taskflow.hpp>
#include <optick.h>
#include <leveldb/db.h>

// Main code
int main(int, char**)
{
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "test.db", &db);
	delete db;

	if (imgui_app::select_platform(imgui_app_fw::platform::win32_dx12))
	{
		OPTICK_APP("ConsoleApp");

		OPTICK_SET_MEMORY_ALLOCATOR(
			[](size_t size) -> void* { return operator new(size); }, 
			[](void* p) { operator delete(p); }, 
			[]() { /* Do some TLS initialization here if needed */ }
		);

		if (imgui_app::init())
		{
			imgui_app::info("Welcome to the imgui-console example!");
			imgui_app::info("The following variables have been exposed to the console:");
			imgui_app::info("\tbackground_color - set: [int int int int]");
			imgui_app::info("Try running the following command:");
			imgui_app::error("\tset background_color [255 0 0 255]");

			bool   show_demo_window = true;
			ImVec4 clear_color		= ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			tf::Executor executor;
			tf::Taskflow taskflow;

			auto [A, B, C, D] = taskflow.emplace(
			[] () { imgui_app::info("TaskA"); },               //  task dependency graph
			[] () { imgui_app::info("TaskB"); },               // 
			[] () { imgui_app::info("TaskC"); },               //          +---+          
			[] () { imgui_app::info("TaskD"); }                //    +---->| B |-----+   
			);                                                 //    |     +---+     |
																//  +---+           +-v-+ 
			A.precede(B);  // A runs before B                  //  | A |           | D | 
			A.precede(C);  // A runs before C                  //  +---+           +-^-+ 
			B.precede(D);  // B runs before D                  //    |     +---+     |    
			C.precede(D);  // C runs before D                  //    +---->| C |-----+    
																//          +---+          
			executor.run(taskflow).wait();
			
			while (imgui_app::pump())
			{
				OPTICK_FRAME("MainThread");
				imgui_app::begin_frame();

				if (show_demo_window)
				{
					ImGui::ShowDemoWindow(&show_demo_window);
				}

				imgui_app::end_frame(clear_color);
			}

			imgui_app::destroy();
		}
		OPTICK_SHUTDOWN();
	}

	return 0;
}
