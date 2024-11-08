option (@PROJECT_NAME@_ENABLE_VDP_TEST "Build developer test applications" ON)
if (@PROJECT_NAME@_ENABLE_VDP_TEST)
  enable_testing ()
endif (@PROJECT_NAME@_ENABLE_VDP_TEST)

if (@PROJECT_NAME@_ENABLE_VDP_TEST)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../mk4_util/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../vex/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../VDPSerialization/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../VexInterface/include)

    set (VDP_LIBS
        mk4_util
        vex
        VDPSerialization
        VexInterface
    )

    set(SOURCE_BASENAMES
        TestParseVex
        TestSerialization
        TestStructWriting
        TestVexConversion
    )

    foreach( BASENAME ${SOURCE_BASENAMES} )
        add_executable( ${BASENAME} ${BASENAME}.cc )
        target_link_libraries( ${BASENAME} ${VDP_LIBS} )
    endforeach( BASENAME )

    vdp_install_executables( ${SOURCE_BASENAMES} )

    vdp_install_data ( ${CMAKE_CURRENT_SOURCE_DIR}/data/0727-115.zbgwce )
    vdp_install_data ( ${CMAKE_CURRENT_SOURCE_DIR}/data/OJ287.yyexqh )

endif (@PROJECT_NAME@_ENABLE_VDP_TEST)
