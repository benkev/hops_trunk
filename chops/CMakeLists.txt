cmake_minimum_required(VERSION 3.0)
project(chops)

if(POLICY CMP0053)
    cmake_policy(SET CMP0053 OLD)
endif()

# Module version
set(MODULE_VERSION_MAJOR 0)
set(MODULE_VERSION_MINOR 0)
set(MODULE_VERSION_PATCH 0)
set(MODULE_VERSION "${MODULE_VERSION_MAJOR}.${MODULE_VERSION_MINOR}.${MODULE_VERSION_PATCH}")
set(MODULE_VERSION_NUMBER "${MODULE_VERSION_MAJOR}.${MODULE_VERSION_MINOR}${MODULE_VERSION_PATCH}")


#if this option is enabled, the cmake build with re-configure the automake
#build files (.am), used for boot-strapping automake build, useful for developers only
option(CHOPS_RECONFIGURE_AUTOMAKE "Reconfigure automake files. Use with caution." OFF )

#to avoid conflict with native hops library names, 
#we post-fix all chops libraries with a 'b' and
#pre-fix all chops executables with a 'b'
set (CHOPS_LIBPOSTFIX "b")
set (CHOPS_EXEPREFIX "b")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
include( chopsDefaults )

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install" CACHE PATH "default install path" FORCE )
endif()

#define install directories
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/config")
set(SHARE_TEXT_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/text")
set(DATA_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/data")

file(MAKE_DIRECTORY ${INCLUDE_INSTALL_DIR})
file(MAKE_DIRECTORY ${LIB_INSTALL_DIR})
file(MAKE_DIRECTORY ${BIN_INSTALL_DIR})
file(MAKE_DIRECTORY ${CONFIG_INSTALL_DIR})
file(MAKE_DIRECTORY ${DATA_INSTALL_DIR})
file(MAKE_DIRECTORY ${SHARE_TEXT_INSTALL_DIR})

chops_module_paths( chops )

#compiler flags
#chops_require_cpp11()

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra" )
# set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror" )
# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror" )
message(STATUS "Using compiler ${COMPILER_ID} ${COMPILER_VERSION}")

#set source directories
set (CSOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source/c_src)
set (CPPSOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source/cpp_src)
set (PYSOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source/python_src)
set (SHSOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source/bash_src)

#config header generation to mimic behavior of autoheader generated hops_config.h
#some of these are probably not necessary

include(TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    message(STATUS "Machine is BIG_ENDIAN")
    set(WORDS_BIGENDIAN TRUE)
else()
    message(STATUS "Machine is LITTLE_ENDIAN")
    set(WORDS_BIGENDIAN FALSE)
    # chops_add_cflag(BYTFLP)
endif()

#mimic a bunch of checks automake does (which afaik are not used anywhere)
include(CheckIncludeFile)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)

check_include_file(alloca.h HAVE_ALLOCA_H)
check_include_file(arpa/inet.h HAVE_ARPA_INET_H)
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(float.h HAVE_FLOAT_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(limits.h HAVE_LIMITS_H)

check_include_file(locale.h HAVE_LOCALE_H)
check_include_file(netdb.h HAVE_NETDB_H)
check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(stddef.h HAVE_STDDEF_H)
check_include_file(stdint.h HAVE_STDINT_H)

check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(unistd.h HAVE_UNISTD_H)

set(HOPS_SVN_REV_VALUE 0)
set(HOPS_REVISION_VALUE ${MODULE_VERSION_NUMBER})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/hops_config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/include/hops_config.h)
chops_install_headers(${CMAKE_CURRENT_BINARY_DIR}/include/hops_config.h)
chops_include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
chops_add_cflag(HAVE_CONFIG_H)

#mk4 and vex libraries
add_subdirectory(${CSOURCE}/mk4util)
add_subdirectory(${CSOURCE}/vex)
add_subdirectory(${CSOURCE}/dfio)
add_subdirectory(${CSOURCE}/afio)

#fourfit libraries
add_subdirectory(${CSOURCE}/fourfit_libs/ffcontrol)
add_subdirectory(${CSOURCE}/fourfit_libs/ffmath)
add_subdirectory(${CSOURCE}/fourfit_libs/ffcore)
add_subdirectory(${CSOURCE}/fourfit_libs/ffio)
add_subdirectory(${CSOURCE}/fourfit_libs/ffsearch)
add_subdirectory(${CSOURCE}/fourfit_libs/ffplot)

#hops c executables
add_subdirectory(${CSOURCE}/applications/fourfit)
add_subdirectory(${CSOURCE}/applications/alist)
add_subdirectory(${CSOURCE}/applications/aedit)

#python interface modules
add_subdirectory(${PYSOURCE}/mk4_module)
add_subdirectory(${PYSOURCE}/hopstest_module)
add_subdirectory(${PYSOURCE}/vpal_module)
add_subdirectory(${PYSOURCE}/ffcontrol_module)
add_subdirectory(${PYSOURCE}/vex_module)

#data for testing
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/data)

# add_subdirectory(${SOURCE}/chopsSerialization)
# add_subdirectory(${SOURCE}/VexInterface)
# add_subdirectory(${SOURCE}/Test)

#figure out the python install path
#so we can add it to PYTHONPATH
find_program(PYTHON "python")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/pypath.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR} 
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
    GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
execute_process(COMMAND ${PYTHON} "${CMAKE_CURRENT_BINARY_DIR}/pypath.py"  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} OUTPUT_VARIABLE PYTHON_SITE_PREFIX)

#env script
configure_file( ${SHSOURCE}/chopsenv.sh.cmake ${CMAKE_CURRENT_BINARY_DIR}/chopsenv.sh @ONLY)
install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/chopsenv.sh DESTINATION ${BIN_INSTALL_DIR} )
