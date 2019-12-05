# encoding: UTF-8. no other encoding is known to work.
project(Gomoku)

set(PROJECT_SOURCE_DIR .)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ./bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ./bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall --std=c89 -O3")
add_library(public ${PROJECT_SOURCE_DIR}/GomokuPublic.c)
add_library(FrontEnd ${PROJECT_SOURCE_DIR}/FrontEnd.c)
add_library(BackEnd ${PROJECT_SOURCE_DIR}/Gameover.c ${PROJECT_SOURCE_DIR}/Evaluation.c)
add_library(MCTS ${PROJECT_SOURCE_DIR}/MCTSSeul.c)

add_executable(main ${PROJECT_SOURCE_DIR}/main.c)
target_link_libraries(main public FrontEnd BackEnd MCTS)
