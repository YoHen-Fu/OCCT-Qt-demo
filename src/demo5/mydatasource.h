#ifndef MYDATASOURCE_H
#define MYDATASOURCE_H

#include <MeshVS_DataSource.hxx>

// 假设的 ElementData 和 NodeData 结构体
struct ElementData {
    Standard_Integer ID; // 元素标识符
    std::vector<Standard_Integer> nodes; // 组成元素的节点 ID 列表
    ElementData(Standard_Integer ID_, std::vector<Standard_Integer> nodes_){
        ID = ID_;
        nodes = nodes_;
    }
};

struct NodeData {
    Standard_Integer ID; // 节点标识符
    Standard_Real x, y, z; // 节点的坐标

    NodeData(Standard_Integer ID_, Standard_Real x_, Standard_Real y_, Standard_Real z_){
        ID = ID_;
        x = x_;
        y = y_;
        z = z_;
    }

    Standard_Real X() const { return x; }
    Standard_Real Y() const { return y; }
    Standard_Real Z() const { return z; }

    bool isValid() const { return true; } // 假设所有节点都是有效的
};

struct GroupData {
    Standard_Integer ID; // 组标识符
    std::vector<Standard_Integer> elements; // 组成组的元素 ID 列表
    GroupData(Standard_Integer ID_, std::vector<Standard_Integer> elements_){
        ID = ID_;
        elements = elements_;
    }
};

class MyDataSource : public MeshVS_DataSource
{
private:
    const ElementData* GetElementData(const Standard_Integer ID) const;
    const NodeData* GetNodeData(const Standard_Integer ID) const;
    const GroupData* GetGroupData(const Standard_Integer ID) const;
    const std::vector<NodeData>& GetAllNodeData() const;
    const std::vector<ElementData>& GetAllElementData() const;
    const std::vector<GroupData>& GetAllGroupData() const;
public:
    MyDataSource();

    //! Returns geometry information about node or element
    //! ID is the numerical identificator of node or element
    //! IsElement indicates this ID describe node ( if Standard_False ) or element ( if Standard_True )
    //! Coords is an array of coordinates of node(s).
    //! For node it is only 3 numbers: X, Y, Z in the strict order
    //! For element it is 3*n numbers, where n is number of this element vertices
    //! The order is strict also: X1, Y1, Z1, X2,...., where Xi, Yi, Zi are coordinates of vertices
    //! NbNodes is number of nodes. It is recommended this parameter to be set to 1 for node.
    //! Type is type of node or element (from enumeration). It is recommended this parameter to be set to
    //! MeshVS_ET_Node for node.
    virtual Standard_Boolean GetGeom (const Standard_Integer ID, const Standard_Boolean IsElement, TColStd_Array1OfReal& Coords, Standard_Integer& NbNodes, MeshVS_EntityType& Type) const override;

    //! This method is similar to GetGeom, but returns only element or node type.
    virtual Standard_Boolean GetGeomType (const Standard_Integer ID, const Standard_Boolean IsElement, MeshVS_EntityType& Type) const override;

    //! This method returns topology information about 3D-element
    //! Returns false if element with ID isn't 3D or because other troubles
    virtual Standard_Boolean Get3DGeom (const Standard_Integer ID, Standard_Integer& NbNodes, Handle(MeshVS_HArray1OfSequenceOfInteger)& Data) const override;

    //! This method returns pointer which represents element or node data structure.
    //! This address will be saved in MeshVS_MeshEntityOwner, so that you can access to data structure fast
    //! by the method Owner(). In the redefined method you can return NULL.
    //! ID is the numerical identificator of node or element
    //! IsElement indicates this ID describe node ( if Standard_False ) or element ( if Standard_True )
    virtual Standard_Address GetAddr (const Standard_Integer ID, const Standard_Boolean IsElement) const override;

    //! This method returns information about nodes this element consist of.
    //! ID is the numerical identificator of element.
    //! NodeIDs is the output array of nodes IDs in correct order,
    //! the same as coordinates returned by GetGeom().
    //! NbNodes is number of nodes (number of items set in NodeIDs).
    //! Returns False if element does not exist
    virtual Standard_Boolean GetNodesByElement (const Standard_Integer ID, TColStd_Array1OfInteger& NodeIDs, Standard_Integer& NbNodes) const override;

    //! This method returns map of all nodes the object consist of.
    virtual const TColStd_PackedMapOfInteger& GetAllNodes() const override;

    //! This method returns map of all elements the object consist of.
    virtual const TColStd_PackedMapOfInteger& GetAllElements() const override;

    //! This method calculates normal of face, which is using for correct reflection presentation.
    //! There is default method, for advance reflection this method can be redefined.
    //! Id is the numerical identificator of only element!
    //! Max is maximal number of nodes an element can consist of
    //! nx, ny, nz  are values whose represent coordinates of normal (will be returned)
    //! In the redefined method you can return normal with length more then 1, but in this case
    //! the appearance of element will be more bright than usual. For ordinary brightness you must return
    //! normal with length 1
    virtual Standard_Boolean GetNormal (const Standard_Integer Id, const Standard_Integer Max, Standard_Real& nx, Standard_Real& ny, Standard_Real& nz) const override;

    //! This method return normal of node ranknode of face Id,
    //! which is using for smooth shading presentation.
    //! Returns false if normal isn't defined.
    virtual Standard_Boolean GetNodeNormal (const Standard_Integer ranknode, const Standard_Integer ElementId, Standard_Real& nx, Standard_Real& ny, Standard_Real& nz) const override;

    //! This method puts components of normal vectors at each node of a mesh face (at each face of a mesh volume)
    //! into the output array.
    //! Returns false if some problem was detected during calculation of normals.
    //! Id is an identifier of the mesh element.
    //! IsNodal, when true, means that normals at mesh element nodes are needed. If nodal normals
    //! are not available, or IsNodal is false, or the mesh element is a volume, then the output array contents
    //! depend on the element type:
    //! face: a normal calculated by GetNormal() is duplicated for each node of the face;
    //! volume: normals to all faces of the volume are computed (not for each node!).
    //! MaxNodes is maximal number of nodes an element can consist of.
    //! Normals contains the result.
    virtual Standard_Boolean GetNormalsByElement (const Standard_Integer Id, const Standard_Boolean IsNodal, const Standard_Integer MaxNodes, Handle(TColStd_HArray1OfReal)& Normals) const override;

    //! This method returns map of all groups the object contains.
    virtual void GetAllGroups (TColStd_PackedMapOfInteger& Ids) const override;

    //! This method returns map of all group elements.
    virtual Standard_Boolean GetGroup (const Standard_Integer Id, MeshVS_EntityType& Type, TColStd_PackedMapOfInteger& Ids) const override;

    //! This method returns pointer which represents group data structure.
    //! This address will be saved in MeshVS_MeshOwner, so that you can access to data structure fast
    //! by the method Owner(). In the redefined method you can return NULL.
    //! ID is the numerical identificator of group
    virtual Standard_Address GetGroupAddr (const Standard_Integer ID) const override;

    //! Returns True if advanced mesh selection is enabled.
    //! Default implementation returns False.
    //! It should be redefined to return True for advanced
    //! mesh selection activation.
    virtual Standard_Boolean IsAdvancedSelectionEnabled() const override;

    //! Returns the bounding box of the whole mesh.
    //! It is used in advanced selection mode to define roughly
    //! the sensitive area of the mesh.
    //! It can be redefined to get access to a box computed in advance.
    virtual Bnd_Box GetBoundingBox() const override;

    //! Returns maps of entities (nodes and elements) detected
    //! by mouse click at the point (X,Y) on the current view plane,
    //! with the tolerance aTol.
    //! DMin - is out argument should return actual detection tolerance.
    //! Returns True if something is detected.
    //! It should be redefined if the advanced mesh selection is
    //! activated. Default implementation returns False.
    virtual Standard_Boolean GetDetectedEntities (const Handle(MeshVS_Mesh)& Prs, const Standard_Real X, const Standard_Real Y, const Standard_Real aTol, Handle(TColStd_HPackedMapOfInteger)& Nodes, Handle(TColStd_HPackedMapOfInteger)& Elements, Standard_Real& DMin) override;

    //! Returns maps of entities (nodes and elements) detected
    //! by mouse selection with rectangular box (XMin, YMin, XMax, YMax)
    //! on the current view plane, with the tolerance aTol.
    //! Returns True if something is detected.
    //! It should be redefined if the advanced mesh selection is
    //! activated. Default implementation returns False.
    virtual Standard_Boolean GetDetectedEntities (const Handle(MeshVS_Mesh)& Prs, const Standard_Real XMin, const Standard_Real YMin, const Standard_Real XMax, const Standard_Real YMax, const Standard_Real aTol, Handle(TColStd_HPackedMapOfInteger)& Nodes, Handle(TColStd_HPackedMapOfInteger)& Elements) override;

    //! Returns maps of entities (nodes and elements) detected
    //! by mouse selection with the polyline <Polyline>
    //! on the current view plane, with the tolerance aTol.
    //! Returns True if something is detected.
    //! It should be redefined if the advanced mesh selection is
    //! activated. Default implementation returns False.
    virtual Standard_Boolean GetDetectedEntities (const Handle(MeshVS_Mesh)& Prs, const TColgp_Array1OfPnt2d& Polyline, const Bnd_Box2d& aBox, const Standard_Real aTol, Handle(TColStd_HPackedMapOfInteger)& Nodes, Handle(TColStd_HPackedMapOfInteger)& Elements) override;

    //! Filter out the maps of mesh entities so as to keep
    //! only the entities that are allowed to be selected
    //! according to the current context.
    //! Returns True if any of the maps has been changed.
    //! It should be redefined if the advanced mesh selection is
    //! activated. Default implementation returns False.
    virtual Standard_Boolean GetDetectedEntities (const Handle(MeshVS_Mesh)& Prs, Handle(TColStd_HPackedMapOfInteger)& Nodes, Handle(TColStd_HPackedMapOfInteger)& Elements) override;
};

#endif // MYDATASOURCE_H
