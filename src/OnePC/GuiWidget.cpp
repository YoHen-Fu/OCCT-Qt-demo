#include "GuiWidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QPropertyAnimation>

// 临时
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <AIS_ColoredShape.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include "Settings.h"

#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>
#include "Model.h"

// 临时
#include <V3d_DirectionalLight.hxx>
#include <V3d_DirectionalLight.hxx>
#include <V3d_DirectionalLight.hxx>


GuiWidget::GuiWidget(QWidget *parent)
    : QWidget(parent),
    m_dpiScale(Settings::instance()->getdpiScale())
{
    Handle(Aspect_DisplayConnection) hAspect_DisplayConnect = new Aspect_DisplayConnection();
    //创建3D接口定义图形驱动
    Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(hAspect_DisplayConnect);
    viewer = new V3d_Viewer(driver);
    context = new AIS_InteractiveContext(viewer);
    view = viewer->CreateView();


    viewer->SetComputedMode(Standard_True);
    viewer->SetDefaultComputedMode(Standard_True);
    viewer->SetDefaultLights();
    viewer->SetLightOn();

    view->TriedronDisplay(
        Aspect_TOTP_LEFT_LOWER,
        Quantity_NOC_GRAY50,
        0.25,
        V3d_WIREFRAME);
    view->SetBgGradientColors(Quantity_NOC_GRAY, Quantity_NOC_WHITE, Aspect_GFM_VER); // 设置背景渐变色

    context->SetDisplayMode(AIS_Shaded, Standard_True);

    m_qtOccView = new OccViewWidget(view, this);
    auto layout = new QVBoxLayout;
    layout->addWidget(m_qtOccView);
    this->setLayout(layout);
    // view->MustBeResized();
    // view->Redraw();

    m_cameraAnimation = new V3dViewCameraAnimation(view, this);
    Handle_Graphic3d_Camera m_cameraStart = new Graphic3d_Camera;
    Handle_Graphic3d_Camera m_cameraEnd = new Graphic3d_Camera;
    QPropertyAnimation* V3dViewCameraAnimation = new QPropertyAnimation(this);
    model = new Model(context);
}

GuiWidget::~GuiWidget(){}

void GuiWidget::paintEvent(QPaintEvent* event)
{
    view->Redraw();
}


void GuiWidget::FitView(){
    m_cameraAnimation->configure(&V3dView_fitAll);
    m_cameraAnimation->start(QAbstractAnimation::KeepWhenStopped);
}
void GuiWidget::FrontView(){
    m_cameraAnimation->configure([=](Handle_V3d_View view) {
        view->SetProj(V3d_Yneg);
        V3dView_fitAll(view);
    });
    m_cameraAnimation->start(QAbstractAnimation::KeepWhenStopped);
}
void GuiWidget::LeftView(){
    m_cameraAnimation->configure([=](Handle_V3d_View view) {
        view->SetProj(V3d_Xneg);
        V3dView_fitAll(view);
    });
    m_cameraAnimation->start(QAbstractAnimation::KeepWhenStopped);
}
void GuiWidget::TopView(){
    m_cameraAnimation->configure([=](Handle_V3d_View view) {
        view->SetProj(V3d_Zpos);
        V3dView_fitAll(view);
    });
    m_cameraAnimation->start(QAbstractAnimation::KeepWhenStopped);
}
void GuiWidget::IsometricView(){
    {
        m_cameraAnimation->configure([=](Handle_V3d_View view) {
            view->SetProj(V3d_XposYnegZpos);
            V3dView_fitAll(view);
        });
        m_cameraAnimation->start(QAbstractAnimation::KeepWhenStopped);
    }
}

Model& GuiWidget::getModel(){
    return *model;
}



void GuiWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (view) {
        view->MustBeResized();
        view->Redraw();
    }
}


QPaintEngine* GuiWidget::paintEngine() const
{
    return 0;
}
void GuiWidget::mousePressEvent(QMouseEvent* event)
{
    //平移 鼠标右键+shift
    if ((event->buttons() & Qt::RightButton))//&& (QApplication::keyboardModifiers() == Qt::ShiftModifier))
    {
        m_mode = Action3d_Panning;
        m_x = event->pos().x();
        m_y = event->pos().y();
    }
    else if (event->buttons() & Qt::MiddleButton)//旋转
    {
        m_mode = Action3d_Rotation;
        //开始旋转视图围绕屏幕轴
        view->StartRotation(m_dpiScale*event->pos().x(), m_dpiScale*event->pos().y());
    }
}
void GuiWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_mode = Action3d_Nothing;
}

void GuiWidget::mouseMoveEvent(QMouseEvent* event)
{
    switch (m_mode)
    {
    case GuiWidget::Action3d_Panning:
        view->Pan(m_dpiScale*(event->pos().x() - m_x), m_dpiScale*(m_y - event->pos().y()));
        m_x = event->pos().x();
        m_y = event->pos().y();
        break;
    case GuiWidget::Action3d_Rotation:
        view->Rotation(m_dpiScale*event->pos().x(), m_dpiScale*event->pos().y());
        break;
    default:
        break;
    }
}
//缩放
//Zoom方法通过相邻两侧鼠标位置来判断缩小或者放大，以及缩放程度。鼠标滚轮event->angleDelta().y()会返回数值，以此模拟第二次鼠标位置
#include <Aspect_ScrollDelta.hxx>
void GuiWidget::wheelEvent(QWheelEvent* event)
{

    // 根据鼠标滚轮方向进行缩放
    double aFactor = 5;

    double aX = event->position().x();
    double aY = event->position().y();

    if (event->angleDelta().y() > 0)
    {
        aX += aFactor;
        aY += aFactor;
    }
    else
    {
        aX -= aFactor;
        aY -= aFactor;
    }
    view->StartZoomAtPoint(m_dpiScale*event->position().x(), m_dpiScale*event->position().y());
    view->ZoomAtPoint(event->position().x(), event->position().y(), aX, aY);
}




V3dViewCameraAnimation::V3dViewCameraAnimation(const Handle_V3d_View& view, QObject* parent)
    : QAbstractAnimation(parent),
    m_view(view),
    m_cameraStart(new Graphic3d_Camera),
    m_cameraEnd(new Graphic3d_Camera)
{
}

int V3dViewCameraAnimation::duration() const
{
    return m_duration_ms;
}

void V3dViewCameraAnimation::setDuration(int msecs)
{
    m_duration_ms = msecs;
}

void V3dViewCameraAnimation::setCameraStart(const Handle_Graphic3d_Camera& camera)
{
    m_cameraStart->Copy(camera);
}

void V3dViewCameraAnimation::setCameraEnd(const Handle_Graphic3d_Camera& camera)
{
    m_cameraEnd->Copy(camera);
}

const QEasingCurve& V3dViewCameraAnimation::easingCurve() const
{
    return m_easingCurve;
}

void V3dViewCameraAnimation::setEasingCurve(const QEasingCurve& easingCurve)
{
    m_easingCurve = easingCurve;
}

void V3dViewCameraAnimation::configure(const std::function<void(Handle_V3d_View)>& fnViewChange)
{
    if (this->state() == QAbstractAnimation::Running)
        this->stop();

    const bool wasImmediateUpdateOn = m_view->SetImmediateUpdate(false);
    m_cameraStart->Copy(m_view->Camera());
    fnViewChange(m_view);
    m_cameraEnd->Copy(m_view->Camera());
    m_view->Camera()->Copy(m_cameraStart); // Restore
    m_view->SetImmediateUpdate(wasImmediateUpdateOn);
}

void V3dViewCameraAnimation::updateCurrentTime(int currentTime)
{
    const double t = m_easingCurve.valueForProgress(currentTime / double(m_duration_ms));
    const bool prevImmediateUpdate = m_view->SetImmediateUpdate(false);
    const Graphic3d_CameraLerp cameraLerp(m_cameraStart, m_cameraEnd);
    Handle_Graphic3d_Camera camera = m_view->Camera();
    cameraLerp.Interpolate(t, camera);
    m_view->SetCamera(camera);
    m_view->ZFitAll();
    m_view->SetImmediateUpdate(prevImmediateUpdate);
    m_view->Update();
}


void V3dView_fitAll(const Handle_V3d_View& view)
{
    view->ZFitAll();
    view->FitAll(0.01, false);
}

