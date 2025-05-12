#include "myopengl.h"
#include <QDebug>
#include "AIS_ColorScale.hxx"
#include "AIS_Manipulator.hxx"

#include "MeshVS_Mesh.hxx"
#include <MeshVS_Drawer.hxx>
#include "MeshVS_BuilderPriority.hxx"
#include "MeshVS_NodalColorPrsBuilder.hxx"
#include "MeshVS_DrawerAttribute.hxx"

QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
    : QWidget(parent)
{
    resize(800, 600);

    //提供X server的连接，在window和Mac中不起作用
    Handle(Aspect_DisplayConnection) hAspect_DisplayConnect = new Aspect_DisplayConnection;
    //创建3D接口定义图形驱动
    Handle(OpenGl_GraphicDriver) driver = new OpenGl_GraphicDriver(hAspect_DisplayConnect);

    //该类的方法允许编辑、询问连接该类的其他参数（如视图、光）
    viewer = new V3d_Viewer(driver);
    view = viewer->CreateView();

    WId win_handle = winId();
    qDebug()<< win_handle;
    //在已有的窗口上创建窗口
    window = new WNT_Window((Aspect_Handle)win_handle);
    view->SetWindow(window);
    if (!window->IsMapped())
    {
        window->Map();//打开窗口
    }

    view->SetBackgroundColor(Quantity_NOC_WHITE);
    view->MustBeResized();
    viewer->SetDefaultLights();
    setAttribute(Qt::WA_PaintOnScreen);
    //交互式上下文

    context = new AIS_InteractiveContext(viewer);
    context->SetDisplayMode(AIS_Shaded, Standard_True);


    TopoDS_Shape boxShape = occ.MakeBottle(50, 70, 30);

    Quantity_Color color = Quantity_Color(0.3, 0.5, 0.3, Quantity_TOC_RGB);



    Handle(AIS_Shape) abox = new AIS_Shape(boxShape);


    context->SetMaterial(abox, Graphic3d_NameOfMaterial_Stone, Standard_False);
    context->SetColor(abox, color, Standard_False);

    // context->Display(abox,Standard_True);
    std::vector<PointXYZ> coord;
    coord.push_back(PointXYZ(0, 0, 0, 0));
    coord.push_back(PointXYZ(1, 10, 0, 0));
    coord.push_back(PointXYZ(2, 0, 10, 0));
    std::vector<Element> ele;
    ele.push_back(Element(0, 1, 2));
    Handle(MeshVS_Mesh) m_mesh = CreateMesh(coord, ele);

    std::vector<double> c = {10,20,0};
    MeshVS_DataMapOfIntegerColor colormap = getMeshDataMap(c, 20, 0);
    Handle(MeshVS_NodalColorPrsBuilder) nodal = new MeshVS_NodalColorPrsBuilder(m_mesh, MeshVS_DMF_NodalColorDataPrs | MeshVS_DMF_OCCMask);
    nodal->SetColors(colormap);
    // m_mesh->setDistRadio(distRadio);
    m_mesh->AddBuilder(nodal);
    context->Display(m_mesh, Standard_True);


    Handle(AIS_ColorScale) aColorScale = new AIS_ColorScale();
    // aColorScale->SetPosition(100,100); // 设置位置
    aColorScale->SetSize(100, 400); // 设置大小
    aColorScale->SetRange(0, 20); // 设置颜色范围
    aColorScale->SetNumberOfIntervals(9);
    aColorScale->SetLabelPosition(Aspect_TOCSP_RIGHT);
    aColorScale->SetTextHeight(20);
    aColorScale->SetColor(Quantity_Color(Quantity_NOC_BLACK));
    aColorScale->SetSmoothTransition(Standard_True);//颜色平衡过度
    aColorScale->SetZLayer (Graphic3d_ZLayerId_TopOSD);
    aColorScale->SetLabelType(Aspect_TOCSD_AUTO);
    aColorScale->SetZLayer(Graphic3d_ZLayerId_TopOSD);

    Graphic3d_Vec2i newPosition(0, Standard_Integer(0));
    Handle(Graphic3d_TransformPers) aTrsfPers = new Graphic3d_TransformPers(Graphic3d_TMF_2d, Aspect_TOTP_LEFT, newPosition);
    aColorScale->SetTransformPersistence(aTrsfPers);//显示位置


    context->Display(aColorScale, Standard_True);
    // 使刻度轴不受变换影响

    view->SetProj(V3d_Zpos);
    view->FitAll();

}


MeshVS_DataMapOfIntegerColor QtWidgetsApplication1::getMeshDataMap(std::vector<double> tt, double max, double min)
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

QtWidgetsApplication1::~QtWidgetsApplication1(){}

void QtWidgetsApplication1::paintEvent(QPaintEvent* event)
{
    view->Redraw();
}

QPaintEngine* QtWidgetsApplication1::paintEngine() const
{
    return 0;
}
void QtWidgetsApplication1::mousePressEvent(QMouseEvent* event)
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
        view->StartRotation(event->pos().x(), event->pos().y());
    }
}
void QtWidgetsApplication1::mouseReleaseEvent(QMouseEvent* event)
{
    m_mode = Action3d_Nothing;
}

void QtWidgetsApplication1::mouseMoveEvent(QMouseEvent* event)
{
    switch (m_mode)
    {
    case QtWidgetsApplication1::Action3d_Panning:
        view->Pan(event->pos().x() - m_x, m_y - event->pos().y());
        m_x = event->pos().x();
        m_y = event->pos().y();
        break;
    case QtWidgetsApplication1::Action3d_Rotation:
        view->Rotation(event->pos().x(), event->pos().y());
        break;
    default:
        break;
    }
}
//缩放
//Zoom方法通过相邻两侧鼠标位置来判断缩小或者放大，以及缩放程度。鼠标滚轮event->angleDelta().y()会返回数值，以此模拟第二次鼠标位置
void QtWidgetsApplication1::wheelEvent(QWheelEvent* event)
{
    //view->Zoom(0, 0, event->angleDelta().y(), 0);//固定点缩放
    //跟随鼠标位置进行缩放
    view->StartZoomAtPoint(event->position().x(), event->position().y());
    view->ZoomAtPoint(0, 0, event->angleDelta().y(), 0);

}

Handle(MeshVS_Mesh) QtWidgetsApplication1::CreateMesh(std::vector<PointXYZ> coordData, std::vector<Element> ele2NodeData){

    Handle(MeshVS_Mesh) mesh = new MeshVS_Mesh();

    Handle(MeshDataSource) data = new MeshDataSource(coordData,ele2NodeData);

    mesh->SetDataSource(data);
    mesh->GetDrawer()->SetColor(MeshVS_DA_EdgeColor,Quantity_NOC_BLACK);//线颜色
    mesh->GetDrawer()->SetColor(MeshVS_DA_VectorColor,Quantity_NOC_BLACK);//点颜色
    mesh->GetDrawer()->SetColor(MeshVS_DA_InteriorColor, Quantity_NOC_GRAY);//体颜色
    mesh->GetDrawer()->SetBoolean(MeshVS_DA_ShowEdges, Standard_True);//显示线

    mesh->GetDrawer()->SetBoolean(MeshVS_DA_DisplayNodes, Standard_False);//显示点
    mesh->SetDisplayMode(MeshVS_DMF_Shading);//显示模式
    mesh->SetMeshSelMethod(MeshVS_MSM_PRECISE);//选择模式
    //mesh->SetHilightMode(MeshVS_DMF_Shading);

    return mesh;
}

