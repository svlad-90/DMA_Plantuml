################### CPP ###################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
################### CPP ( END ) ###########################

CMAKE_MINIMUM_REQUIRED(VERSION 3.14 FATAL_ERROR)

################### COMMON_DEPS ###########################
include(FetchContent)
FetchContent_Declare(
  dma_common_deps
  GIT_REPOSITORY https://github.com/svlad-90/DMA_CommonDeps.git
  GIT_TAG        master
) 
FetchContent_MakeAvailable(dma_common_deps)
include(${FETCHCONTENT_BASE_DIR}/dma_common_deps-src/CMakeLists_Helper.txt)
################### COMMON_DEPS ( END ) ###################

################### DEPENDENCIES ##########################
DMA_sync_g_test_framework()
################### DEPENDENCIES ( END )###################

################### PROJECT ###############################
project (DMA_Plantuml)
enable_testing()

DMA_Prepare()

include_directories(include)

DMA_add_subdirectory_with_clang_tidy(src)
add_subdirectory(test)
################### PROJECT ( END ) #######################

