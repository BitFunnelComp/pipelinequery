cmake_minimum_required(VERSION 2.8)
project(INDEX_PARTITIONING)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif ()

if (UNIX)
   # C++11
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fopenmp -mavx -lrt -laio -Wall")

   # For hardware popcount
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2")

   # Extensive warnings
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-missing-braces")
   # Silence a warning bug in Boost
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs")
   endif ()
   # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion")

   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb") # Add debug info anyway

endif()

find_package(Boost 1.42.0 COMPONENTS iostreams unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#link_directories(${INDEX_PARTITIONING_SOURCE_DIR}/PM_part)

# add the root directory to include path to make includes absolute
include_directories(${INDEX_PARTITIONING_SOURCE_DIR})

add_subdirectory(succinct EXCLUDE_FROM_ALL)
add_subdirectory(FastPFor EXCLUDE_FROM_ALL)
add_subdirectory(streamvbyte EXCLUDE_FROM_ALL)

# bypass integer_encoding_library build system, only take what we need
include_directories(${INDEX_PARTITIONING_SOURCE_DIR}/integer_encoding_library/include)
add_library(block_codecs
  block_codecs.cpp
  FastPFor/src/simdunalignedbitpacking.cpp
  integer_encoding_library/src/compress/table/decUnary.cpp
  integer_encoding_library/src/compress/table/decGamma.cpp
  integer_encoding_library/src/compress/table/decDelta.cpp
  integer_encoding_library/src/io/BitsReader.cpp
  integer_encoding_library/src/io/BitsWriter.cpp
  )

#add_executable(create_freq_index create_freq_index.cpp)
#target_link_libraries(create_freq_index
#  ${Boost_LIBRARIES}
#  FastPFor_lib
#  block_codecs
#  streamvbyte_static 
#  streamvbyte
#  )

#add_executable(create_wand_data create_wand_data.cpp)
#target_link_libraries(create_wand_data
#  ${Boost_LIBRARIES}
#  )

#add_executable(queries queries.cpp)
#target_link_libraries(queries
#  ${Boost_LIBRARIES}
#  FastPFor_lib
#  block_codecs
#  )

add_executable(Query_DRAMbase Query_DRAMbase.cpp)
target_link_libraries(Query_DRAMbase
  ${Boost_LIBRARIES}
  FastPFor_lib
  block_codecs
  streamvbyte_static 
  streamvbyte
  )
  
add_executable(Query_Nativebase Query_Nativebase.cpp)
target_link_libraries(Query_Nativebase
  ${Boost_LIBRARIES}
  FastPFor_lib
  block_codecs
  streamvbyte_static 
  streamvbyte
  )
  
add_executable(Query_Pipebase Query_Pipebase.cpp)
target_link_libraries(Query_Pipebase
  ${Boost_LIBRARIES}
  FastPFor_lib
  block_codecs
  streamvbyte_static 
  streamvbyte
  )
  
add_executable(Query_PipeCache Query_PipeCache.cpp)
target_link_libraries(Query_PipeCache
  ${Boost_LIBRARIES}
  FastPFor_lib
  block_codecs
  streamvbyte_static 
  streamvbyte
  )
add_executable(Query_PipeCachePref Query_PipeCachePref.cpp)
target_link_libraries(Query_PipeCachePref
  ${Boost_LIBRARIES}
  FastPFor_lib
  block_codecs
  streamvbyte_static 
  streamvbyte
  )
  
add_executable(Greedy_Reorder Greedy_Reorder.cpp)
target_link_libraries(Greedy_Reorder
  ${Boost_LIBRARIES}
  FastPFor_lib
  block_codecs
  streamvbyte_static 
  streamvbyte
  )
    
#enable_testing()
#add_subdirectory(test)