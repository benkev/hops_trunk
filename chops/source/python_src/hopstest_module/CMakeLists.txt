#python package setup ##########################################################
set(abs_srcdir "${CMAKE_CURRENT_SOURCE_DIR}")

find_program(PYTHON "python")

if (PYTHON)
    set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
    set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
    set(DEPS        "${CMAKE_CURRENT_SOURCE_DIR}/hopstestb/__init__.py")
    set(OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/build/hopstest_timestamp")


    configure_file(${SETUP_PY_IN} ${SETUP_PY})

    add_custom_command(OUTPUT ${OUTPUT}
                       COMMAND ${PYTHON} ${SETUP_PY} build
                       COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
                       DEPENDS ${DEPS})

    add_custom_target(hopstest_target ALL DEPENDS ${OUTPUT})

    #install the module/package 
    install(CODE "execute_process(COMMAND ${PYTHON} ${SETUP_PY} install --prefix=${CMAKE_INSTALL_PREFIX} )")

    set(HOPSTEST_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/hopstestsuiteb.py.in")
    set(HOPSTEST_PY    "${CMAKE_CURRENT_BINARY_DIR}/hopstestsuiteb.py")

    # #configure the executable to get the python version
    configure_file(${HOPSTEST_PY_IN} ${HOPSTEST_PY} @ONLY)

    #install script (currently goes in bin directory, but we may want to make a separate 'scripts directory')
    install(PROGRAMS ${HOPSTEST_PY} DESTINATION ${BIN_INSTALL_DIR})
endif()

#configure/generate Makefile.am for autotools build ############################

chops_configure_automake("" "" "" "" "")
