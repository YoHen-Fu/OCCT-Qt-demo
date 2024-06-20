#pragma once

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

#include <QWidget>

#include <QAbstractAnimation>
#include <QEasingCurve>
#include <QGraphicsView>


#include "OcctWindow.h"
#include "OccViewWidget.h"
#include "Model.h"


class V3dViewCameraAnimation;

class GuiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GuiWidget(QWidget *parent = nullptr);
    ~GuiWidget();

public slots:
    void FitView();
    void FrontView();
    void LeftView();
    void TopView();
    void IsometricView();
    Model& getModel();

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

    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;



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
    // OcctWindow* window;
    OccViewWidget* m_qtOccView = nullptr;


    Standard_Integer m_x;//记录鼠标平移坐标X
    Standard_Integer m_y;//记录鼠标平移坐标y
    Standard_Integer m_z;//记录鼠标平移坐标z
    Action3d m_mode;//平移、缩放、旋转

    V3dViewCameraAnimation* m_cameraAnimation = nullptr;

    BRep_Builder builder;

    // 创建复合对象
    TopoDS_Compound compound;
    Handle_AIS_Shape selectedLine;
    double m_dpiScale;
    Model* model;

signals:
};

void V3dView_fitAll(const Handle_V3d_View& view);
class V3dViewCameraAnimation : public QAbstractAnimation {
public:
    V3dViewCameraAnimation(const Handle_V3d_View& view, QObject* parent = nullptr);

    int duration() const override;
    void setDuration(int msecs);

    void setCameraStart(const Handle_Graphic3d_Camera& camera);
    void setCameraEnd(const Handle_Graphic3d_Camera& camera);

    const QEasingCurve& easingCurve() const;
    void setEasingCurve(const QEasingCurve& easingCurve);

    void configure(const std::function<void(Handle_V3d_View)>& fnViewChange);


protected:
    void updateCurrentTime(int currentTime) override;

private:
    Handle_V3d_View m_view;
    Handle_Graphic3d_Camera m_cameraStart;
    Handle_Graphic3d_Camera m_cameraEnd;
    QEasingCurve m_easingCurve; // Linear by default
    int m_duration_ms = 200;
};


