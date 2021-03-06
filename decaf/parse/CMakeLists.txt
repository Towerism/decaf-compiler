set(FLEX_INPUT ${CMAKE_CURRENT_LIST_DIR}/scanner.l)
set(FLEX_OUTPUT lex.cc)
if(FLEX_FOUND)
  add_custom_command(
    OUTPUT ${FLEX_OUTPUT}
    COMMAND ${FLEX_EXECUTABLE} -d -o ${FLEX_OUTPUT} ${FLEX_INPUT}
    MAIN_DEPENDENCY ${FLEX_INPUT}
    )
endif()

set(BISON_INPUT ${CMAKE_CURRENT_LIST_DIR}/parser.y)
set(BISON_OUTPUT parse.cc)
if(BISON_FOUND)
  add_custom_command(
    OUTPUT ${BISON_OUTPUT}
    COMMAND ${BISON_EXECUTABLE} -dvty ${BISON_INPUT} -o ${BISON_OUTPUT}
    MAIN_DEPENDENCY ${BISON_INPUT}
    DEPENDS ${FLEX_OUTPUT}
    )
endif()

set_source_files_properties(${FLEX_OUTPUT} GENERATED)
set_source_files_properties(${BISON_OUTPUT} GENERATED)

set(PARSE_SRCS
  ${BISON_OUTPUT}
  ${FLEX_OUTPUT}
  )

set(PARSE_LIBS
  ast
  util
  )
add_library(parse ${PARSE_SRCS})
target_link_libraries(parse ${PARSE_LIBS})
