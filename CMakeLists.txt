gaudi_subdir(LCIOInput v0r1)

find_package(EDM4HEP REQUIRED)
find_package(K4LCIOReader REQUIRED)

set(LCIOInput_srcs
    src/LCIOInput.cc
    )

gaudi_add_module(LCIOInput ${LCIOInput_srcs}
    INCLUDE_DIRS FWCore ${EDM4HEP_INCLUDE_DIR} ${K4LCIOReader_INCLUDE_DIR}
    LINK_LIBRARIES GaudiKernel GaudiAlgLib FWCore K4LCIOReader::K4LCIOReader
    )

#gaudi_add_test()
