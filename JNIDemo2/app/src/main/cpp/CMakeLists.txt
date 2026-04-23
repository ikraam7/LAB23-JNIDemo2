cmake_minimum_required(VERSION 3.22.1)

project("shielddemo")

# Compile shield-core.cpp en bibliothèque partagée (.so)
add_library(
        shield-core
        SHARED
        jnidemo.cpp)

# Récupère la lib Android log du SDK
find_library(
        log-lib
        log)

# Lie shield-core avec la lib log
target_link_libraries(
        shield-core
        ${log-lib})