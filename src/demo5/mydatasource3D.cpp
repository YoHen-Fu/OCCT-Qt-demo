#include "MyDataSource3D.h"

MyDataSource3D::MyDataSource3D() {
    for (Standard_Integer aNodeID = 1; aNodeID <= 4; aNodeID++)
    {
        m_nodes.Add( aNodeID );
    }

    for (Standard_Integer anElemID = 1; anElemID <= 1; anElemID++)
    {
        m_elements.Add( anElemID );
    }

    m_nodeCoords = new TColStd_HArray2OfReal(1, 4, 1, 3);

    m_nodeCoords->SetValue( 1, 1, 50 );
    m_nodeCoords->SetValue( 1, 2, 50 );
    m_nodeCoords->SetValue( 1, 3, 200 );

    m_nodeCoords->SetValue( 2, 1, 0 );
    m_nodeCoords->SetValue( 2, 2, 100 );
    m_nodeCoords->SetValue( 2, 3, 100 );

    m_nodeCoords->SetValue( 3, 1, 100 );
    m_nodeCoords->SetValue( 3, 2, 0 );
    m_nodeCoords->SetValue( 3, 3, 100 );

    m_nodeCoords->SetValue( 4, 1, 0 );
    m_nodeCoords->SetValue( 4, 2, 0 );
    m_nodeCoords->SetValue( 4, 3, 100 );

    m_elemNbNodes = new TColStd_HArray1OfInteger(1, 1);

    m_elemNbNodes->SetValue( 1, 4 );

    m_elemNodes = new TColStd_HArray2OfInteger(1, 1, 1, 4);

    m_elemNodes->SetValue(1, 1, 1);
    m_elemNodes->SetValue(1, 2, 2);
    m_elemNodes->SetValue(1, 3, 3);
    m_elemNodes->SetValue(1, 4, 4);

}

Standard_Boolean MyDataSource3D::GetGeom (const Standard_Integer ID,
                                        const Standard_Boolean IsElement,
                                        TColStd_Array1OfReal& Coords,
                                        Standard_Integer& NbNodes,
                                        MeshVS_EntityType& Type) const {
    if (IsElement)
    {
        if (ID >= 1 && ID <= m_elements.Extent())
        {
            Type = MeshVS_ET_Volume;
            NbNodes = m_elemNbNodes->Value(ID);

            for (Standard_Integer aNodeI = 1, aGlobCoordI = 1; aNodeI <= NbNodes; aNodeI++)
            {
                Standard_Integer anIdxNode = m_elemNodes->Value(ID, aNodeI);
                for(Standard_Integer aCoordI = 1; aCoordI <= 3; aCoordI++, aGlobCoordI++ )
                    Coords(aGlobCoordI) = m_nodeCoords->Value(anIdxNode, aCoordI);
            }
            return Standard_True;
        }
        else
            return Standard_False;
    }
    else
        if (ID >= 1 && ID <= m_nodes.Extent())
        {
            Type = MeshVS_ET_Node;
            NbNodes = 1;

            Coords( 1 ) = m_nodeCoords->Value(ID, 1);
            Coords( 2 ) = m_nodeCoords->Value(ID, 2);
            Coords( 3 ) = m_nodeCoords->Value(ID, 3);
            return Standard_True;
        }
        else
            return Standard_False;
}

Standard_Boolean MyDataSource3D::MyDataSource3D::Get3DGeom (const Standard_Integer ID,
                                                        Standard_Integer& NbNodes,
                                                        Handle(MeshVS_HArray1OfSequenceOfInteger)& Data) const {
    Handle(MeshVS_HArray1OfSequenceOfInteger) aMeshData;
    if (ID == 1 || ID == 5)
    {
        aMeshData = new MeshVS_HArray1OfSequenceOfInteger(1,4);
        NbNodes = 4;
        for (Standard_Integer anElemI = 1; anElemI <= 4; anElemI++)
        {
            aMeshData->ChangeValue(anElemI).Append( (anElemI - 1) % 4 );
            aMeshData->ChangeValue(anElemI).Append( anElemI % 4 );
            aMeshData->ChangeValue(anElemI).Append( (anElemI + 1) % 4 );
        }
        Data = aMeshData;
        return Standard_True;
    }

    if (ID == 2 || ID == 4)
    {
        aMeshData = new MeshVS_HArray1OfSequenceOfInteger(1,6);
        NbNodes = 8;
        for (Standard_Integer anElemI = 1, k = 1; anElemI <= 4; anElemI++)
        {
            aMeshData->ChangeValue(anElemI).Append( (k - 1) % 8 );
            aMeshData->ChangeValue(anElemI).Append( k % 8 );
            aMeshData->ChangeValue(anElemI).Append( (k + 1) % 8 );
            aMeshData->ChangeValue(anElemI).Append( (k + 2) % 8 );
            k+=2;
        }

        aMeshData->ChangeValue(5).Append( 0 );
        aMeshData->ChangeValue(5).Append( 3 );
        aMeshData->ChangeValue(5).Append( 4 );
        aMeshData->ChangeValue(5).Append( 7 );

        aMeshData->ChangeValue(6).Append( 1 );
        aMeshData->ChangeValue(6).Append( 2 );
        aMeshData->ChangeValue(6).Append( 5 );
        aMeshData->ChangeValue(6).Append( 6 );

        Data = aMeshData;
        return Standard_True;
    }

    if (ID == 3)
    {
        aMeshData = new MeshVS_HArray1OfSequenceOfInteger(1,5);
        NbNodes = 6;
        for (Standard_Integer anElemI = 1; anElemI <= 2; anElemI++)
        {
            aMeshData->ChangeValue(anElemI).Append( (anElemI - 1) * 3 );
            aMeshData->ChangeValue(anElemI).Append( (anElemI - 1) * 3 + 1 );
            aMeshData->ChangeValue(anElemI).Append( (anElemI - 1) * 3 + 2 );
        }
        for (Standard_Integer anElemI = 1; anElemI <= 3; anElemI++)
        {
            aMeshData->ChangeValue(2 + anElemI).Append( (anElemI - 1) % 3 );
            aMeshData->ChangeValue(2 + anElemI).Append( anElemI % 3 );
            aMeshData->ChangeValue(2 + anElemI).Append( anElemI % 3 + 3 );
            aMeshData->ChangeValue(2 + anElemI).Append( (anElemI - 1) % 3 + 3 );
        }
        Data = aMeshData;
        return Standard_True;
    }

    return Standard_False;
}

Standard_Boolean MyDataSource3D::GetGeomType (const Standard_Integer ID,
                                           const Standard_Boolean IsElement,
                                           MeshVS_EntityType& Type) const {
    if (IsElement) {
        if (ID >= 1 && ID <= m_elements.Extent())
        {
            Type = MeshVS_ET_Volume;
            return Standard_True;
        }
    }
    else {
        if (ID >= 1 && ID <= m_nodes.Extent())
        {
            Type = MeshVS_ET_Node;
            return Standard_True;
        }
    }
    return Standard_False;
}

Standard_Address MyDataSource3D::GetAddr (const Standard_Integer ID,
                                        const Standard_Boolean IsElement) const {
    return nullptr;
}

Standard_Boolean MyDataSource3D::GetNodesByElement (const Standard_Integer ID,
                                                  TColStd_Array1OfInteger& NodeIDs,
                                                  Standard_Integer& NbNodes) const {
    Standard_Integer aLow;
    if (ID == 1 || ID == 5)
    {
        NbNodes = 4;
        aLow = NodeIDs.Lower();
        NodeIDs (aLow)     = m_elemNodes->Value(ID, 1 );
        NodeIDs (aLow + 1) = m_elemNodes->Value(ID, 2 );
        NodeIDs (aLow + 2) = m_elemNodes->Value(ID, 3 );
        NodeIDs (aLow + 3) = m_elemNodes->Value(ID, 4 );
        return Standard_True;
    }

    if (ID == 2 || ID == 4)
    {
        NbNodes = 8;
        aLow = NodeIDs.Lower();
        NodeIDs (aLow)     = m_elemNodes->Value(ID, 1 );
        NodeIDs (aLow + 1) = m_elemNodes->Value(ID, 2 );
        NodeIDs (aLow + 2) = m_elemNodes->Value(ID, 3 );
        NodeIDs (aLow + 3) = m_elemNodes->Value(ID, 4 );
        NodeIDs (aLow + 4) = m_elemNodes->Value(ID, 5 );
        NodeIDs (aLow + 5) = m_elemNodes->Value(ID, 6 );
        NodeIDs (aLow + 6) = m_elemNodes->Value(ID, 7 );
        NodeIDs (aLow + 7) = m_elemNodes->Value(ID, 8 );
        return Standard_True;
    }

    if (ID == 3)
    {
        NbNodes = 6;
        aLow = NodeIDs.Lower();
        NodeIDs (aLow)     = m_elemNodes->Value(ID, 1 );
        NodeIDs (aLow + 1) = m_elemNodes->Value(ID, 2 );
        NodeIDs (aLow + 2) = m_elemNodes->Value(ID, 3 );
        NodeIDs (aLow + 3) = m_elemNodes->Value(ID, 4 );
        NodeIDs (aLow + 4) = m_elemNodes->Value(ID, 5 );
        NodeIDs (aLow + 5) = m_elemNodes->Value(ID, 6 );
        return Standard_True;
    }

    return Standard_False;
}

const TColStd_PackedMapOfInteger& MyDataSource3D::GetAllNodes() const {
    return m_nodes;
}

const TColStd_PackedMapOfInteger& MyDataSource3D::GetAllElements() const {
    return m_elements;
}

Standard_Boolean MyDataSource3D::GetNormal (const Standard_Integer Id,
                                          const Standard_Integer Max,
                                          Standard_Real& nx,
                                          Standard_Real& ny,
                                          Standard_Real& nz) const {
    return Standard_False;
}
