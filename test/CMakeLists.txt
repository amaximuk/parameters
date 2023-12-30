set(TARGET_NAME test_parameters)

# определяем директории для сборки
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/release)

# добавляем исходники и хидеры
file(GLOB PROJECT_SOURCES src/*.cpp) 
file(GLOB PROJECT_HEADERS src/*.h)

# распределяем файлы на манер вижуал студии
source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("Header Files" FILES ${PROJECT_HEADERS})

# добавление библиотеки в проект
add_executable(${TARGET_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES
	FOLDER "tests"
	VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration))
target_link_libraries(${TARGET_NAME} PRIVATE
	tpv2::gtest)

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
