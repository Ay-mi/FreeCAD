from __future__ import annotations

from Base.Metadata import export

from App.DocumentObjectGroup import DocumentObjectGroup

@export(Include="Mod/MbDFEM/App/PartGroup.h", Namespace="MbDFEM")
class PartGroup(DocumentObjectGroup):
    """
    This class is a group subclass for MbDPart objects.

    """
