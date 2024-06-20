#ifndef MODEL_H
#define MODEL_H

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <TopoDS_Compound.hxx>

#include <QDebug>
#include <map>

#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <Geom_Circle.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <AIS_ColoredShape.hxx>
#include <Prs3d_LineAspect.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

// 测试
#include <AIS_Animation.hxx>
#include <AIS_AnimationObject.hxx>
#include <StdPrs_BRepTextBuilder.hxx>
#include <StdPrs_BRepFont.hxx>
#include <NCollection_String.hxx>
#include <AIS_Trihedron.hxx>  //显示坐标轴
#include <Geom_Axis2Placement.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx> //绘制圆柱体
#include <AIS_AnimationAxisRotation.hxx>

// 临时
#include <QWidget>


class Model: public QWidget
{
    Q_OBJECT
public:
    Model(Handle_AIS_InteractiveContext context_);
    void test();
    void BuildingPC();
    void ShowPC();
    void HidePC();
    void ShowFrame();
    void HideFrame();
    void OnePCOpen();
    void OnePCClose();
public:
    TopoDS_Shape OnePCLogo(const Standard_Real Size);
    TopoDS_Shape rounded_cube(gp_Vec centerpos, Standard_Real length, Standard_Real width, Standard_Real height, Standard_Real radius);
    TopoDS_Compound linear_duplicate(TopoDS_Shape& shape, Standard_Integer numX, Standard_Integer numY, Standard_Real xSpacing, Standard_Real ySpacing);
    TopoDS_Shape slide_shape(gp_Pnt centerpos, Standard_Real radius, Standard_Real height);
    TopoDS_Shape make_connection_BD(gp_Vec centerpos, Standard_Real length, Standard_Real width, Standard_Real height);
public:
    std::map<std::string, Handle(AIS_Shape)> getOnePC_AIS();
private:
    Standard_Integer show_status{-1};
    Standard_Integer frame_status{1};
    Standard_Integer open_status{1};
    Handle_AIS_InteractiveContext context;
    TopoDS_Compound  display_compound;
    BRep_Builder OnePC_builder;
    TopoDS_Compound  OnePC_compound_shape;
    Handle(AIS_Shape) OnePC_compound_AIS;
    Handle_AIS_Shape OnePC_compound_frame;
    std::map<std::string, Handle(AIS_Shape)> OnePC_AIS_compound;
    std::map<std::string, Handle(AIS_Shape)> OnePC_FR_AIS_compound;
    std::map<Handle(AIS_Shape), std::string> OnePC_AIS_compound_SW;
    std::map<Handle(AIS_Shape), std::string> OnePC_FR_AIS_compound_SW;
    // 旋转部件
    Handle_AIS_Shape connection_BD_AIS;
    Handle_AIS_Shape base_display_AIS;
    Handle_AIS_Shape display_AIS;
    Handle_AIS_Shape connection_BD_FR_AIS;
    Handle_AIS_Shape base_display_FR_AIS;
    Handle_AIS_Shape display_FR_AIS;
};

#endif // MODEL_H
