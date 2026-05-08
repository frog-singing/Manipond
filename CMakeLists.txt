# Manipond/CMakeLists.txt
cmake_minimum_required(VERSION 3.21)
project(Manipond_Repository LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL) #VERSION 3.21+ 特性
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()


if(TARGET manipond::exosuit)
	message(STATUS "[Manipond] Using existing manipond::exosuit target.")
else()
    # 定义一个接口库（Header-only 专用）
    add_library(manipond_exosuit INTERFACE)

    # 设置一个库别名
	add_library(manipond::exosuit ALIAS manipond_exosuit)

    # 强制要求 C++20 特性传递
    target_compile_features(manipond_exosuit INTERFACE cxx_std_20)

    # 关闭编译器特定的扩展，确保代码的纯度和可移植性
	set_target_properties(manipond_exosuit PROPERTIES CXX_EXTENSIONS OFF)

    # 为当前库设置包含路径，并且任何链接此库的目标都将自动获得此路径
    target_include_directories(manipond_exosuit INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/exosuit_library>
		$<INSTALL_INTERFACE:include>
    )

    # 把接口头文件放在这里，使得IDE可见。不影响编译
    target_sources(manipond_exosuit INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/exosuit_library/manipond/exosuit.h"
    )

    message(STATUS "[Manipond] Created manipond::exosuit target.")
endif()


if(TARGET manipond::exosuit_runtime)
	message(STATUS "[Manipond] Using existing manipond::exosuit_runtime target.")
else()
    # 定义一个静态库 exosuit runtime
    add_library(manipond_exosuit_runtime STATIC exosuit_library/manipond/library_placeholder.cpp)

    # 设置一个库别名
	add_library(manipond::exosuit_runtime ALIAS manipond_exosuit_runtime)

    # 为当前库设置包含路径，并且任何链接此库的目标都将自动获得此路径
    target_include_directories(manipond_exosuit_runtime PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/exosuit_library>
		$<INSTALL_INTERFACE:include>
    )
    
    # 链接本仓库中的 exosuit 库
	target_link_libraries(manipond_exosuit_runtime PUBLIC manipond::exosuit)

    message(STATUS "[Manipond] Created manipond::exosuit_runtime target.")
endif()


if(TARGET manipond::meta)
	message(STATUS "[Manipond] Using existing manipond::meta target.")
else()
    # 定义一个接口库（Header-only 专用）
    add_library(manipond_meta INTERFACE)

    # 设置一个库别名
	add_library(manipond::meta ALIAS manipond_meta)

    # 强制要求 C++20 特性传递
    target_compile_features(manipond_meta INTERFACE cxx_std_20)

    # 关闭编译器特定的扩展，确保代码的纯度和可移植性
	set_target_properties(manipond_meta PROPERTIES CXX_EXTENSIONS OFF)

    # 为当前库设置包含路径，并且任何链接此库的目标都将自动获得此路径
    target_include_directories(manipond_meta INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/meta_library>
		$<INSTALL_INTERFACE:include>
    )

    # 把接口头文件放在这里，使得IDE可见。不影响编译
    target_sources(manipond_meta INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/meta_library/manipond/meta.h"
    )

    # 链接本仓库中的 exosuit 库
	target_link_libraries(manipond_meta INTERFACE manipond::exosuit)

    message(STATUS "[Manipond] Created manipond::meta target.")
endif()


if(PROJECT_IS_TOP_LEVEL)
    enable_testing()

    # 自动收集 test 目录下的所有 .cpp 文件，并且不会下探到子目录
    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

    foreach(test_file ${TEST_SOURCES})
        get_filename_component(test_name ${test_file} NAME_WE)
        add_executable(${test_name} ${test_file})

        # 链接到当前仓库的库
        target_link_libraries(${test_name} PRIVATE
            manipond::meta
            manipond::exosuit_runtime
        )

        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()
