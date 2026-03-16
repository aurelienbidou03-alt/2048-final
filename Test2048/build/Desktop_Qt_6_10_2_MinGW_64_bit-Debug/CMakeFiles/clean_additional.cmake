# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appTest2048_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appTest2048_autogen.dir\\ParseCache.txt"
  "appTest2048_autogen"
  )
endif()
