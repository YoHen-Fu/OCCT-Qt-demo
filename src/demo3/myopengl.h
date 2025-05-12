#ifndef STUDY_H
#define STUDY_H

#include <QMainWindow>
#include <Standard_Handle.hxx>
#include <V3d_Viewer.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <WNT_Window.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDs_Shape.hxx>
#include <AIS_Shape.hxx>

#include <qevent.h>

#include "OCC.h"
#include "meshdatasource.h"
#include "MeshVS_DataMapOfIntegerColor.hxx"

class QtWidgetsApplication1 : public QWidget
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget* parent = nullptr);
    ~QtWidgetsApplication1();
protected:
    // //重写绘图事件
    void paintEvent(QPaintEvent* event) override;
    //返回窗口绘图引擎
    QPaintEngine* paintEngine() const;

    enum Action3d
    {
        Action3d_Nothing,
        Action3d_Panning,//平移
        Action3d_Zooming,//缩放
        Action3d_Rotation//旋转
    };

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    MeshVS_DataMapOfIntegerColor getMeshDataMap(std::vector<double> tt, double max, double min);
    Handle(MeshVS_Mesh) CreateMesh(std::vector<PointXYZ> coordData, std::vector<Element> ele2NodeData);


private:
    //Ui::QtWidgetsApplication1Class  ui;
    //定义查看器viewer 3D查看器
    Handle(V3d_Viewer) viewer;
    //视图
    Handle(V3d_View) view;
    //交互式上下文，管理一个或者多个viewer
    Handle(AIS_InteractiveContext) context;
    //window NT窗口
    Handle(WNT_Window) window;


    Standard_Integer m_x;//记录鼠标平移坐标X
    Standard_Integer m_y;//记录鼠标平移坐标y
    Action3d m_mode;//平移、缩放、旋转

    OCC occ;

};

#endif // STUDY_H
