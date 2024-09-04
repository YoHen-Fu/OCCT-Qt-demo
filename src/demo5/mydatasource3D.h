#ifndef MYDATASOURCE3D_H
#define MYDATASOURCE3D_H

#include <MeshVS_DataSource.hxx>
#include <MeshVS_DataSource3D.hxx>
#include <TColStd_HArray1OfInteger.hxx>
#include <TColStd_HArray2OfReal.hxx>
#include <TColStd_HArray2OfInteger.hxx>

class MyDataSource3D : public MeshVS_DataSource3D
{
public:
    MyDataSource3D();

    //! Returns geometry information about node (if IsElement is False) or element (IsElement is True) by coordinates.
    //! For element this method must return all its nodes coordinates in the strict order: X, Y, Z and
    //! with nodes order is the same as in wire bounding the face or link. NbNodes is number of nodes of element.
    //! It is recommended to return 1 for node. Type is an element type.
    Standard_EXPORT Standard_Boolean GetGeom (const Standard_Integer theID, const Standard_Boolean theIsElement, TColStd_Array1OfReal& theCoords, Standard_Integer& theNbNodes, MeshVS_EntityType& theType) const Standard_OVERRIDE;

    //! This method returns topology information about 3D-element
    //! Returns false if element with ID isn't 3D or because other troubles
    Standard_EXPORT virtual Standard_Boolean Get3DGeom (const Standard_Integer theID, Standard_Integer& theNbNodes, Handle(MeshVS_HArray1OfSequenceOfInteger)& theData) const Standard_OVERRIDE;

    //! This method is similar to GetGeom, but returns only element or node type. This method is provided for
    //! a fine performance.
    Standard_EXPORT Standard_Boolean GetGeomType (const Standard_Integer theID, const Standard_Boolean theIsElement, MeshVS_EntityType& theType) const Standard_OVERRIDE;

    //! This method returns by number an address of any entity which represents element or node data structure.
    Standard_EXPORT Standard_Address GetAddr (const Standard_Integer theID, const Standard_Boolean theIsElement) const Standard_OVERRIDE;

    //! This method returns information about what node this element consist of.
    Standard_EXPORT virtual Standard_Boolean GetNodesByElement (const Standard_Integer theID, TColStd_Array1OfInteger& theNodeIDs, Standard_Integer& theNbNodes) const Standard_OVERRIDE;

    //! This method returns map of all nodes the object consist of.
    Standard_EXPORT const TColStd_PackedMapOfInteger& GetAllNodes() const Standard_OVERRIDE;

    //! This method returns map of all elements the object consist of.
    Standard_EXPORT const TColStd_PackedMapOfInteger& GetAllElements() const Standard_OVERRIDE;

    //! This method calculates normal of face, which is using for correct reflection presentation.
    //! There is default method, for advance reflection this method can be redefined.
    Standard_EXPORT virtual Standard_Boolean GetNormal (const Standard_Integer theID, const Standard_Integer theMax, Standard_Real& theNx, Standard_Real& theNy, Standard_Real& theNz) const Standard_OVERRIDE;
private:
    TColStd_PackedMapOfInteger m_nodes;
    TColStd_PackedMapOfInteger m_elements;
    Handle(TColStd_HArray1OfInteger) m_elemNbNodes;
    Handle(TColStd_HArray2OfReal) m_nodeCoords;
    Handle(TColStd_HArray2OfInteger) m_elemNodes;
};

#endif // MYDATASOURCE3D_H
