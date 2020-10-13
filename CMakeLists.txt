cmake_minimum_required( VERSION 3.12 )
project(imgui_app
	VERSION 1.7.8)

include(CMakeDependentOption)

option(IMGUI_BUILD_EXAMPLES "Build examples." OFF)
cmake_dependent_option(IMGUI_BUILD_APP_FW "Build app framework." OFF "IMGUI_BUILD_EXAMPLES" ON)
option(IMGUI_BUILD_APP_WIN32_DX11 "Win32 DX11" ON)
option(IMGUI_BUILD_APP_WIN32_DX12 "Win32 DX12" ON)
option(IMGUI_BUILD_APP_GLFW_VULKAN "GLFW Vulkan" OFF)

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(cmake/CPM.cmake)

####

CPMAddPackage(
  NAME PackageProject.cmake
  GITHUB_REPOSITORY TheLartians/PackageProject.cmake
  VERSION 1.3)

####

CPMAddPackage(
	NAME imgui_addons_cpm
	GITHUB_REPOSITORY loopunit/imgui_addons.cpm
	GIT_TAG 41b9321ac15c2468b1a44a5cc1a8a13c7c86ae5b 
	OPTIONS
		"IMGUI_BUILD_EXAMPLES ${IMGUI_BUILD_EXAMPLES}"
		"IMGUI_BUILD_APP_FW ${IMGUI_BUILD_APP_FW}"
		"IMGUI_BUILD_APP_WIN32_DX11 ${IMGUI_BUILD_APP_WIN32_DX11}"
		"IMGUI_BUILD_APP_WIN32_DX12 ${IMGUI_BUILD_APP_WIN32_DX12}"
		"IMGUI_BUILD_APP_GLFW_VULKAN ${IMGUI_BUILD_APP_GLFW_VULKAN}")

if( NOT ${imgui_addons_ADDED} )
	message( FATAL_ERROR "Could not find imgui_addons.cpm" )
endif()

#############

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

##

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

##

CPMAddPackage(
	NAME reckless
	GITHUB_REPOSITORY mattiasflodin/reckless
	GIT_TAG 5ac98bd799a44aef9b89fbfae509b044a4d1032b 
	DOWNLOAD_ONLY true)

if( NOT ${reckless_ADDED} )
	message( FATAL_ERROR "Could not find reckless" )
endif()

file(GLOB reckless_source 
	${reckless_SOURCE_DIR}/reckless/src/output_buffer.cpp
	${reckless_SOURCE_DIR}/reckless/src/ntoa.cpp
	${reckless_SOURCE_DIR}/reckless/src/template_formatter.cpp
	${reckless_SOURCE_DIR}/reckless/src/writer.cpp
	${reckless_SOURCE_DIR}/reckless/src/basic_log.cpp
	${reckless_SOURCE_DIR}/reckless/src/policy_log.cpp
	${reckless_SOURCE_DIR}/reckless/src/file_writer.cpp
	${reckless_SOURCE_DIR}/reckless/src/fd_writer.cpp
	${reckless_SOURCE_DIR}/reckless/src/mpsc_ring_buffer.cpp
	${reckless_SOURCE_DIR}/reckless/src/platform.cpp
	${reckless_SOURCE_DIR}/reckless/src/lockless_cv.cpp)

if(WIN32)
	file(GLOB reckless_platform_source 
		${reckless_SOURCE_DIR}/reckless/src/spsc_event_win32.cpp
		${reckless_SOURCE_DIR}/reckless/src/crash_handler_win32.cpp)
else()
	file(GLOB reckless_platform_source 
		${reckless_SOURCE_DIR}/reckless/src/crash_handler_unix.cpp)
endif()

install(
    DIRECTORY ${reckless_SOURCE_DIR}/reckless/include/reckless
    DESTINATION include/imgui_app-${PROJECT_VERSION})

##

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

##

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

##

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

##

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

##

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

##

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

#############

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

#############

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

#############

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

#############

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.*)

list(APPEND sources ${reckless_source} ${reckless_platform_source} ${cwalk_source} ${stx_source} ${optick_source} ${uv_source} ${uv_platform_source} ${leveldb_source} ${leveldb_platform_source})

add_library(imgui_app STATIC ${sources})

target_link_libraries(imgui_app PUBLIC imgui_app_fw imgui_addons)

if(WIN32)
	target_link_libraries(imgui_app PUBLIC Synchronization ${uv_platform_libraries})
endif()

target_include_directories(imgui_app 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		   $<BUILD_INTERFACE:${reckless_SOURCE_DIR}/reckless/include>
		   $<BUILD_INTERFACE:${atomic_queue_SOURCE_DIR}/include>
		   $<BUILD_INTERFACE:${stx_SOURCE_DIR}/include>
		   $<BUILD_INTERFACE:${nlohmann_json_SOURCE_DIR}/include>
		   $<BUILD_INTERFACE:${cereal_SOURCE_DIR}/include>
		   $<BUILD_INTERFACE:${entt_SOURCE_DIR}/src>
		   $<BUILD_INTERFACE:${taskflow_SOURCE_DIR}>
		   $<BUILD_INTERFACE:${optick_SOURCE_DIR}/src>
		   $<BUILD_INTERFACE:${libuv_SOURCE_DIR}/include>
		   $<BUILD_INTERFACE:${leveldb_SOURCE_DIR}/include>
		   $<BUILD_INTERFACE:${cwalk_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:imgui_app-${PROJECT_VERSION}>
	PRIVATE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
			$<BUILD_INTERFACE:${libuv_SOURCE_DIR}/src>
			$<BUILD_INTERFACE:${leveldb_SOURCE_DIR}>)

set_target_properties(imgui_app PROPERTIES CXX_STANDARD 17)

target_compile_definitions(imgui_app 
	PUBLIC 
		USE_OPTICK=1
		OPTICK_ENABLE_TRACING=1
		OPTICK_ENABLE_GPU_D3D12=0	
		OPTICK_ENABLE_GPU_VULKAN=0
	PRIVATE
		LEVELDB_COMPILE_LIBRARY
		${LEVELDB_PLATFORM_NAME}=1)

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