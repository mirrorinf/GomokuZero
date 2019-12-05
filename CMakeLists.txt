# encoding: UTF-8. no other encoding is known to work.
project(Gomoku)

set(PROJECT_SOURCE_DIR .)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ./bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ./bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall --std=c89 -Ofast")

add_executable(main ${PROJECT_SOURCE_DIR}/main.c ${PROJECT_SOURCE_DIR}/GomokuPublic.c ${PROJECT_SOURCE_DIR}/FrontEnd.c ${PROJECT_SOURCE_DIR}/Gameover.c ${PROJECT_SOURCE_DIR}/Evaluation.c ${PROJECT_SOURCE_DIR}/MCTSSeul.c)
