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
#include "mydatasource3D.h"
#include "mydatasource.h"


//根据数据最大值最小值映射到颜色红到蓝，转换网格点对应颜色
MeshVS_DataMapOfIntegerColor getMeshDataMap(std::vector<double> tt, double max, double min)
{
    double a, r, g, b = 0;
    MeshVS_DataMapOfIntegerColor colormap;
    int index = 0;
    for (double t : tt)
    {
        a = (t - min) / (max - min);
        r = a;
        b = 1 - a;
        g = 1 - ((r - b) < 0 ? b - r : r - b);
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
    // 绘制立方体
    TopoDS_Shape box = 	BRepPrimAPI_MakeBox(100, 100, 100);
    Handle(AIS_Shape) box_AIS = new AIS_Shape(box);
    // m_context->Display(box_AIS, Standard_True);

    // Handle(MeshVS_Mesh) aMesh = new MeshVS_Mesh();
    // MyDataSource3D* aDataSource = new MyDataSource3D();
    // aMesh->SetDataSource(aDataSource);

    // // 创建MeshVS_MeshPrsBuilder
    // Handle(MeshVS_MeshPrsBuilder) aBuilder = new MeshVS_MeshPrsBuilder(aMesh);
    // aMesh->AddBuilder(aBuilder, Standard_True);

    // aMesh->GetDrawer()->SetColor(MeshVS_DA_EdgeColor,Quantity_NOC_BLACK);//线颜色
    // // aMesh->GetDrawer()->SetColor(MeshVS_DA_VectorColor,Quantity_NOC_BLUE);//点颜色
    // // aMesh->GetDrawer()->SetColor(MeshVS_DA_InteriorColor, Quantity_NOC_BLUE);//体颜色
    // aMesh->GetDrawer()->SetBoolean(MeshVS_DA_ShowEdges, Standard_True);//显示线

    // aMesh->GetDrawer()->SetBoolean(MeshVS_DA_DisplayNodes, Standard_True);//显示点
    // // // aMesh->SetDisplayMode(MeshVS_DMF_Shading);//显示模式
    // // // aMesh->SetMeshSelMethod(MeshVS_MSM_PRECISE);//选择模式

    // // MeshVS_DisplayModeFlags displayMode =  MeshVS_DMF_Shading;
    // // aMesh->SetDisplayMode(displayMode);

    // // std::vector<double> tt{0, 10, 1, 1};
    // // MeshVS_DataMapOfIntegerColor colormap = getMeshDataMap(tt, 10, 0);
    // // Handle(MeshVS_NodalColorPrsBuilder) nodal = new MeshVS_NodalColorPrsBuilder(aMesh, MeshVS_DMF_NodalColorDataPrs | MeshVS_DMF_OCCMask);
    // // nodal->SetColors(colormap);
    // // // mesh->setDistRadio(distRadio);
    // // aMesh->AddBuilder(nodal);

    Handle(MeshVS_Mesh) aMesh = new MeshVS_Mesh();
    // MyDataSource* aDataSource = new MyDataSource();
    MyDataSource3D* aDataSource = new MyDataSource3D();
    aMesh->SetDataSource(aDataSource);

    // 创建MeshVS_MeshPrsBuilder
    Handle(MeshVS_ElementalColorPrsBuilder) aBuilder = new MeshVS_ElementalColorPrsBuilder(aMesh, MeshVS_DMF_ElementalColorDataPrs| MeshVS_DMF_OCCMask);
    aBuilder->SetColor1(1, Quantity_NOC_RED);
    // Handle(MeshVS_MeshPrsBuilder) aBuilder = new MeshVS_MeshPrsBuilder(aMesh);
    aMesh->AddBuilder(aBuilder, Standard_True);

    // aMesh->GetDrawer()->SetColor(MeshVS_DA_EdgeColor,Quantity_NOC_BLACK);//线颜色
    // aMesh->GetDrawer()->SetColor(MeshVS_DA_InteriorColor, Quantity_NOC_BLACK);


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

