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
	GIT_TAG f1a1e155fb24462995d84e18ae6490b783a542e7 
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

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.*)

add_library(imgui_app STATIC ${sources})

target_link_libraries(imgui_app PUBLIC imgui_app_fw imgui_addons)

target_include_directories(imgui_app PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:imgui_app-${PROJECT_VERSION}>)

packageProject(
  # the name of the target to export
  NAME imgui_app
  # the version of the target to export
  VERSION ${PROJECT_VERSION}
  # a temporary directory to create the config files
  BINARY_DIR ${PROJECT_BINARY_DIR}
  # location of the target's public headers
  INCLUDE_DIR ${STAGING_ROOT}/include
  # should match the target's INSTALL_INTERFACE include directory
  INCLUDE_DESTINATION include/imgui_app-${PROJECT_VERSION}
  # semicolon separated list of the project's dependencies
  DEPENDENCIES ""
)

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