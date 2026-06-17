# SPDX-License-Identifier: LGPL-2.1-or-later

if (NOT FREECAD_USE_EXTERNAL_ONDSELSOLVER)
include_directories(
    ${CMAKE_SOURCE_DIR}/src/3rdParty/OndselSolver
)
else ()
    check_include_file_cxx(OndselSolver/enum.h HAVE_ONDSELSOLVER_H)
    if (NOT HAVE_ONDSELSOLVER_H)
        message(FATAL_ERROR "FREECAD_USE_EXTERNAL_ONDSELSOLVER is set but the required system includes are not available")
    endif ()
endif ()

add_subdirectory(App)

if(BUILD_GUI)
    add_subdirectory(Gui)
endif(BUILD_GUI)

set(MbDFEM_Scripts
    Init.py
    CommandCreateMbDAssembly.py
    CommandCreateMbDBom.py
    CommandInsertMbDLink.py
    CommandInsertMbDNewPart.py
    CommandSolveMbDAssembly.py
    CommandCreateMbDJoint.py
    CommandCreateMbDView.py
    CommandCreateMbDSimulation.py
    CommandExportMbDASMT.py
    TestMbDFEMWorkbench.py
    MbDJointObject.py
    MbDFEMPreferences.py
    MbDFEMImport.py
    MbDSoSwitchMarker.py
    UtilsMbDFEM.py
)

if(BUILD_GUI)
    list (APPEND MbDFEM_Scripts InitGui.py)
endif(BUILD_GUI)

INSTALL(
    FILES
    ${MbDFEM_Scripts}
    DESTINATION
    Mod/MbDFEM
)

SET(MbDFEMScripts_SRCS
    MbDFEM/__init__.py
)


SET(MbDFEMTests_SRCS
    MbDFEMTests/__init__.py
    MbDFEMTests/TestCore.py
    MbDFEMTests/TestCommandInsertLink.py
    MbDFEMTests/mocks/__init__.py
    MbDFEMTests/mocks/MockGui.py
)


SET(all_files
    ${MbDFEMTests_SRCS}
    ${MbDFEMScripts_SRCS}
)

ADD_CUSTOM_TARGET(MbDFEMScripts ALL
    SOURCES ${all_files}
)

SET(test_files
    ${MbDFEM_Scripts}
    ${MbDFEMTests_SRCS}
)

ADD_CUSTOM_TARGET(MbDFEMTests ALL
    SOURCES ${test_files}
)

# Target MbDFEMScripts handles copying MbDFEMTests_SRCS and MbDFEMScripts_SRCS
fc_copy_sources(MbDFEMScripts "${CMAKE_BINARY_DIR}/Mod/MbDFEM" ${all_files})

# Target MbDFEMTests should only handle copying the remaining MbDFEM_Scripts
# to avoid racing with MbDFEMScripts over the same files.
fc_copy_sources(MbDFEMTests "${CMAKE_BINARY_DIR}/Mod/MbDFEM" ${MbDFEM_Scripts})

# Ensure MbDFEMScripts finishes its directory creation and copying before
# MbDFEMTests begins.
add_dependencies(MbDFEMTests MbDFEMScripts)

INSTALL(
    FILES
        ${MbDFEMTests_SRCS}
    DESTINATION
        Mod/MbDFEM/MbDFEMTests
)
INSTALL(
    FILES
        ${MbDFEMScripts_SRCS}
    DESTINATION
        Mod/MbDFEM/MbDFEM
)
