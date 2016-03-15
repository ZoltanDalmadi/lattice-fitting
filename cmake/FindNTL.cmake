find_path(NTL_INCLUDE_DIR
  NAMES NTL/ZZ.h
  HINTS ENV NTL_INC_DIR
  ENV NTL_DIR
  PATH_SUFFIXES include
  DOC "The directory containing the NTL include files"
)

find_library(NTL_LIBRARY
  NAMES ntl
  HINTS ENV NTL_LIB_DIR
  ENV NTL_DIR
  PATH_SUFFIXES lib
  DOC "Path to the NTL library"
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(NTL
  DEFAULT_MSG
  NTL_LIBRARY
  NTL_INCLUDE_DIR
)

if(NTL_FOUND)

  set(NTL_VERSION_H "${NTL_INCLUDE_DIR}/NTL/version.h")

  if (EXISTS ${NTL_VERSION_H})

    file(READ "${NTL_VERSION_H}" NTL_VERSION_H_CONTENTS)
    string(REGEX MATCH "[0-9]+(\\.[0-9]+)+" NTL_VERSION "${NTL_VERSION_H_CONTENTS}")
    message(STATUS "NTL VERSION ${NTL_VERSION}")

  endif (EXISTS ${NTL_VERSION_H})

  set(NTL_INCLUDE_DIRS ${NTL_INCLUDE_DIR})
  set(NTL_LIBRARIES ${NTL_LIBRARY})
  include_directories(${NTL_INCLUDE_DIRS})

  if (NOT MSVC)
    list(APPEND CMAKE_EXE_LINKER_FLAGS "-lntl")
  endif()

endif(NTL_FOUND)
