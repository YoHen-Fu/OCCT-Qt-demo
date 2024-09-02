#ifndef MYDATASOURCE_H
#define MYDATASOURCE_H

#include <MeshVS_DataSource.hxx>
#include <MeshVS_DataSource3D.hxx>
#include <TColStd_HArray1OfInteger.hxx>
#include <TColStd_HArray2OfReal.hxx>
#include <TColStd_HArray2OfInteger.hxx>

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

class MyDataSource : public MeshVS_DataSource3D
{
public:
    const ElementData* GetElementData(const Standard_Integer ID) const;
    const NodeData* GetNodeData(const Standard_Integer ID) const;
    const GroupData* GetGroupData(const Standard_Integer ID) const;
    const std::vector<NodeData>& GetAllNodeData() const;
    const std::vector<ElementData>& GetAllElementData() const;
    const std::vector<GroupData>& GetAllGroupData() const;
public:
    MyDataSource();

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

#endif // MYDATASOURCE_H
