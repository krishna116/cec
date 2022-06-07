set(CEC_ENUM_LEXER_LEX ${CMAKE_CURRENT_SOURCE_DIR}/CecEnumLexer.lex)
set(CEC_ENUM_LEXER_CPP ${CMAKE_CURRENT_SOURCE_DIR}/CecEnumLexer.cpp)
set_source_files_properties(${CEC_ENUM_LEXER_CPP} PROPERTIES GENERATED true)

find_program(RE2C_EXE NAMES "re2c-3.0" "re2c")
if(RE2C_EXE)
    message("${PROJECT_NAME}: find re2c tool = ${RE2C_EXE}")
    add_custom_command(OUTPUT ${CEC_ENUM_LEXER_CPP}
    COMMAND ${RE2C_EXE} ${CEC_ENUM_LEXER_LEX} -o ${CEC_ENUM_LEXER_CPP} "--input-encoding" "utf8"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${CEC_ENUM_LEXER_LEX})
else()
    message(WARNING "${PROJECT_NAME}: Cannot find re2c tool, the Lexer won't update.")
endif()

##--------------------------------------------------------
add_library(ceclib 
    ${CEC_ENUM_LEXER_CPP}
    CecEnumParser.cpp
    CecEnumChecker.cpp
    CecCodeGenerator.cpp
    CecWaiter.cpp
)

target_include_directories(ceclib 
PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR} ## config.h
)

##--------------------------------------------------------
add_executable(${PROJECT_NAME}
    cec.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE ceclib)

##--------------------------------------------------------
set(COLOR_TXT "${CMAKE_SOURCE_DIR}/test/Color.txt")
set(COLOR_HPP "${CMAKE_SOURCE_DIR}/test/Color.hpp")
set(UNIT_TEST_SAMPLE "${CMAKE_SOURCE_DIR}/sample/CodeUnitTestSample.txt")
set(COLOR_UNIT_TEST "${CMAKE_SOURCE_DIR}/test/ColorUnitTest.cpp")
set_source_files_properties(${COLOR_HPP} PROPERTIES GENERATED true)
set_source_files_properties(${COLOR_UNIT_TEST} PROPERTIES GENERATED true)

add_custom_command(TARGET ${PROJECT_NAME} 
POST_BUILD 
    COMMAND $<TARGET_FILE:${PROJECT_NAME}> ${COLOR_TXT} "-o" ${COLOR_HPP}
    COMMAND $<TARGET_FILE:${PROJECT_NAME}> ${COLOR_TXT} "-o" ${COLOR_UNIT_TEST} "-i" ${UNIT_TEST_SAMPLE}
)

##--------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
