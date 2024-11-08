include( VDPDefaults )
option(@PROJECT_NAME@_DEACTIVATE_DEPRECATED_DATA "Deactivate output of deprecated vex elements" ON )
if (@PROJECT_NAME@_DEACTIVATE_DEPRECATED_DATA)
  vdp_add_cxxflag (DEACTIVATE_DEPRECATED_DATA)
endif (@PROJECT_NAME@_DEACTIVATE_DEPRECATED_DATA)

include( VDPDefaults )
option(@PROJECT_NAME@_ACTIVATE_EMPTY_ELEMENTS "Activate output of empty xml elements" OFF )
if (@PROJECT_NAME@_ACTIVATE_EMPTY_ELEMENTS)
  vdp_add_cxxflag (ACTIVATE_EMPTY_ELEMENTS)
endif (@PROJECT_NAME@_ACTIVATE_EMPTY_ELEMENTS)

set (VEXINTERFACE_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPTestA.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPTestB.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPTestC.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPTestD.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPTestE.hh
    #mk4_typedefs
    ${CMAKE_CURRENT_SOURCE_DIR}/include/DateStructWrapper.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SkyCoordStructWrapper.hh
    #ovex
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SourceStructWrapper.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ScanStructWrapper.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ChanStructWrapper.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/StationStructWrapper.hh
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../VDPSerialization/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../mk4_util/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../vex/include)

set (VEXINTERFACE_SOURCEFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPTestA.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPTestB.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPTestC.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPTestD.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPTestE.cc
    #mk4_typedefs
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DateStructWrapper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SkyCoordStructWrapper.cc
    #ovex
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SourceStructWrapper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ScanStructWrapper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ChanStructWrapper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/StationStructWrapper.cc
)

##################################################

add_library (VexInterface SHARED ${VEXINTERFACE_SOURCEFILES} ${VEXINTERFACE_HEADERFILES})

target_link_libraries (VexInterface VDPSerialization vex mk4_util)

vdp_install_headers (${VEXINTERFACE_HEADERFILES})
vdp_install_libraries (VexInterface)
