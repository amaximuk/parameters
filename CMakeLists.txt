cmake_minimum_required(VERSION 3.18.0)

project(parameters
    VERSION   1.0.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PACKAGE_MANAGER "NONE" CACHE STRING "Type of package manager to be used for configure project")
set_property(CACHE PACKAGE_MANAGER PROPERTY STRINGS NONE CONAN 3RDPARTY)

if("${PACKAGE_MANAGER}" STREQUAL "CONAN")
	#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	#conan_basic_setup()
	set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
	set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

	#find_package(GTest REQUIRED CONFIG)
	find_package(yaml-cpp REQUIRED CONFIG)
	find_package(jsoncpp REQUIRED CONFIG)
elseif("${PACKAGE_MANAGER}" STREQUAL "3RDPARTY")
	include("${CMAKE_CURRENT_LIST_DIR}/3rdparty_v2/cmake/configure.cmake")
else()
	#find_package(GTest REQUIRED)
	find_package(yaml-cpp REQUIRED)
	find_package(jsoncpp REQUIRED)
endif()

include(GNUInstallDirs)

# начальное создание файла git_commit_hash.h, чтобы добавить его в проект
execute_process(
	COMMAND ${CMAKE_COMMAND} -DCOMPILE_PARAMETERS_OUT=${CMAKE_CURRENT_BINARY_DIR}/git_commit_hash.h -DCOMPILE_PARAMETERS_IN=${CMAKE_CURRENT_SOURCE_DIR}/cmake/git_commit_hash.h.in -P ${CMAKE_SOURCE_DIR}/cmake/git_commit_hash.cmake
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

# заполняем список конфигурационных файлов
set(GIT_COMMIT_HASH_FILES
	${CMAKE_CURRENT_BINARY_DIR}/git_commit_hash.h)

file(GLOB PROJECT_SOURCES src/*.cpp src/ext/*.cpp)
file(GLOB PROJECT_HEADERS src/*.h src/ext/*.h)

source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("Header Files" FILES ${PROJECT_HEADERS})

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${GIT_COMMIT_HASH_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME parameters
    DEBUG_POSTFIX d)
target_include_directories(${PROJECT_NAME} PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>)

if("${PACKAGE_MANAGER}" STREQUAL "CONAN")
	#target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
	target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp jsoncpp_lib_static)
elseif("${PACKAGE_MANAGER}" STREQUAL "3RDPARTY")
	target_link_libraries(${PROJECT_NAME} tpv2::yamlcpp tpv2::jsoncpp)
else()
	target_link_libraries(${PROJECT_NAME} yaml-cpp jsoncpp_lib_static)
endif()

# добавляем триггер на создание файла git_commit_hash.h при перекомпиляции
add_custom_command(
	TARGET ${PROJECT_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -DCOMPILE_PARAMETERS_OUT=${CMAKE_CURRENT_BINARY_DIR}/git_commit_hash.h -DCOMPILE_PARAMETERS_IN=${CMAKE_CURRENT_SOURCE_DIR}/cmake/git_commit_hash.h.in -P ${CMAKE_SOURCE_DIR}/cmake/git_commit_hash.cmake
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION cmake)

install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h")
# install(FILES ${PROJECT_SOURCE_DIR}/src/parameters.h
		# DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(parameters_compiler)

# добавляем тесты
if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()
