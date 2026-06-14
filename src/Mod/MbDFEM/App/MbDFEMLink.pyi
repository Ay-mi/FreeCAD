# SPDX-License-Identifier: LGPL-2.1-or-later

from __future__ import annotations

from typing import Final

from Base.Metadata import export

from App.Part import Part

@export(
    Include="Mod/MbDFEM/App/MbDFEMLink.h",
    Namespace="MbDFEM",
)
class MbDFEMLink(Part):
    """
    This class handles document objects in MbDFEM

    Author: Ondsel (development@ondsel.com)
    License: LGPL-2.1-or-later
    """

    Joints: Final[list]
    """A list of all joints this MbDFEM link has."""
