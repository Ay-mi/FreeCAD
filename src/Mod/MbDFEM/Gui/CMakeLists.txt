# SPDX-License-Identifier: LGPL-2.1-or-later


set(MbDFEMGui_LIBS
    MbDFEM
    PartDesign
    PartGui
    Spreadsheet
    SpreadsheetGui
    FreeCADGui
)

PYSIDE_WRAP_RC(MbDFEM_QRC_SRCS Resources/MbDFEM.qrc)

set (MbDFEM_TR_QRC ${CMAKE_CURRENT_BINARY_DIR}/Resources/MbDFEM_translation.qrc)
qt_find_and_add_translation(QM_SRCS "Resources/translations/*_*.ts"
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/translations)
qt_create_resource_file(${MbDFEM_TR_QRC} ${QM_SRCS})
qt_add_resources(MbDFEMResource_SRCS Resources/MbDFEM.qrc ${MbDFEM_TR_QRC})

SOURCE_GROUP("Resources" FILES ${MbDFEMResource_SRCS})

generate_from_py(ViewProviderMbDFEM)

set(MbDFEMGui_UIC_SRCS
    TaskMbDFEMCreateBom.ui
    TaskMbDFEMCreateJoint.ui
    TaskMbDFEMCreateSimulation.ui
    TaskMbDFEMCreateView.ui
    TaskMbDFEMInsertLink.ui
    MbDFEM.ui
)

SET(Python_SRCS
    ViewProviderMbDFEM.pyi
    ViewProviderMbDFEMPyImp.cpp
)
SOURCE_GROUP("Python" FILES ${Python_SRCS})

SET(MbDFEMGui_SRCS_Module
    AppMbDFEMGui.cpp
    AppMbDFEMGuiPy.cpp
    PreCompiled.h
    Commands.cpp
    Commands.h
    TaskMbDFEMMessages.cpp
    TaskMbDFEMMessages.h
    ViewProviderMbDFEM.cpp
    ViewProviderMbDFEM.h
    ViewProviderMbDAssemblyLink.cpp
    ViewProviderMbDAssemblyLink.h
    ViewProviderMbDPart.cpp
    ViewProviderMbDPart.h
    ViewProviderBom.cpp
    ViewProviderBom.h
    ViewProviderBomGroup.cpp
    ViewProviderBomGroup.h
    ViewProviderJointGroup.cpp
    ViewProviderJointGroup.h
    ViewProviderPartsGroup.cpp
    ViewProviderPartsGroup.h
    ViewProviderViewGroup.cpp
    ViewProviderViewGroup.h
    ViewProviderSimulationGroup.cpp
    ViewProviderSimulationGroup.h
    ${MbDFEM_QRC_SRCS}
)

SOURCE_GROUP("Module" FILES ${MbDFEMGui_SRCS_Module})


SET(MbDFEMGui_SRCS
    ${MbDFEMResource_SRCS}
    ${MbDFEMGui_UIC_HDRS}
    ${MbDFEMGui_SRCS_Module}
    ${Python_SRCS}
)

SET(MbDFEMGuiIcon_SVG
    Resources/icons/MbDFEMWorkbench.svg
)

add_library(MbDFEMGui SHARED ${MbDFEMGui_SRCS} ${MbDFEMGuiIcon_SVG})

if(FREECAD_USE_PCH)
    target_precompile_headers(MbDFEMGui PRIVATE
            $<$<COMPILE_LANGUAGE:CXX>:"${CMAKE_CURRENT_LIST_DIR}/PreCompiled.h">
    )
endif(FREECAD_USE_PCH)

target_include_directories(
    MbDFEMGui
    PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

if(WIN32)
    target_compile_definitions(MbDFEMGui PRIVATE _USE_MATH_DEFINES)
endif(WIN32)

target_link_libraries(MbDFEMGui ${MbDFEMGui_LIBS})
if (FREECAD_WARN_ERROR)
    target_compile_warn_error(MbDFEMGui)
endif()

SET_BIN_DIR(MbDFEMGui MbDFEMGui /Mod/MbDFEM)
SET_PYTHON_PREFIX_SUFFIX(MbDFEMGui)

fc_copy_sources(MbDFEMGui "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}/Mod/MbDFEM" ${MbDFEMGuiIcon_SVG})
fc_target_copy_resource(MbDFEMGui
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_BINARY_DIR}/Mod/MbDFEM
        MbDFEM_rc.py)

INSTALL(TARGETS MbDFEMGui DESTINATION ${CMAKE_INSTALL_LIBDIR})
INSTALL(FILES ${MbDFEMGuiIcon_SVG} DESTINATION "${CMAKE_INSTALL_DATADIR}/Mod/MbDFEM/Resources/icons")
INSTALL(FILES ${MbDFEM_QRC_SRCS} DESTINATION Mod/MbDFEM
)
