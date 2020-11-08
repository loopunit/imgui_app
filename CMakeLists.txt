cmake_minimum_required( VERSION 3.12 )
project(imgui_app
	VERSION 1.7.8)

include(CMakeDependentOption)

option(IMGUI_BUILD_EXAMPLES "Build examples." OFF)
cmake_dependent_option(IMGUI_BUILD_APP_FW "Build app framework." OFF "IMGUI_BUILD_EXAMPLES" ON)
option(IMGUI_BUILD_APP_WIN32_DX11 "Win32 DX11" ON)
option(IMGUI_BUILD_APP_WIN32_DX12 "Win32 DX12" OFF)
option(IMGUI_BUILD_APP_GLFW_VULKAN "GLFW Vulkan" OFF)

option(USING_IMGUI_ADDONS "Use IMGui addons" ON)
option(USING_STX "Use STX" OFF)
option(USING_ATOMIC_QUEUE "Use atomic_queue" ON)
option(USING_SPDLOG "Use spdlog" ON)
option(USING_NLOHMANN_JSON "Use nlohmann_json" OFF)
option(USING_CEREAL "Use cereal" OFF)
option(USING_ENTT "Use entt" OFF)
option(USING_CWALK "Use cwalk" OFF)
option(USING_FMT "Use fmt" ON)
option(USING_TASKFLOW "Use Taskflow" OFF)
option(USING_GSL "Use gsl" OFF)
option(USING_UNITS "Use units" OFF)
option(USING_ARGH "Use argh" OFF)
option(USING_SCOPE_GUARD "Use scope_guard" OFF)
option(USING_HEDLEY "Use hedley" OFF)
option(USING_RTM "Use rtm" OFF)
option(USING_TINYFSM "Use tinyfsm" ON)
option(USING_NFD "Use NFD" ON)
option(USING_BOXER "Use Boxer" ON) 
option(USING_PLF_NANOTIMER "Use plf_nanotimer" OFF)
option(USING_OPTICK "Use optick" OFF)
option(USING_MUSTACH "Use mustach" OFF) 
option(USING_LIBUV "Use libuv" OFF)
option(USING_LEVELDB "Use leveldb" OFF)


# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(cmake/CPM.cmake)

#############

CPMAddPackage(
  NAME PackageProject.cmake
  GITHUB_REPOSITORY TheLartians/PackageProject.cmake
  VERSION 1.3)

#############

if(USING_IMGUI_ADDONS)
	CPMAddPackage(
		NAME imgui_addons_cpm
		GITHUB_REPOSITORY loopunit/imgui_addons.cpm
		GIT_TAG fb70b1d90c80c158f2bbd3d40c8115a15dbf8a1f 
		OPTIONS
			"IMGUI_BUILD_EXAMPLES ${IMGUI_BUILD_EXAMPLES}"
			"IMGUI_BUILD_APP_FW ${IMGUI_BUILD_APP_FW}"
			"IMGUI_BUILD_APP_WIN32_DX11 ${IMGUI_BUILD_APP_WIN32_DX11}"
			"IMGUI_BUILD_APP_WIN32_DX12 ${IMGUI_BUILD_APP_WIN32_DX12}"
			"IMGUI_BUILD_APP_GLFW_VULKAN ${IMGUI_BUILD_APP_GLFW_VULKAN}")

	if( NOT ${imgui_addons_ADDED} )
		message( FATAL_ERROR "Could not find imgui_addons.cpm" )
	endif()
endif()

#############

if(USING_STX)
	CPMAddPackage(
		NAME stx
		GITHUB_REPOSITORY lamarrr/STX
		GIT_TAG a984c0fe9610548e880e775e279403bd8941d2eb
		DOWNLOAD_ONLY true)

	if( NOT ${stx_ADDED} )
		message( FATAL_ERROR "Could not find stx" )
	endif()

	file(GLOB stx_source 
		${stx_SOURCE_DIR}/src/panic/hook.cc 
		${stx_SOURCE_DIR}/src/panic.cc)

	install(
		DIRECTORY ${stx_SOURCE_DIR}/include/stx
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if(USING_ATOMIC_QUEUE)
	CPMAddPackage(
	NAME atomic_queue
		GITHUB_REPOSITORY max0x7ba/atomic_queue
		GIT_TAG df794032353a9fbf6204481bdd53ef0263ce51f3 
		DOWNLOAD_ONLY true)

	if( NOT ${atomic_queue_ADDED} )
		message( FATAL_ERROR "Could not find atomic_queue" )
	endif()

	install(
		DIRECTORY ${atomic_queue_SOURCE_DIR}/include/atomic_queue
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if(USING_SPDLOG)
	CPMAddPackage(
		NAME spdlog
		GITHUB_REPOSITORY gabime/spdlog
		GIT_TAG 18e3f07f7d333d33c0ef8fbea02299dbe5d49f97 
		DOWNLOAD_ONLY true)

	if( NOT ${spdlog_ADDED} )
		message( FATAL_ERROR "Could not find spdlog" )
	endif()

	install(
		DIRECTORY ${spdlog_SOURCE_DIR}/include/spdlog
		DESTINATION include/imgui_app-${PROJECT_VERSION})

	file(GLOB spdlog_source 
		${spdlog_SOURCE_DIR}/src/*.cpp)
endif()

#############

if(USING_NLOHMANN_JSON)
	CPMAddPackage(
		NAME nlohmann_json
		GITHUB_REPOSITORY nlohmann/json
		GIT_TAG fd7a9f600712b2724463e9f7f703878ade676d6e
		DOWNLOAD_ONLY true)

	if( NOT ${nlohmann_json_ADDED} )
		message( FATAL_ERROR "Could not find nlohmann_json" )
	endif()

	install(
		DIRECTORY ${nlohmann_json_SOURCE_DIR}/include/nlohmann
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if(USING_CEREAL)
	CPMAddPackage(
		NAME cereal
		GITHUB_REPOSITORY USCiLab/cereal
		GIT_TAG 3e4d1b84cab4891368d2179a61a7ba06a5693e7f
		DOWNLOAD_ONLY true)

	if( NOT ${cereal_ADDED} )
		message( FATAL_ERROR "Could not find cereal" )
	endif()

	install(
		DIRECTORY ${cereal_SOURCE_DIR}/include/cereal
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if(USING_ENTT)
	CPMAddPackage(
		NAME entt
		GITHUB_REPOSITORY skypjack/entt
		GIT_TAG cf395f707a1db97fb9a2a5e49a6d0114ec8fedec
		DOWNLOAD_ONLY true)

	if( NOT ${entt_ADDED} )
		message( FATAL_ERROR "Could not find entt" )
	endif()

	install(
		DIRECTORY ${entt_SOURCE_DIR}/src/entt
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if(USING_CWALK)
	CPMAddPackage(
		NAME cwalk
		GITHUB_REPOSITORY likle/cwalk
		GIT_TAG 30da9d275b0ca4e9ef5307521ee452d8b85015e4
		DOWNLOAD_ONLY true)

	if( NOT ${cwalk_ADDED} )
		message( FATAL_ERROR "Could not find cwalk" )
	endif()

	install(
		FILES ${cwalk_SOURCE_DIR}/include/cwalk.h
		DESTINATION include/imgui_app-${PROJECT_VERSION})

	file(GLOB cwalk_source 
		${cwalk_SOURCE_DIR}/src/cwalk.c)
endif()

#############

if(USING_FMT)
	CPMAddPackage(
		NAME fmt
		GITHUB_REPOSITORY fmtlib/fmt
		GIT_TAG af283059612dd06c321dc509255ae9b003ca5e69
		DOWNLOAD_ONLY true)

	if( NOT ${fmt_ADDED} )
		message( FATAL_ERROR "Could not find fmt" )
	endif()

	install(
		DIRECTORY ${fmt_SOURCE_DIR}/include/fmt
		DESTINATION include/imgui_app-${PROJECT_VERSION})

	file(GLOB fmt_source 
		${fmt_SOURCE_DIR}/src/*.cc)
endif()

#############

if (USING_TASKFLOW)
	CPMAddPackage(
		NAME taskflow
		GITHUB_REPOSITORY taskflow/taskflow
		GIT_TAG c75c05f3d549534e8bd05d6e9afdac82829ea786
		DOWNLOAD_ONLY true)

	if( NOT ${taskflow_ADDED} )
		message( FATAL_ERROR "Could not find taskflow" )
	endif()

	install(
		DIRECTORY ${taskflow_SOURCE_DIR}/taskflow
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if (USING_GSL)
	CPMAddPackage(
		NAME gsl
		GITHUB_REPOSITORY microsoft/GSL
		GIT_TAG bd23bdc0adae7028365cbbe934f88cbf436da7ee
		DOWNLOAD_ONLY true)

	if( NOT ${gsl_ADDED} )
		message( FATAL_ERROR "Could not find gsl" )
	endif()

	install(
		DIRECTORY ${gsl_SOURCE_DIR}/include/gsl
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if (USING_UNITS)
	CPMAddPackage(
		NAME units
		GITHUB_REPOSITORY nholthaus/units
		GIT_TAG ea6d126942cb3225a341568ab57ec52513977875
		DOWNLOAD_ONLY true)

	if( NOT ${units_ADDED} )
		message( FATAL_ERROR "Could not find units" )
	endif()

	install(
		FILES ${units_SOURCE_DIR}/include/units.h
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if (USING_ARGH)
	CPMAddPackage(
		NAME argh
		GITHUB_REPOSITORY adishavit/argh
		GIT_TAG d9964d4fae6ce6dec9d52fa5c86d648fff4d4026
		DOWNLOAD_ONLY true)

	if( NOT ${argh_ADDED} )
		message( FATAL_ERROR "Could not find argh" )
	endif()

	install(
		FILES ${argh_SOURCE_DIR}/argh.h
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if (USING_SCOPE_GUARD)
	CPMAddPackage(
		NAME scope_guard
		GITHUB_REPOSITORY ricab/scope_guard
		GIT_TAG 760de0a9ea0fec980a5244604ffa5b1acff2db7a
		DOWNLOAD_ONLY true)

	if( NOT ${scope_guard_ADDED} )
		message( FATAL_ERROR "Could not find scope_guard" )
	endif()

	install(
		FILES ${scope_guard_SOURCE_DIR}/scope_guard.hpp
	    DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if (USING_HEDLEY)
	CPMAddPackage(
		NAME hedley
		GITHUB_REPOSITORY nemequ/hedley
		GIT_TAG 4a59eb6d8e3e73b6c60aed2c92a6590d26df93d8
		DOWNLOAD_ONLY true)

	if( NOT ${hedley_ADDED} )
		message( FATAL_ERROR "Could not find hedley" )
	endif()

	install(
		FILES ${hedley_SOURCE_DIR}/hedley.h
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if (USING_RTM)
	CPMAddPackage(
		NAME rtm
		GITHUB_REPOSITORY nfrechette/rtm
		GIT_TAG 60e61b4fa37b01817e931164a30b21150b260354
		DOWNLOAD_ONLY true)

	if( NOT ${rtm_ADDED} )
		message( FATAL_ERROR "Could not find rtm" )
	endif()

	install(
		DIRECTORY ${rtm_SOURCE_DIR}/includes/rtm
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if(USING_TINYFSM)
	CPMAddPackage(
		NAME tinyfsm
		GITHUB_REPOSITORY digint/tinyfsm
		GIT_TAG 06f27072266d6c34d990f910c4c43fb11240501c
		DOWNLOAD_ONLY true)

	if( NOT ${tinyfsm_ADDED} )
		message( FATAL_ERROR "Could not find tinyfsm" )
	endif()

	install(
		FILES ${tinyfsm_SOURCE_DIR}/include/tinyfsm.hpp
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if(USING_NFD)
	CPMAddPackage(
		NAME nfd
		GITHUB_REPOSITORY native-toolkit/nfd
		GIT_TAG e9e3636b031797396aa502b78432ae1347c2afe3
		DOWNLOAD_ONLY true)

	if( NOT ${nfd_ADDED} )
		message( FATAL_ERROR "Could not find nfd" )
	endif()

	install(
		FILES ${nfd_SOURCE_DIR}/src/include/nfd.h
		DESTINATION include/imgui_app-${PROJECT_VERSION})

	file(GLOB nfd_source 
		${nfd_SOURCE_DIR}/src/nfd_win.cpp
		${nfd_SOURCE_DIR}/src/common.h
		${nfd_SOURCE_DIR}/src/nfd_common.h
		${nfd_SOURCE_DIR}/src/nfd_common.c)
endif()

#############

if(USING_BOXER)
	CPMAddPackage(
		NAME boxer
		GITHUB_REPOSITORY aaronmjacobs/Boxer
		GIT_TAG 01c17f6a9cd66068f7890ea887ab3b9a673f0434
		DOWNLOAD_ONLY true)

	if( NOT ${boxer_ADDED} )
		message( FATAL_ERROR "Could not find boxer" )
	endif()

	install(
		DIRECTORY ${boxer_SOURCE_DIR}/src/include/boxer
		DESTINATION include/imgui_app-${PROJECT_VERSION})

	if(APPLE)
		file(GLOB boxer_source 
			${boxer_SOURCE_DIR}/src/boxer_osx.mm)
	elseif (WIN32)
		file(GLOB boxer_source 
			${boxer_SOURCE_DIR}/src/boxer_win.cpp)
	elseif (LINUX)
		file(GLOB boxer_source 
			${boxer_SOURCE_DIR}/src/boxer_linux.cpp)
	endif()
endif()

#############

if (USING_PLF_NANOTIMER)
	CPMAddPackage(
		NAME plf_nanotimer
		GITHUB_REPOSITORY mattreecebentley/plf_nanotimer
		GIT_TAG bc8b44d2befc4395f63701c923ece89835d8415c
		DOWNLOAD_ONLY true)

	if( NOT ${plf_nanotimer_ADDED} )
		message( FATAL_ERROR "Could not find plf_nanotimer" )
	endif()

	install(
		FILES ${plf_nanotimer_SOURCE_DIR}/plf_nanotimer.h
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if (USING_OPTICK)
	CPMAddPackage(
		NAME optick
		GITHUB_REPOSITORY bombomby/optick
		GIT_TAG 7b940feedebd0dcd8ee6448f7997044a1e535693
		DOWNLOAD_ONLY true)

	if( NOT ${optick_ADDED} )
		message( FATAL_ERROR "Could not find optick" )
	endif()

	file(GLOB optick_source ${optick_SOURCE_DIR}/src/*.*)

	install(
		FILES ${optick_SOURCE_DIR}/src/optick.h ${optick_SOURCE_DIR}/src/optick.config.h
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if (USING_MUSTACH)
	CPMAddPackage(
		NAME mustach
		GIT_REPOSITORY https://gitlab.com/jobol/mustach.git
		GIT_TAG dc2a8a4f331258be8526044edae6ab95bf803353
		DOWNLOAD_ONLY true)

	if( NOT ${mustach_ADDED} )
		message( FATAL_ERROR "Could not find mustach" )
	endif()

	file(GLOB mustach_source ${mustach_SOURCE_DIR}/mustach.c)

	install(
		FILES ${mustach_SOURCE_DIR}/mustach.h
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if (USING_LIBUV)
	CPMAddPackage(
		NAME libuv
		GITHUB_REPOSITORY libuv/libuv
		GIT_TAG 30ff5bf2161257921f3a3ce5655804f7cb282aa9
		DOWNLOAD_ONLY true)

	if( NOT ${libuv_ADDED} )
		message( FATAL_ERROR "Could not find libuv" )
	endif()

	file(GLOB uv_source
		${libuv_SOURCE_DIR}/src/fs-poll.c
		${libuv_SOURCE_DIR}/src/idna.c
		${libuv_SOURCE_DIR}/src/inet.c
		${libuv_SOURCE_DIR}/src/random.c
		${libuv_SOURCE_DIR}/src/strscpy.c
		${libuv_SOURCE_DIR}/src/threadpool.c
		${libuv_SOURCE_DIR}/src/timer.c
		${libuv_SOURCE_DIR}/src/uv-common.c
		${libuv_SOURCE_DIR}/src/uv-data-getter-setters.c
		${libuv_SOURCE_DIR}/src/version.c)

	if(WIN32)
		#list(APPEND uv_platform_defines WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0600)
	
		list(APPEND uv_platform_libraries
			psapi
			user32
			advapi32
			iphlpapi
			userenv
			ws2_32)

		file(GLOB uv_platform_source
			${libuv_SOURCE_DIR}/src/win/async.c
			${libuv_SOURCE_DIR}/src/win/core.c
			${libuv_SOURCE_DIR}/src/win/detect-wakeup.c
			${libuv_SOURCE_DIR}/src/win/dl.c
			${libuv_SOURCE_DIR}/src/win/error.c
			${libuv_SOURCE_DIR}/src/win/fs.c
			${libuv_SOURCE_DIR}/src/win/fs-event.c
			${libuv_SOURCE_DIR}/src/win/getaddrinfo.c
			${libuv_SOURCE_DIR}/src/win/getnameinfo.c
			${libuv_SOURCE_DIR}/src/win/handle.c
			${libuv_SOURCE_DIR}/src/win/loop-watcher.c
			${libuv_SOURCE_DIR}/src/win/pipe.c
			${libuv_SOURCE_DIR}/src/win/thread.c
			${libuv_SOURCE_DIR}/src/win/poll.c
			${libuv_SOURCE_DIR}/src/win/process.c
			${libuv_SOURCE_DIR}/src/win/process-stdio.c
			${libuv_SOURCE_DIR}/src/win/signal.c
			${libuv_SOURCE_DIR}/src/win/snprintf.c
			${libuv_SOURCE_DIR}/src/win/stream.c
			${libuv_SOURCE_DIR}/src/win/tcp.c
			${libuv_SOURCE_DIR}/src/win/tty.c
			${libuv_SOURCE_DIR}/src/win/udp.c
			${libuv_SOURCE_DIR}/src/win/util.c
			${libuv_SOURCE_DIR}/src/win/winapi.c
			${libuv_SOURCE_DIR}/src/win/winsock.c)
	else()
		#list(APPEND uv_platform_defines _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE)

		file(GLOB uv_platform_source
			   ${libuv_SOURCE_DIR}/src/unix/async.c
			   ${libuv_SOURCE_DIR}/src/unix/core.c
			   ${libuv_SOURCE_DIR}/src/unix/dl.c
			   ${libuv_SOURCE_DIR}/src/unix/fs.c
			   ${libuv_SOURCE_DIR}/src/unix/getaddrinfo.c
			   ${libuv_SOURCE_DIR}/src/unix/getnameinfo.c
			   ${libuv_SOURCE_DIR}/src/unix/loop-watcher.c
			   ${libuv_SOURCE_DIR}/src/unix/loop.c
			   ${libuv_SOURCE_DIR}/src/unix/pipe.c
			   ${libuv_SOURCE_DIR}/src/unix/poll.c
			   ${libuv_SOURCE_DIR}/src/unix/process.c
			   ${libuv_SOURCE_DIR}/src/unix/random-devurandom.c
			   ${libuv_SOURCE_DIR}/src/unix/signal.c
			   ${libuv_SOURCE_DIR}/src/unix/stream.c
			   ${libuv_SOURCE_DIR}/src/unix/tcp.c
			   ${libuv_SOURCE_DIR}/src/unix/thread.c
			   ${libuv_SOURCE_DIR}/src/unix/tty.c
			   ${libuv_SOURCE_DIR}/src/unix/udp.c)	   
	endif()

	install(
		FILES ${libuv_SOURCE_DIR}/include/uv.h
		DESTINATION include/imgui_app-${PROJECT_VERSION})

	install(
		DIRECTORY ${libuv_SOURCE_DIR}/include/uv
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

if(USING_LEVELDB)
	CPMAddPackage(
		NAME leveldb
		GITHUB_REPOSITORY google/leveldb
		GIT_TAG b7d302326961fb809d92a95ce813e2d26fe2e16e
		DOWNLOAD_ONLY true)

	if( NOT ${leveldb_ADDED} )
		message( FATAL_ERROR "Could not find leveldb" )
	endif()

	set(LEVELDB_PUBLIC_INCLUDE_DIR "include/leveldb")
	set(LEVELDB_PORT_CONFIG_DIR "include/port")

	set(HAVE_FDATASYNC OFF)
	set(HAVE_FULLFSYNC OFF)
	set(HAVE_O_CLOEXEC OFF)
	set(HAVE_CRC32C OFF)
	set(HAVE_SNAPPY OFF)

	configure_file(
	  "${leveldb_SOURCE_DIR}/port/port_config.h.in"
	  "${PROJECT_BINARY_DIR}/include/port/port_config.h"
	)

	if (WIN32)
	  set(LEVELDB_PLATFORM_NAME LEVELDB_PLATFORM_WINDOWS)
	else()
	  set(LEVELDB_PLATFORM_NAME LEVELDB_PLATFORM_POSIX)
	endif()

	file(GLOB leveldb_source
		${leveldb_SOURCE_DIR}/db/builder.cc
		${leveldb_SOURCE_DIR}/db/builder.h
		${leveldb_SOURCE_DIR}/db/c.cc
		${leveldb_SOURCE_DIR}/db/db_impl.cc
		${leveldb_SOURCE_DIR}/db/db_impl.h
		${leveldb_SOURCE_DIR}/db/db_iter.cc
		${leveldb_SOURCE_DIR}/db/db_iter.h
		${leveldb_SOURCE_DIR}/db/dbformat.cc
		${leveldb_SOURCE_DIR}/db/dbformat.h
		${leveldb_SOURCE_DIR}/db/dumpfile.cc
		${leveldb_SOURCE_DIR}/db/filename.cc
		${leveldb_SOURCE_DIR}/db/filename.h
		${leveldb_SOURCE_DIR}/db/log_format.h
		${leveldb_SOURCE_DIR}/db/log_reader.cc
		${leveldb_SOURCE_DIR}/db/log_reader.h
		${leveldb_SOURCE_DIR}/db/log_writer.cc
		${leveldb_SOURCE_DIR}/db/log_writer.h
		${leveldb_SOURCE_DIR}/db/memtable.cc
		${leveldb_SOURCE_DIR}/db/memtable.h
		${leveldb_SOURCE_DIR}/db/repair.cc
		${leveldb_SOURCE_DIR}/db/skiplist.h
		${leveldb_SOURCE_DIR}/db/snapshot.h
		${leveldb_SOURCE_DIR}/db/table_cache.cc
		${leveldb_SOURCE_DIR}/db/table_cache.h
		${leveldb_SOURCE_DIR}/db/version_edit.cc
		${leveldb_SOURCE_DIR}/db/version_edit.h
		${leveldb_SOURCE_DIR}/db/version_set.cc
		${leveldb_SOURCE_DIR}/db/version_set.h
		${leveldb_SOURCE_DIR}/db/write_batch_internal.h
		${leveldb_SOURCE_DIR}/db/write_batch.cc
		${leveldb_SOURCE_DIR}/port/port_stdcxx.h
		${leveldb_SOURCE_DIR}/port/port.h
		${leveldb_SOURCE_DIR}/port/thread_annotations.h
		${leveldb_SOURCE_DIR}/table/block_builder.cc
		${leveldb_SOURCE_DIR}/table/block_builder.h
		${leveldb_SOURCE_DIR}/table/block.cc
		${leveldb_SOURCE_DIR}/table/block.h
		${leveldb_SOURCE_DIR}/table/filter_block.cc
		${leveldb_SOURCE_DIR}/table/filter_block.h
		${leveldb_SOURCE_DIR}/table/format.cc
		${leveldb_SOURCE_DIR}/table/format.h
		${leveldb_SOURCE_DIR}/table/iterator_wrapper.h
		${leveldb_SOURCE_DIR}/table/iterator.cc
		${leveldb_SOURCE_DIR}/table/merger.cc
		${leveldb_SOURCE_DIR}/table/merger.h
		${leveldb_SOURCE_DIR}/table/table_builder.cc
		${leveldb_SOURCE_DIR}/table/table.cc
		${leveldb_SOURCE_DIR}/table/two_level_iterator.cc
		${leveldb_SOURCE_DIR}/table/two_level_iterator.h
		${leveldb_SOURCE_DIR}/util/arena.cc
		${leveldb_SOURCE_DIR}/util/arena.h
		${leveldb_SOURCE_DIR}/util/bloom.cc
		${leveldb_SOURCE_DIR}/util/cache.cc
		${leveldb_SOURCE_DIR}/util/coding.cc
		${leveldb_SOURCE_DIR}/util/coding.h
		${leveldb_SOURCE_DIR}/util/comparator.cc
		${leveldb_SOURCE_DIR}/util/crc32c.cc
		${leveldb_SOURCE_DIR}/util/crc32c.h
		${leveldb_SOURCE_DIR}/util/env.cc
		${leveldb_SOURCE_DIR}/util/filter_policy.cc
		${leveldb_SOURCE_DIR}/util/hash.cc
		${leveldb_SOURCE_DIR}/util/hash.h
		${leveldb_SOURCE_DIR}/util/logging.cc
		${leveldb_SOURCE_DIR}/util/logging.h
		${leveldb_SOURCE_DIR}/util/mutexlock.h
		${leveldb_SOURCE_DIR}/util/no_destructor.h
		${leveldb_SOURCE_DIR}/util/options.cc
		${leveldb_SOURCE_DIR}/util/random.h
		${leveldb_SOURCE_DIR}/util/status.cc
		${leveldb_SOURCE_DIR}/helpers/memenv/memenv.cc
		${leveldb_SOURCE_DIR}/helpers/memenv/memenv.h
		)

	if(WIN32)
		file(GLOB leveldb_platform_source
			${leveldb_SOURCE_DIR}/util/env_windows.cc
			${leveldb_SOURCE_DIR}/util/windows_logger.h)
	else()
		file(GLOB leveldb_platform_source
		  ${leveldb_SOURCE_DIR}/util/env_posix.cc
		  ${leveldb_SOURCE_DIR}/util/posix_logger.h)
	endif()

	install(
		DIRECTORY ${leveldb_SOURCE_DIR}/include/leveldb
		DESTINATION include/imgui_app-${PROJECT_VERSION})
endif()

#############

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.*)

list(APPEND sources ${spdlog_source} ${cwalk_source} ${fmt_source} ${stx_source} ${optick_source} ${mustach_source} ${uv_source} ${uv_platform_source} ${leveldb_source} ${leveldb_platform_source} ${nfd_source} ${boxer_source})

add_library(imgui_app STATIC ${sources})

target_link_libraries(imgui_app PUBLIC imgui_app_fw imgui_addons)

if(USING_LEVELDB)
	if(WIN32)
		target_link_libraries(imgui_app PUBLIC Synchronization ${uv_platform_libraries})
	endif()
endif()

set(private_include_paths
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

set(public_include_paths $<INSTALL_INTERFACE:imgui_app-${PROJECT_VERSION}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
	
if(USING_STX)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${stx_SOURCE_DIR}/include>)
endif()

if(USING_ATOMIC_QUEUE)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${atomic_queue_SOURCE_DIR}/include>)
endif()

if(USING_SPDLOG)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${spdlog_SOURCE_DIR}/include>)
endif()

if(USING_NLOHMANN_JSON)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${nlohmann_json_SOURCE_DIR}/include>)
endif()

if(USING_CEREAL)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${cereal_SOURCE_DIR}/include>)
endif()

if(USING_ENTT)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${entt_SOURCE_DIR}/src>)
endif()

if(USING_CWALK)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${cwalk_SOURCE_DIR}/include>)
endif()

if(USING_FMT)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${fmt_SOURCE_DIR}/include> $<BUILD_INTERFACE:${nfd_SOURCE_DIR}/src/include>)
endif()

if(USING_TASKFLOW)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${taskflow_SOURCE_DIR}>)
endif()

if(USING_GSL)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${gsl_SOURCE_DIR}/include>)
endif()
	
if(USING_UNITS)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${units_SOURCE_DIR}/include>)
endif()

if(USING_ARGH)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${argh_SOURCE_DIR}>)
endif()

if(USING_SCOPE_GUARD)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${scope_guard_SOURCE_DIR}>)
endif()

if(USING_HEDLEY)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${hedley_SOURCE_DIR}>)
endif()

if(USING_RTM)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${rtm_SOURCE_DIR}/includes>)
endif()

if(USING_TINYFSM)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${tinyfsm_SOURCE_DIR}/include>)
endif()

if(USING_NFD)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${nfd_SOURCE_DIR}/src>)
endif()

if(USING_BOXER)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${boxer_SOURCE_DIR}/include>)
endif()

if(USING_PLF_NANOTIMER)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${plf_nanotimer_SOURCE_DIR}>)
endif()
	
if(USING_OPTICK)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${optick_SOURCE_DIR}/src>)

	target_compile_definitions(imgui_app 
		PUBLIC 
			USE_OPTICK=1
			OPTICK_ENABLE_TRACING=1
			OPTICK_ENABLE_GPU_D3D12=0	
			OPTICK_ENABLE_GPU_VULKAN=0)
endif()

if(USING_MUSTACH)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${mustach_SOURCE_DIR}>)
endif()

if(USING_LIBUV)
	list(APPEND private_include_paths $<BUILD_INTERFACE:${libuv_SOURCE_DIR}/src>)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${libuv_SOURCE_DIR}/include>)
endif()

if(USING_LEVELDB)
	list(APPEND private_include_paths $<BUILD_INTERFACE:${leveldb_SOURCE_DIR}>)
	list(APPEND public_include_paths $<BUILD_INTERFACE:${leveldb_SOURCE_DIR}/include>)

	target_compile_definitions(imgui_app 
		PRIVATE
			LEVELDB_COMPILE_LIBRARY
			${LEVELDB_PLATFORM_NAME}=1)
endif()

target_include_directories(imgui_app 
	PUBLIC ${public_include_paths}
	PRIVATE ${private_include_paths})

set_target_properties(imgui_app PROPERTIES CXX_STANDARD 17)

packageProject(
  # the name of the target to export
  NAME imgui_app
  # the version of the target to export
  VERSION ${PROJECT_VERSION}
  # a temporary directory to create the config files
  BINARY_DIR ${PROJECT_BINARY_DIR}
  # location of the target's public headers
  INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include
  # should match the target's INSTALL_INTERFACE include directory
  INCLUDE_DESTINATION include/imgui_app-${PROJECT_VERSION}
  # semicolon separated list of the project's dependencies
  DEPENDENCIES "")

################

if(IMGUI_BUILD_APP_EXAMPLES)
	file(GLOB example_sources 
		${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cpp)

    add_executable(imgui_app_example
        ${example_sources})
    
	set_target_properties(imgui_app_example PROPERTIES CXX_STANDARD 17)

    target_include_directories(imgui_app_example
        PRIVATE 
            ${imgui_SOURCE_DIR}/examples)
    
    target_link_libraries(imgui_app_example
        PUBLIC
            imgui_app)
endif()