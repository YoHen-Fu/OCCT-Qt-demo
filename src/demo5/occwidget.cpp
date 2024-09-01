#include "occwidget.h"
#include "occt_window.h"

#include <QMouseEvent>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>

// 临时头文件
#include <MeshVS_Mesh.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>
#include <MeshVS_Drawer.hxx>
#include <MeshVS_DrawerAttribute.hxx>
#include "mydatasource.h"

OCCWidget::OCCWidget(QWidget *parent)
    : QWidget{parent}
    , m_dpiScale{this->devicePixelRatioF()}
{
    // 直接绘制在屏幕上
    this->setAttribute(Qt::WA_PaintOnScreen);
    // 创建连接显示设备
    Handle(Aspect_DisplayConnection) m_Aspect_DisplayConnect = new Aspect_DisplayConnection();
    // 创建3D接口定义图形驱动
    Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(m_Aspect_DisplayConnect);
    // 创建3D查看器对象，并指定图形驱动
    m_viewer = new V3d_Viewer(driver);
    // 创建交互上下文对象，关联到3D查看器
    m_context = new AIS_InteractiveContext(m_viewer);
    // 创建视图，并关联到3D查看器
    m_view = m_viewer->CreateView();
    // 获取窗口句柄并创建OCCT_Window
    OCCT_Window* wind = new OCCT_Window(this);
    // 设置视图窗口
    m_view->SetWindow(wind);
    if (!wind->IsMapped()) wind->Map();
    // 绘制立方体
    TopoDS_Shape box = 	BRepPrimAPI_MakeBox(100, 100, 100);
    Handle(AIS_Shape) box_AIS = new AIS_Shape(box);
    // m_context->Display(box_AIS, Standard_True);

    Handle(MeshVS_Mesh) aMesh = new MeshVS_Mesh();
    MyDataSource* aDataSource = new MyDataSource();
    aMesh->SetDataSource(aDataSource);

    // 创建MeshVS_MeshPrsBuilder
    Handle(MeshVS_MeshPrsBuilder) aBuilder = new MeshVS_MeshPrsBuilder(aMesh);
    aMesh->AddBuilder(aBuilder, Standard_True);

    // 创建Drawer并设置显示样式
    Handle(MeshVS_Drawer) aDrawer = aMesh->GetDrawer();
    if (!aDrawer.IsNull())
    {
        // 设置网格面颜色
        Quantity_Color aFaceColor(Quantity_NOC_GREEN);
        aDrawer->SetColor(MeshVS_DA_InteriorColor, aFaceColor);

        // 设置网格线宽
        aDrawer->SetDouble(MeshVS_DA_EdgeWidth, 2.0);

        // 设置网格透明度
        // aDrawer->SetDouble(MeshVS_DA_Transparency, 0.5);

        // 设置网格线颜色
        Quantity_Color aLineColor(Quantity_NOC_RED);
        aDrawer->SetColor(MeshVS_DA_EdgeColor, aLineColor);

        aDrawer->SetBoolean(MeshVS_DA_DisplayNodes, Standard_False);

        // 应用Drawer到MeshVS_Mesh
        aMesh->SetDrawer(aDrawer);
    }

    // 将Mesh添加到AIS_InteractiveContext
    m_context->Display(aMesh, Standard_True);

}

QPaintEngine* OCCWidget::paintEngine() const
{
    return nullptr;
}

void OCCWidget::paintEvent( QPaintEvent* /*theEvent*/ )
{
    m_view->Redraw();
}

void OCCWidget::resizeEvent( QResizeEvent* /*theEvent*/ )
{
    if( !m_view.IsNull() )
    {
        m_view->MustBeResized();
    }
}

void OCCWidget::mousePressEvent(QMouseEvent* event){
    //平移 鼠标右键
    if ((event->buttons() & Qt::RightButton))
    {
        m_mode = Action3d_Panning;
        m_x = event->pos().x();
        m_y = event->pos().y();
    }
    //旋转 鼠标中键
    else if (event->buttons() & Qt::MiddleButton)
    {
        m_mode = Action3d_Rotation;
        m_view->StartRotation(m_dpiScale*event->pos().x(), m_dpiScale*event->pos().y());
    }
}

void OCCWidget::mouseReleaseEvent(QMouseEvent* event){
    m_mode = Action3d_Nothing;
}

void OCCWidget::mouseMoveEvent(QMouseEvent* event){
    switch (m_mode)
    {
    case OCCWidget::Action3d_Panning:
        m_view->Pan(m_dpiScale*(event->pos().x() - m_x), m_dpiScale*(m_y - event->pos().y()));
        m_x = event->pos().x();
        m_y = event->pos().y();
        break;
    case OCCWidget::Action3d_Rotation:
        m_view->Rotation(m_dpiScale*event->pos().x(), m_dpiScale*event->pos().y());
        break;
    default:
        break;
    }
};
void OCCWidget::wheelEvent(QWheelEvent* event){
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
    m_view->StartZoomAtPoint(m_dpiScale*event->position().x(), m_dpiScale*event->position().y());
    m_view->ZoomAtPoint(event->position().x(), event->position().y(), aX, aY);
};

