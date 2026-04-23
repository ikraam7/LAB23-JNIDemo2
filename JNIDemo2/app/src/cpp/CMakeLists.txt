cmake_minimum_required(VERSION 3.22.1)

project("nativecalc")

# Crée la bibliothèque partagée .so
add_library(
        nativecalc
        SHARED
        nativecalc.cpp)

# Trouve la lib Android pour les logs natifs
find_library(log-lib log)

# Lie notre lib avec le logger Android
target_link_libraries(
        nativecalc
        ${log-lib})