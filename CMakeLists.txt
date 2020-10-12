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

#############

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.*)

list(APPEND sources ${reckless_source} ${reckless_platform_source} ${stx_source})

add_library(imgui_app STATIC ${sources})

target_link_libraries(imgui_app PUBLIC imgui_app_fw imgui_addons)

if(WIN32)
	target_link_libraries(imgui_app PUBLIC Synchronization)
endif()

target_include_directories(imgui_app 
	PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		   $<BUILD_INTERFACE:${reckless_SOURCE_DIR}/reckless/include>
		   $<BUILD_INTERFACE:${atomic_queue_SOURCE_DIR}/include>
		   $<BUILD_INTERFACE:${stx_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:imgui_app-${PROJECT_VERSION}>)

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