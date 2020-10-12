################### CPP ###################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
################### CPP ( END ) ###########################

################### HELPERS ###############################
include(cmake/CMakeLists_Helper.txt)
################### HELPERS ( END ) #######################

################### PROJECT ###############################
project (DMA_Plantuml)
enable_testing()

CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

sync_g_test_framework()

add_subdirectory(test)

enable_clang_tidy()

add_subdirectory(src)
################### PROJECT ( END ) #######################

