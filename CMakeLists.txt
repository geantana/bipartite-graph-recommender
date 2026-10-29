cmake_minimum_required(VERSION 3.15)
project(bipartite_recommender C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic -O2)
endif()

add_executable(bipartite_recommender src/main.c)

# Copy available dataset to build directory for convenience
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data/interactions.txt")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/data/interactions.txt
        ${CMAKE_CURRENT_BINARY_DIR}/interactions.txt
        COPYONLY
    )
else()
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/data/sample_interactions.txt
        ${CMAKE_CURRENT_BINARY_DIR}/sample_interactions.txt
        COPYONLY
    )
endif()
