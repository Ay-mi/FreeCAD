from __future__ import annotations

from Base.Metadata import export

from App.GeoFeature import GeoFeature

@export(
    Include="Mod/MbDFEM/App/MbDPart.h",
    Namespace="MbDFEM",
)
class MbDPart(GeoFeature):
    """
    This class represents a part in MbDFEM.

    """
