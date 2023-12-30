set(PROJECT_NAME parameters_compiler)

file(GLOB PROJECT_SOURCES *.cpp)
file(GLOB PROJECT_HEADERS *.h)

source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("Header Files" FILES ${PROJECT_HEADERS})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${GIT_COMMIT_HASH_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)
add_dependencies(${PROJECT_NAME} parameters)
if("${PACKAGE_MANAGER}" STREQUAL "CONAN")
	target_link_libraries(${PROJECT_NAME} PRIVATE parameters yaml-cpp jsoncpp_lib_static)
elseif("${PACKAGE_MANAGER}" STREQUAL "3RDPARTY")
	target_link_libraries(${PROJECT_NAME} parameters tpv2::yamlcpp tpv2::jsoncpp)
else()
	target_link_libraries(${PROJECT_NAME} parameters yaml-cpp jsoncpp_lib_static)
endif()

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
