# SPDX-License-Identifier: LGPL-2.1-or-later

set(MbDFEM_LIBS
    Part
    PartDesign
    Spreadsheet
    FreeCADApp
    OndselSolver
)

generate_from_py(MbDAssembly)
generate_from_py(MbDAssemblyLink)
generate_from_py(BomObject)
generate_from_py(BomGroup)
generate_from_py(JointGroup)
generate_from_py(ViewGroup)
generate_from_py(SimulationGroup)


SET(Python_SRCS
    MbDAssembly.pyi
    MbDAssemblyPyImp.cpp
    MbDAssemblyLink.pyi
    MbDAssemblyLinkPyImp.cpp
    BomObject.pyi
    BomObjectPyImp.cpp
    BomGroup.pyi
    BomGroupPyImp.cpp
    JointGroup.pyi
    JointGroupPyImp.cpp
    ViewGroup.pyi
    ViewGroupPyImp.cpp
    SimulationGroup.pyi
    SimulationGroupPyImp.cpp
)
SOURCE_GROUP("Python" FILES ${Python_SRCS})

SET(Module_SRCS
    AppMbDFEM.cpp
    AppMbDFEMPy.cpp
    PreCompiled.h
)
SOURCE_GROUP("Module" FILES ${Module_SRCS})

SET(MbDFEM_SRCS
    MbDAssembly.cpp
    MbDAssembly.h
    MbDAssemblyLink.cpp
    MbDAssemblyLink.h
    MbDAssemblyUtils.cpp
    MbDAssemblyUtils.h
    BomObject.cpp
    BomObject.h
    BomGroup.cpp
    BomGroup.h
    JointGroup.cpp
    JointGroup.h
    ViewGroup.cpp
    ViewGroup.h
    SimulationGroup.cpp
    SimulationGroup.h
    ${Module_SRCS}
    ${Python_SRCS}
)

add_library(MbDFEM SHARED ${MbDFEM_SRCS})
target_include_directories(
    MbDFEM
    PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(MbDFEM ${MbDFEM_LIBS})
if (FREECAD_WARN_ERROR)
    target_compile_warn_error(MbDFEM)
endif()

if(FREECAD_USE_PCH)
    target_precompile_headers(MbDFEM PRIVATE
            $<$<COMPILE_LANGUAGE:CXX>:"${CMAKE_CURRENT_LIST_DIR}/PreCompiled.h">
    )
endif(FREECAD_USE_PCH)

SET_BIN_DIR(MbDFEM MbDFEMApp /Mod/MbDFEM)
SET_PYTHON_PREFIX_SUFFIX(MbDFEM)

INSTALL(TARGETS MbDFEM DESTINATION ${CMAKE_INSTALL_LIBDIR})
