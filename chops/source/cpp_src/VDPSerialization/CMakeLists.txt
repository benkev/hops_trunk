set (VDPSerialization_HEADERFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPCallbackTypes.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPAssociatedPassedPointerPODArrayOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPAssociatedPassedPointerPODOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPAssociatedPointerObjectOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPAssociatedPointerPODArrayInputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPAssociatedPointerPODArrayOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPAssociatedPointerPODInputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPAssociatedPointerPODOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPAssociatedReferencePODInputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPAssociatedValuePODInputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPAssociatedValuePODOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPStructMemberPODInputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPStructMemberPODOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPStructMemberPODArrayOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPCharacterDefinitions.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPDefinitions.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPFileReader.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPFileWriter.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPFixedSizeInputOutputObject.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPInputCollector.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPInputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPInputObject.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPInputOutputObject.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPIsDerivedFrom.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPObject.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPObjectInputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPObjectOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPOutputCollector.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPEmptyOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPOutputObject.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPPODArrayOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPPODConverter.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPPODInputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPPODOutputNode.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPStructuredASCIIHeaders.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPToken.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/include/VDPTokenizer.hh
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set (VDPSerialization_SOURCEFILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPCharacterDefinitions.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPFileReader.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPFileWriter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPInputCollector.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPOutputCollector.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VDPToken.cc
)

##################################################

add_library (VDPSerialization SHARED ${VDPSerialization_SOURCEFILES} ${VDPSerialization_HEADERFILES})

target_link_libraries (VDPSerialization)

vdp_install_headers (${VDPSerialization_HEADERFILES})
vdp_install_libraries (VDPSerialization)
