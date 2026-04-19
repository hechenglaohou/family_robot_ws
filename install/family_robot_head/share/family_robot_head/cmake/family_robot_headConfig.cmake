# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_family_robot_head_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED family_robot_head_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(family_robot_head_FOUND FALSE)
  elseif(NOT family_robot_head_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(family_robot_head_FOUND FALSE)
  endif()
  return()
endif()
set(_family_robot_head_CONFIG_INCLUDED TRUE)

# output package information
if(NOT family_robot_head_FIND_QUIETLY)
  message(STATUS "Found family_robot_head: 0.0.0 (${family_robot_head_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'family_robot_head' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${family_robot_head_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(family_robot_head_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${family_robot_head_DIR}/${_extra}")
endforeach()
