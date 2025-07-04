#include "mydatasource.h"

#include <Precision.hxx>
#include <Standard_Type.hxx>
#include <TColStd_DataMapOfIntegerReal.hxx>


MyDataSource::MyDataSource ()
{


    myNodeCoords = new TColStd_HArray2OfReal (1, 3, 1, 3);
    myNodes.Add( 1 );
    myNodeCoords->SetValue(1, 1, 0);
    myNodeCoords->SetValue(1, 2, 100);
    myNodeCoords->SetValue(1, 3, 0);

    myNodes.Add( 2 );
    myNodeCoords->SetValue(2, 1, 0);
    myNodeCoords->SetValue(2, 2, 100);
    myNodeCoords->SetValue(2, 3, 100);

    myNodes.Add( 3 );
    myNodeCoords->SetValue(3, 1, 0);
    myNodeCoords->SetValue(3, 2, 0);
    myNodeCoords->SetValue(3, 3, 0);

    myElemNormals = new TColStd_HArray2OfReal(1, 1, 1, 3);
    myElemNodes = new TColStd_HArray2OfInteger(1, 1, 1, 3);

    // const Poly_Triangle aTri = myMesh->Triangle (i);

    // Standard_Integer V[3];
    // aTri.Get (V[0], V[1], V[2]);

    const gp_Pnt aP1 {0, 100, 0};
    const gp_Pnt aP2 {0, 100, 100};
    const gp_Pnt aP3 {0, 0, 0};

    gp_Vec aV1(aP1, aP2);
    gp_Vec aV2(aP2, aP3);

    gp_Vec aN = aV1.Crossed(aV2);
    if (aN.SquareMagnitude() > Precision::SquareConfusion())
        aN.Normalize();
    else
        aN.SetCoord(0.0, 0.0, 0.0);

    myElements.Add( 1 );
    myElemNodes->SetValue(1, 1, 1);
    myElemNodes->SetValue(1, 2, 2);
    myElemNodes->SetValue(1, 3, 3);


    myElemNormals->SetValue (1, 1, aN.X());
    myElemNormals->SetValue (1, 2, aN.Y());
    myElemNormals->SetValue (1, 3, aN.Z());

}

//================================================================
// Function : GetGeom
// Purpose  :
//================================================================
Standard_Boolean MyDataSource::GetGeom
    ( const Standard_Integer ID, const Standard_Boolean IsElement,
     TColStd_Array1OfReal& Coords, Standard_Integer& NbNodes,
     MeshVS_EntityType& Type ) const
{
    // if( myMesh.IsNull() )
    //     return Standard_False;

    if( IsElement )
    {
        if( ID>=1 && ID<=myElements.Extent() )
        {
            Type = MeshVS_ET_Face;
            NbNodes = 3;

            for( Standard_Integer i = 1, k = 1; i <= 3; i++ )
            {
                Standard_Integer IdxNode = myElemNodes->Value(ID, i);
                for(Standard_Integer j = 1; j <= 3; j++, k++ )
                    Coords(k) = myNodeCoords->Value(IdxNode, j);
            }

            return Standard_True;
        }
        else
            return Standard_False;
    }
    else
        if( ID>=1 && ID<=myNodes.Extent() )
        {
            Type = MeshVS_ET_Node;
            NbNodes = 1;

            Coords( 1 ) = myNodeCoords->Value(ID, 1);
            Coords( 2 ) = myNodeCoords->Value(ID, 2);
            Coords( 3 ) = myNodeCoords->Value(ID, 3);
            return Standard_True;
        }
        else
            return Standard_False;
}

//================================================================
// Function : GetGeomType
// Purpose  :
//================================================================
Standard_Boolean MyDataSource::GetGeomType
    ( const Standard_Integer,
     const Standard_Boolean IsElement,
     MeshVS_EntityType& Type ) const
{
    if( IsElement )
    {
        Type = MeshVS_ET_Face;
        return Standard_True;
    }
    else
    {
        Type = MeshVS_ET_Node;
        return Standard_True;
    }
}

//================================================================
// Function : GetAddr
// Purpose  :
//================================================================
Standard_Address MyDataSource::GetAddr
    ( const Standard_Integer, const Standard_Boolean ) const
{
    return NULL;
}

//================================================================
// Function : GetNodesByElement
// Purpose  :
//================================================================
Standard_Boolean MyDataSource::GetNodesByElement
    ( const Standard_Integer ID,
     TColStd_Array1OfInteger& theNodeIDs,
     Standard_Integer& /*theNbNodes*/ ) const
{
    // if( myMesh.IsNull() )
    //     return Standard_False;

    if( ID>=1 && ID<=myElements.Extent() && theNodeIDs.Length() >= 3 )
    {
        Standard_Integer aLow = theNodeIDs.Lower();
        theNodeIDs (aLow)     = myElemNodes->Value(ID, 1 );
        theNodeIDs (aLow + 1) = myElemNodes->Value(ID, 2 );
        theNodeIDs (aLow + 2) = myElemNodes->Value(ID, 3 );
        return Standard_True;
    }
    return Standard_False;
}

//================================================================
// Function : GetAllNodes
// Purpose  :
//================================================================
const TColStd_PackedMapOfInteger& MyDataSource::GetAllNodes() const
{
    return myNodes;
}

//================================================================
// Function : GetAllElements
// Purpose  :
//================================================================
const TColStd_PackedMapOfInteger& MyDataSource::GetAllElements() const
{
    return myElements;
}

//================================================================
// Function : GetNormal
// Purpose  :
//================================================================
Standard_Boolean MyDataSource::GetNormal
    ( const Standard_Integer Id, const Standard_Integer Max,
     Standard_Real& nx, Standard_Real& ny,Standard_Real& nz ) const
{
    // if( myMesh.IsNull() )
    //     return Standard_False;

    if( Id>=1 && Id<=myElements.Extent() && Max>=3 )
    {
        nx = myElemNormals->Value(Id, 1);
        ny = myElemNormals->Value(Id, 2);
        nz = myElemNormals->Value(Id, 3);
        return Standard_True;
    }
    else
        return Standard_False;
}


