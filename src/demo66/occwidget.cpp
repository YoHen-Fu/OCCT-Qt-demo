#include "occwidget.h"
#include "occt_window.h"

#include <QMouseEvent>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include <Geom_Axis2Placement.hxx>
#include <AIS_Trihedron.hxx>


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
    m_context->Display(box_AIS, Standard_True);

    m_view->TriedronDisplay(
        Aspect_TOTP_LEFT_UPPER,
        Quantity_NOC_GRAY50,
        0.25,
        V3d_ZBUFFER);


    // // Create custom trihedron
    // Handle(AIS_Trihedron) aCustomTrihedron = new AIS_Trihedron(new Geom_Axis2Placement(gp::XOY()));

    // // Get the drawer to customize appearance
    // Handle(Prs3d_Drawer) aDrawer = aCustomTrihedron->Attributes();

    // // Set text aspects
    // Handle(Prs3d_TextAspect) aTextAspect = new Prs3d_TextAspect();
    // aTextAspect->SetHeight(16.0);  // Text height
    // aTextAspect->SetColor(Quantity_NOC_BLACK);  // Text color
    // aDrawer->SetTextAspect(aTextAspect);

    // // Set individual axis aspects
    // Handle(Prs3d_DatumAspect) aDatumAspect = new Prs3d_DatumAspect();
    // aDatumAspect->SetAxisLength(60.0, 60.0, 60.0);

    // // Set label font size (this method varies by OCC version)
    // aDatumAspect->SetDrawLabels(Standard_True);
    // aDatumAspect->SetTextAspect(aTextAspect);

    // // // Set axis colors
    // // aDatumAspect->FirstAxisAspect()->SetColor(xAxisColor);
    // // aDatumAspect->SecondAxisAspect()->SetColor(yAxisColor);
    // // aDatumAspect->ThirdAxisAspect()->SetColor(zAxisColor);

    // aDrawer->SetDatumAspect(aDatumAspect);
    // aCustomTrihedron->SetAttributes(aDrawer);

    // // Display the custom trihedron
    // m_context->Display(aCustomTrihedron, Standard_True);


    // // Set custom colors for each axis
    // Quantity_Color xAxisColor(1.0, 0.0, 0.0, Quantity_TOC_RGB);  // Red for X axis
    // Quantity_Color yAxisColor(0.0, 1.0, 0.0, Quantity_TOC_RGB);  // Green for Y axis
    // Quantity_Color zAxisColor(0.0, 0.0, 1.0, Quantity_TOC_RGB);  // Blue for Z axis

    // // Create a custom trihedron
    // Handle(AIS_Trihedron) aCustomTrihedron = new AIS_Trihedron(new Geom_Axis2Placement(gp::XOY()));

    // // Get the drawer to customize appearance
    // Handle(Prs3d_Drawer) aDrawer = aCustomTrihedron->Attributes();

    // // Set text aspects
    // Handle(Prs3d_TextAspect) aTextAspect = new Prs3d_TextAspect();
    // aTextAspect->SetHeight(16.0);  // Text height
    // aTextAspect->SetColor(Quantity_NOC_BLACK);  // Text color
    // aDrawer->SetTextAspect(aTextAspect);

    // // Set individual axis aspects
    // Handle(Prs3d_DatumAspect) aDatumAspect = new Prs3d_DatumAspect();
    // aDatumAspect->SetAxisLength(60.0, 60.0, 60.0);

    // // Set label font size
    // aDatumAspect->SetDrawLabels(Standard_True);
    // aDatumAspect->SetTextAspect(aTextAspect);

    // // // Set axis colors
    // // aDatumAspect->FirstAxisAspect()->SetColor(xAxisColor);
    // // aDatumAspect->SecondAxisAspect()->SetColor(yAxisColor);
    // // aDatumAspect->ThirdAxisAspect()->SetColor(zAxisColor);

    // aDrawer->SetDatumAspect(aDatumAspect);
    // aCustomTrihedron->SetAttributes(aDrawer);

    // // Create a 2D presentation for the trihedron
    // Handle(Graphic3d_Structure) aGraphicStructure = new Graphic3d_Structure(m_view->Viewer()->StructureManager());
    // Handle(Graphic3d_Group) aGroup = aGraphicStructure->NewGroup();

    // // Set as overlay - this is the key to keeping it fixed during panning
    // aGraphicStructure->SetTransformPersistence(new Graphic3d_TransformPers(
    //     Graphic3d_TMF_TriedronPers, Aspect_TOTP_LEFT_LOWER));

    // // Display the custom trihedron
    // m_context->Display(aCustomTrihedron, Standard_True);

    // // Set transform persistence to keep it fixed during panning
    // aCustomTrihedron->SetTransformPersistence(new Graphic3d_TransformPers(
    //     Graphic3d_TMF_TriedronPers,      // 2D persistence mode (fixed to screen)
    //     Aspect_TOTP_LEFT_LOWER, // Position in the view
    //     Graphic3d_Vec2i(20, 20))); // Offset from the corner (pixels)

    // // Set ZLayer to top to ensure it's always visible
    // aCustomTrihedron->SetZLayer(Graphic3d_ZLayerId_Top);

    // // Update the view
    // m_view->Update();








    // Set custom colors for each axis
    Quantity_Color xAxisColor(1.0, 0.0, 0.0, Quantity_TOC_RGB);  // Red for X axis
    Quantity_Color yAxisColor(0.0, 1.0, 0.0, Quantity_TOC_RGB);  // Green for Y axis
    Quantity_Color zAxisColor(0.0, 0.0, 1.0, Quantity_TOC_RGB);  // Blue for Z axis

    // Create a custom trihedron
    Handle(AIS_Trihedron) aCustomTrihedron = new AIS_Trihedron(new Geom_Axis2Placement(gp::XOY()));

    // Get the drawer to customize appearance
    Handle(Prs3d_Drawer) aDrawer = aCustomTrihedron->Attributes();

    // Set text aspects
    Handle(Prs3d_TextAspect) aTextAspect = new Prs3d_TextAspect();
    aTextAspect->SetHeight(40.0);  // Text height
    aTextAspect->SetColor(Quantity_NOC_BLACK);  // Text color
    aDrawer->SetTextAspect(aTextAspect);

    // Set individual axis aspects
    Handle(Prs3d_DatumAspect) aDatumAspect = new Prs3d_DatumAspect();
    aDatumAspect->SetAxisLength(100.0, 100.0, 100.0);

    // Set label font size
    aDatumAspect->SetDrawLabels(Standard_True);
    aDatumAspect->SetTextAspect(aTextAspect);

    // Set axis colors
    // aDatumAspect->FirstAxisAspect()->SetColor(xAxisColor);
    // aDatumAspect->SecondAxisAspect()->SetColor(yAxisColor);
    // aDatumAspect->ThirdAxisAspect()->SetColor(zAxisColor);

    aDrawer->SetDatumAspect(aDatumAspect);
    aCustomTrihedron->SetAttributes(aDrawer);

    // Display the custom trihedron
    m_context->Display(aCustomTrihedron, Standard_True);

    // Use Graphic3d_TMF_TriedronPers to keep orientation fixed
    // This is the key setting that prevents rotation with the view
    aCustomTrihedron->SetTransformPersistence(new Graphic3d_TransformPers(
        Graphic3d_TMF_TriedronPers,  // Fixed orientation mode
        Aspect_TOTP_LEFT_LOWER,      // Position in the view
        Graphic3d_Vec2i(200, 200)));     // Offset from the corner (pixels);                       // Size relative to view

    // aCustomTrihedron->SetTransformPersistence(new Graphic3d_TransformPers(
    //     Graphic3d_TMF_TriedronPers,      // 2D persistence mode (fixed to screen)
    //     Aspect_TOTP_LEFT_LOWER, // Position in the view
    //     Graphic3d_Vec2i(20, 20))); // Offset from the corner (pixels)

    // Set ZLayer to top to ensure it's always visible
    aCustomTrihedron->SetZLayer(Graphic3d_ZLayerId_Top);

    // Update the view
    m_view->Update();



    Geom_Axis2Placement* axis = new Geom_Axis2Placement(gp::XOY());
    AIS_Trihedron* aisTrihedron = new AIS_Trihedron(axis);
    aisTrihedron->SetDatumDisplayMode(Prs3d_DM_WireFrame);
    aisTrihedron->SetDrawArrows(false);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_XAxis)->SetWidth(2.5);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_YAxis)->SetWidth(2.5);
    aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_ZAxis)->SetWidth(2.5);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_XAxis, Quantity_NOC_RED2);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_YAxis, Quantity_NOC_GREEN2);
    aisTrihedron->SetDatumPartColor(Prs3d_DP_ZAxis, Quantity_NOC_BLUE2);
    aisTrihedron->SetLabel(Prs3d_DP_XAxis, "");
    aisTrihedron->SetLabel(Prs3d_DP_YAxis, "");
    aisTrihedron->SetLabel(Prs3d_DP_ZAxis, "");
    //aisTrihedron->SetTextColor(Quantity_NOC_GRAY40);
    aisTrihedron->SetSize(60);
    aisTrihedron->SetTransformPersistence(
        new Graphic3d_TransformPers(Graphic3d_TMF_ZoomPers, axis->Ax2().Location())
        );
    aisTrihedron->Attributes()->SetZLayer(Graphic3d_ZLayerId_Topmost);
    aisTrihedron->SetInfiniteState(true);
    aCustomTrihedron->SetTransformPersistence(new Graphic3d_TransformPers(
        Graphic3d_TMF_TriedronPers,  // Fixed orientation mode
        Aspect_TOTP_LEFT_LOWER,      // Position in the view
        Graphic3d_Vec2i(200, 200)));     // Offset from the corner (pixels);
    aisTrihedron->SetTransformPersistence(new Graphic3d_TransformPers(
        Graphic3d_TMF_TriedronPers,  // Fixed orientation mode
        Aspect_TOTP_LEFT_LOWER,      // Position in the view
        Graphic3d_Vec2i(200, 200)));     // Offset from the corner (pixels);
    m_context->Display(aisTrihedron, Standard_True);
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

