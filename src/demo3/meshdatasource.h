#ifndef MESHDATASOURCE_H
#define MESHDATASOURCE_H

#include <Standard.hxx>
#include <Standard_Type.hxx>

#include <Standard_Real.hxx>
// #include <Standard_Address.hxx>
#include <Standard_Boolean.hxx>
#include <Standard_Integer.hxx>

#include <TColStd_PackedMapOfInteger.hxx>
#include <TColStd_HArray2OfInteger.hxx>
#include <TColStd_HArray2OfReal.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <MeshVS_EntityType.hxx>
#include <MeshVS_DataSource.hxx>
#include <Poly_Triangulation.hxx>
#include <vector>

struct PointXYZ
{
    int NodeID;
    double X;
    double Y;
    double Z;
    double Ux = 0;
    double Uy = 0;
    double Uz = 0;

    PointXYZ()
    {
        X = 0;
        Y = 0;
        Z = 0;
    }
    PointXYZ(int id ,double x,double y,double z)
    {
        NodeID = id;
        X = x;
        Y = y;
        Z = z;
    }
    void setUnknowns(double ux,double uy,double uz)
    {
        Ux = ux;
        Uy = uy;
        Uz = uz;
    }
};
struct Element
{
    int G1;
    int G2;
    int G3;
    Element()
    {
        G1 = 0;
        G2 = 0;
        G3 = 0;
    }
    Element(int g1 ,int g2,int g3)
    {
        G1 = g1;
        G2 = g2;
        G3 = g3;
    }
};

class MeshDataSource;
DEFINE_STANDARD_HANDLE(MeshDataSource, MeshVS_DataSource)

class MeshDataSource: public MeshVS_DataSource
{

public:
    explicit MeshDataSource(std::vector<PointXYZ> CoordData, std::vector<Element> Ele2NodeData);

    void setAmplificationFactor(double amp)
    {
        ampFactor = amp;
    }
    void setDistRadio(double amp)
    {
        ampDistRadio = amp;
    }
    Standard_Boolean GetGeom(const Standard_Integer ID, const Standard_Boolean IsElement, TColStd_Array1OfReal& Coords, Standard_Integer& NbNodes, MeshVS_EntityType& Type) const Standard_OVERRIDE;

    Standard_Boolean GetGeomType(const Standard_Integer ID, const Standard_Boolean IsElement, MeshVS_EntityType& Type) const Standard_OVERRIDE;

    Standard_Address GetAddr(const Standard_Integer ID, const Standard_Boolean IsElement) const Standard_OVERRIDE;

    virtual Standard_Boolean GetNodesByElement(const Standard_Integer ID, TColStd_Array1OfInteger& NodeIDs, Standard_Integer& NbNodes) const Standard_OVERRIDE;

    const TColStd_PackedMapOfInteger& GetAllNodes() const Standard_OVERRIDE;

    const TColStd_PackedMapOfInteger& GetAllElements() const Standard_OVERRIDE;

    Standard_Boolean GetNormal(const Standard_Integer Id, const Standard_Integer Max, Standard_Real& nx, Standard_Real& ny, Standard_Real& nz) const Standard_OVERRIDE;
    DEFINE_STANDARD_RTTIEXT(MeshDataSource, MeshVS_DataSource)
protected:

private:

    TColStd_PackedMapOfInteger myNodes;
    TColStd_PackedMapOfInteger myElements;
    Handle(TColStd_HArray2OfInteger) myElemNodes;
    Handle(TColStd_HArray2OfReal) myNodeCoords;
    Handle(TColStd_HArray2OfReal) myNodeDisps;
    Handle(TColStd_HArray2OfReal) myElemNormals;
    double ampFactor = 1;
    double ampDistRadio = 1;
};



#endif // MESHDATASOURCE_H
