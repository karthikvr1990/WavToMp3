include(ExternalProject)

ExternalProject_Add(lame-build
  URL http://downloads.sourceforge.net/project/lame/lame/3.100/lame-3.100.tar.gz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/lame-3.100
  CONFIGURE_COMMAND ${CMAKE_CURRENT_BINARY_DIR}/lame-3.100/src/lame-build/configure --srcdir=${CMAKE_CURRENT_BINARY_DIR}/lame-3.100/src/lame-build/  --enable-static=yes --disable-shared
  #BUILD_COMMAND make
  PATCH_COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/lame/CMakeLists.txt" "${CMAKE_CURRENT_BINARY_DIR}/lame-3.100/src/lame-build/CMakeLists.txt"
  #COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/lame-3.100/src/lame-build/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/lame-3.100/src/lame-build/config.h"
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(lame-build source_dir)
ExternalProject_Get_Property(lame-build binary_dir)

add_library(lame STATIC IMPORTED)

if (WIN32)
	set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
else (WIN32)
	set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
endif (WIN32)

file(MAKE_DIRECTORY "${source_dir}/include")
set_property(TARGET lame PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/include")
set_property(TARGET lame PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/lame-3.100/src/lame-build-build/libmp3lame/.libs/libmp3lame${CMAKE_STATIC_LIBRARY_SUFFIX}")
#set_property(TARGET lame PROPERTY IMPORTED_LOCATION "${binary_dir}/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}libmp3lame-static${CMAKE_STATIC_LIBRARY_SUFFIX}")


add_dependencies(lame lame-build)
