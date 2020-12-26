cmake_minimum_required(VERSION 3.2)

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

add_definitions(/DOTL_ODBC_UNIX)
if (WIN32)
	ExeImport("test" "odbccp32;odbc32")
else(WIN32)
	add_definitions(/DOTL_ODBC_UNIX)
	ExeImport("test" "odbccp32;odbc32")
endif(WIN32)

#ADD_CUSTOM_COMMAND(
#TARGET dmotltest
#PRE_BUILD
#COMMAND ${CMAKE_COMMAND} -E copy_if_different
#${CMAKE_CURRENT_SOURCE_DIR}/config/test.csv
#$<TARGET_FILE_DIR:dmotltest>
#)

#INSTALL(DIRECTORY include/ DESTINATION include)
#INSTALL(TARGETS dmotl
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib)

#SET(DMOTL_LIBS "dmotl")
#MARK_AS_ADVANCED(${DMOTL_LIBS})