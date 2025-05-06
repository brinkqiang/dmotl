cmake_minimum_required(VERSION 3.8)

PROJECT(dmotl)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

SET(DMOTL_VERSION_MAJOR "1")
SET(DMOTL_VERSION_MINOR "0")
SET(DMOTL_VERSION_PATCH "1")
SET(DMOTL_VERSION "${DMOTL_VERSION_MAJOR}.${DMOTL_VERSION_MINOR}.${DMOTL_VERSION_PATCH}")

MESSAGE(STATUS "VERSION: ${DMOTL_VERSION}")

OPTION(USE_DMOTL "use dmotl" OFF)

add_definitions(/DOTL_DESTRUCTORS_DO_NOT_THROW)

ModuleImport("dmflags" "thirdparty/dmflags")

if (WIN32)
	ExeImport("test" "odbccp32;odbc32;dmflags")
else(WIN32)
	add_definitions(/DOTL_ODBC_UNIX)
	
	ExeImport("test" "odbc;dmflags")
endif(WIN32)
