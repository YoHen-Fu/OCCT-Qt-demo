#include "meshdatasource.h"

#include <Precision.hxx>
#include <TColgp_SequenceOfXYZ.hxx>
#include <TColStd_DataMapOfIntegerInteger.hxx>
#include <TColStd_DataMapOfIntegerReal.hxx>

#include <QFile>
#include <QTextStream>

IMPLEMENT_STANDARD_RTTIEXT(MeshDataSource, MeshVS_DataSource)

MeshDataSource::MeshDataSource(std::vector<PointXYZ> CoordData, std::vector<Element> Ele2NodeData)
{
    myNodeCoords = new TColStd_HArray2OfReal(1, CoordData.size(), 1, 3);
    myNodeDisps = new TColStd_HArray2OfReal(1, CoordData.size(), 1, 3);
    for (size_t i = 1; i <= CoordData.size(); i++)
    {
        myNodes.Add(i);
        myNodeCoords->SetValue(i, 1, CoordData[i-1].X);
        myNodeCoords->SetValue(i, 2, CoordData[i-1].Y);
        myNodeCoords->SetValue(i, 3, CoordData[i-1].Z);
        myNodeDisps->SetValue(i, 1, CoordData[i-1].Ux);
        myNodeDisps->SetValue(i, 2, CoordData[i-1].Uy);
        myNodeDisps->SetValue(i, 3, CoordData[i-1].Uz);

    }
    myElemNormals = new TColStd_HArray2OfReal(1, Ele2NodeData.size(), 1, 3);
    myElemNodes = new TColStd_HArray2OfInteger(1, Ele2NodeData.size(), 1, 3);

    for (size_t i = 1; i <= Ele2NodeData.size(); i++)
    {
        myElements.Add(i);

        myElemNodes->SetValue(i, 1, Ele2NodeData[i - 1].G1+1);
        myElemNodes->SetValue(i, 2, Ele2NodeData[i - 1].G2+1);
        myElemNodes->SetValue(i, 3, Ele2NodeData[i - 1].G3+1);

        myElemNormals->SetValue(i, 1, 0);
        myElemNormals->SetValue(i, 2, 0);
        myElemNormals->SetValue(i, 3, 1);
    }
}
Standard_Boolean MeshDataSource::GetGeom(const Standard_Integer ID, const Standard_Boolean IsElement,TColStd_Array1OfReal& Coords, Standard_Integer& NbNodes,MeshVS_EntityType& Type) const
{
    if (IsElement)
    {
        if (ID >= 1 && ID <= myElements.Extent())
        {
            Type = MeshVS_ET_Face;
            NbNodes = 3;

            for (Standard_Integer i = 1, k = 1; i <= 3; i++)
            {
                Standard_Integer IdxNode = myElemNodes->Value(ID, i);
                for (Standard_Integer j = 1; j <= 3; j++, k++)
                    Coords(k) = myNodeCoords->Value(IdxNode, j)+myNodeDisps->Value(IdxNode, j)*ampFactor*ampDistRadio;
            }

            return Standard_True;
        }
        else
            return Standard_False;
    }
    else
        if (ID >= 1 && ID <= myNodes.Extent())
        {
            Type = MeshVS_ET_Node;
            NbNodes = 1;
            Coords(1) = myNodeCoords->Value(ID, 1)+myNodeDisps->Value(ID, 1)*ampFactor*ampDistRadio;
            Coords(2) = myNodeCoords->Value(ID, 2)+myNodeDisps->Value(ID, 2)*ampFactor*ampDistRadio;
            Coords(3) = myNodeCoords->Value(ID, 3)+myNodeDisps->Value(ID, 3)*ampFactor*ampDistRadio;
            return Standard_True;
        }
        else
            return Standard_False;
}


Standard_Boolean MeshDataSource::GetGeomType(const Standard_Integer,const Standard_Boolean IsElement,MeshVS_EntityType& Type) const
{
    if (IsElement)
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


Standard_Address MeshDataSource::GetAddr(const Standard_Integer, const Standard_Boolean) const
{
    return NULL;
}


Standard_Boolean MeshDataSource::GetNodesByElement(const Standard_Integer ID,TColStd_Array1OfInteger& theNodeIDs,Standard_Integer& /*theNbNodes*/) const
{

    if (ID >= 1 && ID <= myElements.Extent() && theNodeIDs.Length() >= 3)
    {
        Standard_Integer aLow = theNodeIDs.Lower();
        theNodeIDs(aLow) = myElemNodes->Value(ID, 1);
        theNodeIDs(aLow + 1) = myElemNodes->Value(ID, 2);
        theNodeIDs(aLow + 2) = myElemNodes->Value(ID, 3);
        return Standard_True;
    }
    return Standard_False;
}


const TColStd_PackedMapOfInteger& MeshDataSource::GetAllNodes() const
{
    return myNodes;
}

const TColStd_PackedMapOfInteger& MeshDataSource::GetAllElements() const
{
    return myElements;
}


Standard_Boolean MeshDataSource::GetNormal(const Standard_Integer Id, const Standard_Integer Max,Standard_Real& nx, Standard_Real& ny, Standard_Real& nz) const
{

    if (Id >= 1 && Id <= myElements.Extent() && Max >= 3)
    {
        nx = myElemNormals->Value(Id, 1);
        ny = myElemNormals->Value(Id, 2);
        nz = myElemNormals->Value(Id, 3);
        return Standard_True;
    }
    else
        return Standard_False;
}
