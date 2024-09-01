// #include <QApplication>
// #include <AIS_InteractiveContext.hxx>
// #include <MeshVS_Mesh.hxx>
// #include <MeshVS_DataSource.hxx>
// #include <MeshVS_MeshPrsBuilder.hxx>
// #include <V3d_View.hxx>
// #include <WNT_Window.hxx>
// #include <OpenGl_GraphicDriver.hxx>
// #include <Aspect_Handle.hxx>
// #include <vector>

// // 定义网格节点结构
// struct GridNode {
//     float x, y, z;
// };

// // 定义四面体结构
// struct Tetrahedron {
//     int v1, v2, v3, v4; // 顶点索引
// };

// // 自定义数据源类
// class MyDataSource : public MeshVS_DataSource {
// public:
//     MyDataSource(const std::vector<GridNode>& nodes, const std::vector<Tetrahedron>& tetrahedra)
//         : myNodes(nodes), myTetrahedra(tetrahedra) {}

//     virtual Standard_Integer GetNodesNumber() const override {
//         return myNodes.size();
//     }

//     virtual gp_Pnt GetNodePoint(const Standard_Integer ID) const override {
//         const GridNode& node = myNodes[ID - 1];
//         return gp_Pnt(node.x, node.y, node.z);
//     }

//     virtual Standard_Integer GetAllElementsNumber() const override {
//         return myTetrahedra.size();
//     }

//     virtual void GetElementNodes(const Standard_Integer ID, TColStd_Array1OfInteger& theNodes) const override {
//         const Tetrahedron& tetra = myTetrahedra[ID - 1];
//         theNodes(1) = tetra.v1 + 1;
//         theNodes(2) = tetra.v2 + 1;
//         theNodes(3) = tetra.v3 + 1;
//         theNodes(4) = tetra.v4 + 1;
//     }

//     virtual MeshVS_EntityType GetElementType(const Standard_Integer ID) const override {
//         return MeshVS_ET_Volume;
//     }

// private:
//     std::vector<GridNode> myNodes;
//     std::vector<Tetrahedron> myTetrahedra;
// };

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);

//     // 初始化OCCT应用程序
//     Handle(Aspect_DisplayConnection) aDisplayConnection = new Aspect_DisplayConnection();
//     Handle(OpenGl_GraphicDriver) aGraphicDriver = new OpenGl_GraphicDriver(aDisplayConnection);
//     Handle(V3d_Viewer) aViewer = new V3d_Viewer(aGraphicDriver);
//     Handle(AIS_InteractiveContext) anInteractiveContext = new AIS_InteractiveContext(aViewer);

//     // 创建一个窗口
//     WNT_Window* aWindow = new WNT_Window("OCCT in Qt", 100, 100, 800, 600);
//     Handle(V3d_View) aView = aViewer->CreateView();
//     aView->SetWindow(aWindow);

//     // 示例网格数据
//     std::vector<GridNode> nodes = {
//         {0, 0, 0},
//         {100, 0, 0},
//         {0, 100, 0},
//         {0, 0, 100}
//     };

//     std::vector<Tetrahedron> tetrahedra = {
//         {0, 1, 2, 3}
//     };

//     // 创建自定义数据源
//     Handle(MyDataSource) aDataSource = new MyDataSource(nodes, tetrahedra);

//     // 创建MeshVS_Mesh对象
//     Handle(MeshVS_Mesh) aMesh = new MeshVS_Mesh();
//     aMesh->SetDataSource(aDataSource);

//     // 创建MeshVS_MeshPrsBuilder对象
//     Handle(MeshVS_MeshPrsBuilder) aBuilder = new MeshVS_MeshPrsBuilder(aMesh);
//     aMesh->AddBuilder(aBuilder, Standard_True);

//     // 将MeshVS_Mesh添加到交互上下文中
//     anInteractiveContext->Display(aMesh, Standard_True);

//     // 设置视图
//     aView->MustBeResized();
//     aView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.08, V3d_ZBUFFER);
//     aView->SetBackgroundColor(Quantity_NOC_BLACK);
//     aView->FitAll();

//     // 显示窗口
//     aWindow->DoEvents();

//     return a.exec();
// }
