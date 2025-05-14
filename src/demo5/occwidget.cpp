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
#include <MeshVS_NodalColorPrsBuilder.hxx>
#include <MeshVS_ElementalColorPrsBuilder.hxx>
#include "mydatasource.h"


//根据数据最大值最小值映射到颜色红到蓝，转换网格点对应颜色
MeshVS_DataMapOfIntegerColor getMeshDataMap(std::vector<double> tt, double max, double min)
{
    double a, r, g, b = 0;
    MeshVS_DataMapOfIntegerColor colormap;
    int index = 0;
    for (double t : tt)
    {
        // 归一化到[0,1]范围
        double normalized = (t - min) / (max - min);

        if (normalized < 0.2) {
            // 紫色到蓝色
            r = (128 - 128 * (normalized / 0.2))/255;
            g = 0;
            b = 1;
        } else if (normalized < 0.4) {
            // 蓝色到青色
            r = 0;
            g = (255 * ((normalized - 0.2) / 0.2))/255;
            b = 1;
        } else if (normalized < 0.6) {
            // 青色到绿色
            r = 0;
            g = 1;
            b = (255 - 255 * ((normalized - 0.4) / 0.2))/255;
        } else if (normalized < 0.8) {
            // 绿色到黄色
            r = (255 * ((normalized - 0.6) / 0.2))/255;
            g = 1;
            b = 0;
        } else {
            // 黄色到红色
            r = 1;
            g = (255 - 255 * ((normalized - 0.8) / 0.2))/255;
            b = 0;
        }
        colormap.Bind(index + 1, Quantity_Color(r, g, b, Quantity_TOC_RGB));
        index++;
    }
    return colormap;
}

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
    m_view->SetBgGradientColors(Quantity_NOC_GRAY, Quantity_NOC_WHITE, Aspect_GFM_VER);
    // 获取窗口句柄并创建OCCT_Window
    OCCT_Window* wind = new OCCT_Window(this);
    // 设置视图窗口
    m_view->SetWindow(wind);
    if (!wind->IsMapped()) wind->Map();

    MyDataSource* aDataSource = new MyDataSource();
    Handle(MeshVS_Mesh) aMesh = new MeshVS_Mesh();
    aMesh->SetDataSource(aDataSource);

    // aMesh->GetDrawer()->SetColor(MeshVS_DA_EdgeColor,Quantity_NOC_BLACK);//线颜色
    // aMesh->GetDrawer()->SetColor(MeshVS_DA_VectorColor,Quantity_NOC_BLUE);//点颜色
    // aMesh->GetDrawer()->SetColor(MeshVS_DA_InteriorColor, Quantity_NOC_BLUE);//体颜色
    // aMesh->GetDrawer()->SetBoolean(MeshVS_DA_ShowEdges, Standard_False);//隐藏边线
    // aMesh->GetDrawer()->SetBoolean(MeshVS_DA_DisplayNodes, Standard_True);//显示点


    std::vector<double> tt{50, 255, 125};
    MeshVS_DataMapOfIntegerColor colormap = getMeshDataMap(tt, 255, 0);
    Handle(MeshVS_NodalColorPrsBuilder) nodal = new MeshVS_NodalColorPrsBuilder(aMesh, MeshVS_DMF_NodalColorDataPrs | MeshVS_DMF_OCCMask);
    nodal->SetColors(colormap);
    aMesh->AddBuilder(nodal);

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

