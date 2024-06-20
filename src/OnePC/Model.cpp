#include "Model.h"

#include <StdPrs_BRepTextBuilder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <gp_Vec.hxx>

// void Model::test(){

// }


Model::Model(Handle_AIS_InteractiveContext _context):context{_context}{


}

void Model::BuildingPC(){
    //****************************底板**********************************//
    TopoDS_Shape base_plate_tmp = rounded_cube(gp_Vec(0, 0, 0), 380, 250, 16, 14);
    BRepFilletAPI_MakeChamfer base_plate_tmp_chamfer(base_plate_tmp);
    TopExp_Explorer base_plate_tmp_ex(base_plate_tmp, TopAbs_EDGE);
    while(base_plate_tmp_ex.More()){
        base_plate_tmp_chamfer.Add(2, TopoDS::Edge(base_plate_tmp_ex.Current()));
        base_plate_tmp_ex.Next();
    }
    base_plate_tmp = base_plate_tmp_chamfer.Shape();
    // 键盘凹陷
    TopoDS_Shape keyboard_plate_rm = rounded_cube(gp_Vec(0, 27.5, 17), 350, 140, -2, 5);
    BRepFilletAPI_MakeFillet keyboard_plate_rm_fillet(keyboard_plate_rm);
    TopExp_Explorer keyboard_plate_rm_ex(keyboard_plate_rm, TopAbs_EDGE);
    while(keyboard_plate_rm_ex.More()){
        keyboard_plate_rm_fillet.Add(0.99, TopoDS::Edge(keyboard_plate_rm_ex.Current()));
        keyboard_plate_rm_ex.Next();
    }
    keyboard_plate_rm = keyboard_plate_rm_fillet.Shape();
    // context->Display(new AIS_Shape(keyboard_plate_rm), Standard_True);
    // 触控板
    TopoDS_Shape touch_tablet = rounded_cube(gp_Vec(0, -82, 16), 120, 70, -1, 10);
    // TypeC电源插口
    TopoDS_Shape typeC_left = rounded_cube(gp_Vec(-190, 90, 3), 6, 20, 10, 2.99);
    gp_Trsf rotation_typeC_left;  // 创建旋转变换
    rotation_typeC_left.SetRotation(gp_Ax1(gp_Pnt(-190, 100, 8), gp_Dir(0, 1, 0)), M_PI/2);
    typeC_left = BRepBuilderAPI_Transform(typeC_left, rotation_typeC_left).Shape();
    TopoDS_Shape typeC_right = rounded_cube(gp_Vec(190, 90, 3), 6, 20, 10, 2.99);
    gp_Trsf rotation_typeC_right;  // 创建旋转变换
    rotation_typeC_right.SetRotation(gp_Ax1(gp_Pnt(190, 100, 8), gp_Dir(0, 1, 0)), M_PI/2);
    typeC_right = BRepBuilderAPI_Transform(typeC_right, rotation_typeC_right).Shape();
    TopoDS_Compound typeC_compound;
    BRep_Builder typeC_builder;
    typeC_builder.MakeCompound(typeC_compound);
    typeC_builder.Add(typeC_compound, typeC_left);
    typeC_builder.Add(typeC_compound, typeC_right);
    // 散热口
    TopoDS_Shape thermovent_tmp = rounded_cube(gp_Vec(-97.5, 80, 0), 2, 30, 3, 0.99);
    TopoDS_Compound thermovent = linear_duplicate(thermovent_tmp, 40, 1, 5, 0);
    // 电源按钮
    gp_Pnt powerBtn_pt(150, 110, 11);
    gp_Ax2 powerBtn_ax = gp_Ax2(powerBtn_pt, gp_Dir(0, 0, 1));
    TopoDS_Shape powerBtn_rm = BRepPrimAPI_MakeCylinder(powerBtn_ax, 6, 5);
    TopoDS_Shape powerBtn = BRepPrimAPI_MakeCylinder(powerBtn_ax, 5.8, 5);
    //***************************底板与显示屏连接处*******************************//
    TopoDS_Shape connection_BD_rm = rounded_cube(gp_Vec(0, 125, 0), 200, 30, 16, 0.1);
    TopoDS_Shape connection_BD = make_connection_BD(gp_Vec(-100, 120, 8), 10, 6, 200);
    gp_Trsf rotation_connection_BD;
    rotation_connection_BD.SetRotation(gp_Ax1(gp_Pnt(0, 120, 8), gp_Dir(1, 0, 0)), -M_PI/4);  //M_PI/4
    connection_BD = BRepBuilderAPI_Transform(connection_BD, rotation_connection_BD).Shape();
    //***************************显示屏*******************************//
    TopoDS_Shape base_display_tmp = rounded_cube(gp_Vec(0, 125+125-10, -2), 380, 250, -7, 14);
    BRepFilletAPI_MakeFillet base_display_tmp_fillet(base_display_tmp);
    TopExp_Explorer base_display_tmp_ex(base_display_tmp, TopAbs_EDGE);
    while(base_display_tmp_ex.More()){
        base_display_tmp_fillet.Add(3, TopoDS::Edge(base_display_tmp_ex.Current()));
        base_display_tmp_ex.Next();
    }
    base_display_tmp = TopoDS_Shape(base_display_tmp_fillet.Shape());
    // Logo
    TopoDS_Shape Logo = OnePCLogo(24);
    // 定义镜像变换
    gp_Trsf transformation2;
    transformation2.SetMirror(gp_Ax2(gp_Pnt(0.0, 0.0, 0.0), gp_Dir(1.0, 0.0, 0.0)));  // 以Z轴为镜像面进行镜像
    // 进行变换
    BRepBuilderAPI_Transform transform2(Logo, transformation2);
    Logo = transform2.Shape();
    // 定义变换
    gp_Trsf transformation;
    transformation.SetTranslation(gp_Vec(50, 250.0, -9));  // 在X轴方向上平移10个单位
    // 进行变换
    BRepBuilderAPI_Transform transform(Logo, transformation);
    Logo = transform.Shape();
    // context->Display(new AIS_Shape(Logo), Standard_True);
    base_display_tmp =  BRepAlgoAPI_Cut(base_display_tmp, Logo).Shape();

    TopoDS_Shape display_rm = rounded_cube(gp_Vec(0, 125+125-10, -2), 375, 245, -1, 14);
    base_display_tmp = BRepAlgoAPI_Cut(base_display_tmp, display_rm).Shape();
    gp_Trsf rotation_base_display;
    rotation_base_display.SetRotation(gp_Ax1(gp_Pnt(0, 120, 8), gp_Dir(1, 0, 0)), M_PI/4);//M_PI/4
    base_display_tmp = BRepBuilderAPI_Transform(base_display_tmp, rotation_base_display).Shape();

    TopoDS_Shape display = rounded_cube(gp_Vec(0, 125+125-10, -2), 375, 245, -1, 14);
    display = BRepBuilderAPI_Transform(display, rotation_base_display).Shape();


    //***************************键盘*******************************//
    // 键盘第一行
    TopoDS_Shape keyboard_tmp_rm1 = rounded_cube(gp_Vec(-159.25, 85, 16), 22.5, 14.5, -3, 1);  //341
    TopoDS_Compound keyboard_rm1 = linear_duplicate(keyboard_tmp_rm1, 14, 1, 22.5+2, 0);

    TopoDS_Shape keyboard_tmp1 = rounded_cube(gp_Vec(-159.25, 85, 16), 22, 14, -3, 1);
    TopoDS_Compound keyboard1 = linear_duplicate(keyboard_tmp1, 14, 1, 22.5+2, 0);
    // 键盘第二行 两行垂直间隔2.5
    TopoDS_Shape keyboard_tmp_rm21 = rounded_cube(gp_Vec(-160.25, 65, 16), 20.5, 20.5, -3, 1); //`~、0~9、-、+
    TopoDS_Compound keyboard_rm21 = linear_duplicate(keyboard_tmp_rm21, 13, 1, 20.5+2, 0); //BackSpace
    TopoDS_Shape keyboard_rm22 = rounded_cube(gp_Vec(146.25, 65, 16), 48.5, 20.5, -3, 1); //

    TopoDS_Shape keyboard_tmp21 = rounded_cube(gp_Vec(-160.25, 65, 16), 20, 20, -3, 1); //`~、0~9、-、+
    TopoDS_Compound keyboard21 = linear_duplicate(keyboard_tmp21, 13, 1, 20.5+2, 0); //BackSpace
    TopoDS_Shape keyboard22 = rounded_cube(gp_Vec(146.25, 65, 16), 48, 20, -3, 1); //
    // 键盘第三行
    TopoDS_Shape keyboard_rm31 = rounded_cube(gp_Vec(-146.25, 42, 16), 48.5, 20.5, -3, 1); //Tab
    TopoDS_Shape keyboard_tmp_rm32 = rounded_cube(gp_Vec(160.25, 42, 16), 20.5, 20.5, -3, 1); //Q~P、{}、|
    TopoDS_Compound keyboard_rm32 = linear_duplicate(keyboard_tmp_rm32, 13, 1, -(20.5+2), 0); //BackSpace

    TopoDS_Shape keyboard31 = rounded_cube(gp_Vec(-146.25, 42, 16), 48, 20, -3, 1); //Tab
    TopoDS_Shape keyboard_tmp32 = rounded_cube(gp_Vec(160.25, 42, 16), 20, 20, -3, 1); //Q~P、{}、|
    TopoDS_Compound keyboard32 = linear_duplicate(keyboard_tmp32, 13, 1, -(20.5+2), 0); //BackSpace
    // 键盘第四行
    TopoDS_Shape keyboard_rm41 = rounded_cube(gp_Vec(-147.625, 19, 16), 45.75, 20.5, -3, 1);
    TopoDS_Shape keyboard_tmp_rm42 = rounded_cube(gp_Vec(-112.5, 19, 16), 20.5, 20.5, -3, 1);
    TopoDS_Compound keyboard_rm42 = linear_duplicate(keyboard_tmp_rm42, 11, 1, 20.5+2, 0);
    TopoDS_Shape keyboard_rm43 = rounded_cube(gp_Vec(147.625, 19, 16), 45.75, 20.5, -3, 1);

    TopoDS_Shape keyboard41 = rounded_cube(gp_Vec(-147.625, 19, 16), 45.25, 20, -3, 1);
    TopoDS_Shape keyboard_tmp42 = rounded_cube(gp_Vec(-112.5, 19, 16), 20, 20, -3, 1);
    TopoDS_Compound keyboard42 = linear_duplicate(keyboard_tmp42, 11, 1, 20.5+2, 0);
    TopoDS_Shape keyboard43 = rounded_cube(gp_Vec(147.625, 19, 16), 45.25, 20, -3, 1);
    // 键盘第五行
    TopoDS_Shape keyboard_rm51 = rounded_cube(gp_Vec(-142, -4, 16), 57, 20.5, -3, 1);
    TopoDS_Shape keyboard_tmp_rm52 = rounded_cube(gp_Vec(-101.25, -4, 16), 20.5, 20.5, -3, 1);
    TopoDS_Compound keyboard_rm52 = linear_duplicate(keyboard_tmp_rm52, 10, 1, 20.5+2, 0);
    TopoDS_Shape keyboard_rm53 = rounded_cube(gp_Vec(142, -4, 16), 57, 20.5, -3, 1);

    TopoDS_Shape keyboard51 = rounded_cube(gp_Vec(-142, -4, 16), 56.5, 20, -3, 1);
    TopoDS_Shape keyboard_tmp52 = rounded_cube(gp_Vec(-101.25, -4, 16), 20, 20, -3, 1);
    TopoDS_Compound keyboard52 = linear_duplicate(keyboard_tmp52, 10, 1, 20.5+2, 0);
    TopoDS_Shape keyboard53 = rounded_cube(gp_Vec(142, -4, 16), 56.5, 20, -3, 1);
    // 键盘第六行
    TopoDS_Shape keyboard_tmp_rm61 = rounded_cube(gp_Vec(-160.25, -27, 16), 20.5, 20.5, -3, 1);
    TopoDS_Compound keyboard_rm61 = linear_duplicate(keyboard_tmp_rm61, 4, 1, 20.5+2, 0);
    TopoDS_Shape keyboard_rm62 = rounded_cube(gp_Vec(-11.25, -27, 16), 138.5, 20.5, -3, 1);
    TopoDS_Shape keyboard_tmp_rm63 = rounded_cube(gp_Vec(160.25, -27, 16), 20.5, 20.5, -3, 1);
    TopoDS_Compound keyboard_rm63 = linear_duplicate(keyboard_tmp_rm63, 5, 1, -(20.5+2), 0);

    TopoDS_Shape keyboard_tmp61 = rounded_cube(gp_Vec(-160.25, -27, 16), 20, 20, -3, 1);
    TopoDS_Compound keyboard61 = linear_duplicate(keyboard_tmp61, 4, 1, 20.5+2, 0);
    TopoDS_Shape keyboard62 = rounded_cube(gp_Vec(-11.25, -27, 16), 138, 20, -3, 1);
    TopoDS_Shape keyboard_tmp63 = rounded_cube(gp_Vec(70.25, -27, 16), 20, 20, -3, 1);
    TopoDS_Compound keyboard63 = linear_duplicate(keyboard_tmp63, 3, 1, (20.5+2), 0);
    TopoDS_Shape keyboard64 = rounded_cube(gp_Vec(137.75, -31.75, 16), 20, 9.5, -3, 1); //上
    TopoDS_Shape keyboard65 = rounded_cube(gp_Vec(137.75, -21.75, 16), 20, 9.5, -3, 1); //下
    TopoDS_Shape keyboard66 = rounded_cube(gp_Vec(160.25, -27, 16), 20, 20, -3, 1);

    // 合并
    TopoDS_Compound keyboard_rm_compound;
    BRep_Builder keyboard_rm_builder;
    keyboard_rm_builder.MakeCompound(keyboard_rm_compound);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm1);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm21);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm22);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm31);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm32);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm41);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm42);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm43);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm51);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm52);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm53);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm61);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm62);
    keyboard_rm_builder.Add(keyboard_rm_compound, keyboard_rm63);

    TopoDS_Compound keyboard_compound;
    BRep_Builder keyboard_builder;
    keyboard_rm_builder.MakeCompound(keyboard_compound);
    keyboard_rm_builder.Add(keyboard_compound, keyboard1);
    keyboard_rm_builder.Add(keyboard_compound, keyboard21);
    keyboard_rm_builder.Add(keyboard_compound, keyboard22);
    keyboard_rm_builder.Add(keyboard_compound, keyboard31);
    keyboard_rm_builder.Add(keyboard_compound, keyboard32);
    keyboard_rm_builder.Add(keyboard_compound, keyboard41);
    keyboard_rm_builder.Add(keyboard_compound, keyboard42);
    keyboard_rm_builder.Add(keyboard_compound, keyboard43);
    keyboard_rm_builder.Add(keyboard_compound, keyboard51);
    keyboard_rm_builder.Add(keyboard_compound, keyboard52);
    keyboard_rm_builder.Add(keyboard_compound, keyboard53);
    keyboard_rm_builder.Add(keyboard_compound, keyboard61);
    keyboard_rm_builder.Add(keyboard_compound, keyboard62);
    keyboard_rm_builder.Add(keyboard_compound, keyboard63);
    keyboard_rm_builder.Add(keyboard_compound, keyboard64);
    keyboard_rm_builder.Add(keyboard_compound, keyboard65);
    keyboard_rm_builder.Add(keyboard_compound, keyboard66);


    //***************************边角美化*******************************//
    TopoDS_Shape slide_base = slide_shape(gp_Pnt(0, -105, -(500-16)/2), 30, 500);
    gp_Trsf rotation_slide_base;
    rotation_slide_base.SetRotation(gp_Ax1(gp_Pnt(0, -105, 30), gp_Dir(0, -1, 0)), M_PI/2);
    slide_base = BRepBuilderAPI_Transform(slide_base, rotation_slide_base).Shape();
    // context->Display(new AIS_Shape(slide_base), Standard_True);
    //***************************渲染*******************************//
    Quantity_Color color(71.0/255, 69.0/255, 79.0/255, Quantity_TOC_RGB);
    Quantity_Color color2(231.0/255, 231.0/255, 224.0/255, Quantity_TOC_RGB);
    Quantity_Color color3(33.0/255, 33.0/255, 33.0/255, Quantity_TOC_RGB);
    // 显示
    base_plate_tmp =  BRepAlgoAPI_Cut(base_plate_tmp, touch_tablet).Shape();
    base_plate_tmp = BRepAlgoAPI_Cut(base_plate_tmp, typeC_compound).Shape();
    base_plate_tmp = BRepAlgoAPI_Cut(base_plate_tmp, connection_BD_rm).Shape();
    base_plate_tmp = BRepAlgoAPI_Cut(base_plate_tmp, thermovent).Shape();
    base_plate_tmp = BRepAlgoAPI_Cut(base_plate_tmp, keyboard_rm_compound).Shape();
    base_plate_tmp = BRepAlgoAPI_Cut(base_plate_tmp, powerBtn_rm).Shape();

    // 布尔操作
    BRepFilletAPI_MakeChamfer base_plate_chamfer(base_plate_tmp);
    TopExp_Explorer base_plate_ex(base_plate_tmp, TopAbs_EDGE);
    while(base_plate_ex.More()){
        base_plate_chamfer.Add(0.1, TopoDS::Edge(base_plate_ex.Current()));
        base_plate_ex.Next();
    }
    base_plate_tmp = base_plate_chamfer.Shape();
    base_plate_tmp = BRepAlgoAPI_Cut(base_plate_tmp, slide_base).Shape();
    base_plate_tmp = BRepAlgoAPI_Cut(base_plate_tmp, keyboard_plate_rm).Shape();

    TopoDS_Shape base_plate = base_plate_tmp;
    TopoDS_Shape base_display = base_display_tmp;
    Handle_AIS_Shape base_plate_AIS = new AIS_Shape(base_plate);
    base_plate_AIS->SetColor(color2);
    base_plate_AIS->SetMaterial(Graphic3d_NameOfMaterial_Steel);
    Handle_AIS_Shape powerBtn_AIS = new AIS_Shape(powerBtn);
    powerBtn_AIS->SetColor(color2);
    connection_BD_AIS = new AIS_Shape(connection_BD);
    connection_BD_AIS->SetColor(color2);
    base_display_AIS = new AIS_Shape(base_display);
    base_display_AIS->SetColor(color2);
    Handle_AIS_Shape keyboard_AIS = new AIS_Shape(keyboard_compound);
    keyboard_AIS->SetColor(color);
    display_AIS = new AIS_Shape(display);
    display_AIS->SetColor(color3);

    OnePC_AIS_compound["base plate"] = base_plate_AIS;
    OnePC_AIS_compound["keyboard"] = keyboard_AIS;
    OnePC_AIS_compound["powerBtn"] = powerBtn_AIS;
    OnePC_AIS_compound["connection"] = connection_BD_AIS;
    OnePC_AIS_compound["base_display"] = base_display_AIS;
    OnePC_AIS_compound["display"] = display_AIS;

    for(auto tmp_AIS : OnePC_AIS_compound){
        context->Display(tmp_AIS.second, Standard_False);
    }

    // 显示轮廓线
    Handle_AIS_Shape base_plate_FR_AIS = new AIS_Shape(base_plate);
    Handle_AIS_Shape keyboard_FR_AIS = new AIS_Shape(keyboard_compound);
    Handle_AIS_Shape powerBtn_FR_AIS = new AIS_Shape(powerBtn);
    connection_BD_FR_AIS = new AIS_Shape(connection_BD);
    base_display_FR_AIS = new AIS_Shape(base_display);
    display_FR_AIS = new AIS_Shape(display);
    base_plate_FR_AIS->SetColor(Quantity_NOC_BLACK);
    base_plate_FR_AIS->Attributes()->SetDisplayMode(0);
    keyboard_FR_AIS->SetColor(Quantity_NOC_BLACK);
    keyboard_FR_AIS->Attributes()->SetDisplayMode(0);
    powerBtn_FR_AIS->SetColor(Quantity_NOC_BLACK);
    powerBtn_FR_AIS->Attributes()->SetDisplayMode(0);
    connection_BD_FR_AIS->SetColor(Quantity_NOC_BLACK);
    connection_BD_FR_AIS->Attributes()->SetDisplayMode(0);
    base_display_FR_AIS->SetColor(Quantity_NOC_BLACK);
    base_display_FR_AIS->Attributes()->SetDisplayMode(0);
    display_FR_AIS->SetColor(Quantity_NOC_BLACK);
    display_FR_AIS->Attributes()->SetDisplayMode(0);

    OnePC_FR_AIS_compound["base plate"] = base_plate_FR_AIS;
    OnePC_FR_AIS_compound["keyboard"] = keyboard_FR_AIS;
    OnePC_FR_AIS_compound["powerBtn"] = connection_BD_FR_AIS;
    OnePC_FR_AIS_compound["connection"] = connection_BD_FR_AIS;
    OnePC_FR_AIS_compound["base display"] = base_display_FR_AIS;
    OnePC_FR_AIS_compound["display"] = display_FR_AIS;

    for(auto tmp_AIS : OnePC_FR_AIS_compound){
        context->Display(tmp_AIS.second, Standard_False);
    }
    context->UpdateCurrentViewer();

    OnePC_AIS_compound_SW[base_plate_AIS] = "base plate";
    OnePC_AIS_compound_SW[keyboard_AIS] = "keyboard";
    OnePC_AIS_compound_SW[powerBtn_AIS] = "powerBtn";
    OnePC_AIS_compound_SW[connection_BD_AIS] = "connection";
    OnePC_AIS_compound_SW[base_display_AIS] = "base display";
    OnePC_AIS_compound_SW[display_AIS] = "display";

    OnePC_FR_AIS_compound_SW[base_plate_FR_AIS] = "base plate";
    OnePC_FR_AIS_compound_SW[keyboard_FR_AIS] = "keyboard";
    OnePC_FR_AIS_compound_SW[connection_BD_FR_AIS] = "powerBtn";
    OnePC_FR_AIS_compound_SW[connection_BD_FR_AIS] = "connection";
    OnePC_FR_AIS_compound_SW[base_display_FR_AIS] = "base_display";
    OnePC_FR_AIS_compound_SW[display_FR_AIS] = "display";
}

void Model::ShowPC(){
    if(show_status == -1){  //初始化显示
        BuildingPC();
        show_status = 1;  //已经初始化
    }else if(show_status == 0){
        for(auto tmp_AIS_ite = OnePC_AIS_compound.begin(), tmp_FR_AIS_ite = OnePC_FR_AIS_compound.begin();
                 tmp_AIS_ite != OnePC_AIS_compound.end() && tmp_FR_AIS_ite != OnePC_FR_AIS_compound.end();
                 tmp_AIS_ite++, tmp_FR_AIS_ite++)
        {
            context->Display((*tmp_AIS_ite).second, Standard_False);
            context->Display((*tmp_FR_AIS_ite).second, Standard_False);
        }
        context->UpdateCurrentViewer();
        show_status = 1;
        frame_status = 1;
    }
}
void Model::HidePC(){
    if(show_status == 1){
        for(auto tmp_AIS_ite = OnePC_AIS_compound.begin(), tmp_FR_AIS_ite = OnePC_FR_AIS_compound.begin();
                 tmp_AIS_ite != OnePC_AIS_compound.end() && tmp_FR_AIS_ite != OnePC_FR_AIS_compound.end();
                 tmp_AIS_ite++, tmp_FR_AIS_ite++)
        {
            context->Erase((*tmp_AIS_ite).second, Standard_False);
            context->Erase((*tmp_FR_AIS_ite).second, Standard_False);
        }
        context->UpdateCurrentViewer();
        show_status = 0;
        frame_status = 0;
    }
}

void Model::ShowFrame(){
    if(frame_status == 0 && show_status == 1){
        AIS_ListOfInteractive anInteractiveList;
        context->DisplayedObjects(anInteractiveList);
        // 遍历显示对象列表
        TopoDS_Shape tmp_shape;
        Handle_AIS_Shape tmp_AIS;
        for (AIS_ListIteratorOfListOfInteractive aDisplayedListIter(anInteractiveList); aDisplayedListIter.More(); aDisplayedListIter.Next()) {
            Handle_AIS_InteractiveObject anInteractive = aDisplayedListIter.Value();
            // 在这里处理每个显示对象
            tmp_AIS = Handle_AIS_Shape::DownCast(anInteractive);
            std::string name_AIS;
            if(OnePC_AIS_compound_SW.find(tmp_AIS) != OnePC_AIS_compound_SW.end()){
                name_AIS = OnePC_AIS_compound_SW[tmp_AIS];
                context->Display(OnePC_FR_AIS_compound[name_AIS], Standard_False);
            }
        }
        context->UpdateCurrentViewer();
        frame_status = 1;
    }
}
void Model::HideFrame(){
    if(frame_status == 1 && show_status == 1){
        AIS_ListOfInteractive anInteractiveList;
        context->DisplayedObjects(anInteractiveList);
        // 遍历显示对象列表
        TopoDS_Shape tmp_shape;
        Handle_AIS_Shape tmp_AIS;
        for (AIS_ListIteratorOfListOfInteractive aDisplayedListIter(anInteractiveList); aDisplayedListIter.More(); aDisplayedListIter.Next()) {
            Handle_AIS_InteractiveObject anInteractive = aDisplayedListIter.Value();
            // 在这里处理每个显示对象
            tmp_AIS = Handle_AIS_Shape::DownCast(anInteractive);
            std::string name_AIS;
            if(OnePC_AIS_compound_SW.find(tmp_AIS) != OnePC_AIS_compound_SW.end()){
                name_AIS = OnePC_AIS_compound_SW[tmp_AIS];
                context->Erase(OnePC_FR_AIS_compound[name_AIS], Standard_False);
            }
        }
        context->UpdateCurrentViewer();
        frame_status = 0;
    }
}

void Model::OnePCOpen(){
    if(open_status == 0){
        gp_Trsf end_trsf_all;
        gp_Trsf start_trsf_display, start_trsf_base_display, start_trsf_connection_BD;
        gp_Trsf start_trsf_display_FR, start_trsf_base_display_FR, start_trsf_connection_BD_FR;
        gp_Ax1 ax1(gp_Pnt(0, 120, 8), gp_Vec(1, 0, 0));
        end_trsf_all.SetRotation(ax1, 3*M_PI/4);
        Handle(AIS_Animation) display_animation = new AIS_Animation("display");
        Handle(AIS_Animation) base_display_animation = new AIS_Animation("base_display");
        Handle(AIS_Animation) connection_BD_animation = new AIS_Animation("connection_BD");
        Handle(AIS_Animation) display_FR_animation = new AIS_Animation("display_FR");
        Handle(AIS_Animation) base_display_FR_animation = new AIS_Animation("base_display_FR");
        Handle(AIS_Animation) connection_BD_FR_animation = new AIS_Animation("connection_BD_FR");

        start_trsf_display = display_AIS->LocalTransformation();
        start_trsf_base_display = base_display_AIS->LocalTransformation();
        start_trsf_connection_BD = connection_BD_AIS->LocalTransformation();
        start_trsf_display_FR = display_FR_AIS->LocalTransformation();
        start_trsf_base_display_FR = base_display_FR_AIS->LocalTransformation();
        start_trsf_connection_BD_FR = connection_BD_FR_AIS->LocalTransformation();

        Handle(AIS_AnimationAxisRotation) display_AARotation = new AIS_AnimationAxisRotation("display", context, display_AIS, ax1, 3*M_PI/4, 0);
        Handle(AIS_AnimationAxisRotation) base_display_AARotation = new AIS_AnimationAxisRotation("base_display", context, base_display_AIS, ax1, 3*M_PI/4, 0);
        Handle(AIS_AnimationAxisRotation) connection_BD_AARotation = new AIS_AnimationAxisRotation("connection_BD", context, connection_BD_AIS, ax1, 3*M_PI/4, 0);
        Handle(AIS_AnimationAxisRotation) display_FR_AARotation = new AIS_AnimationAxisRotation("display_FR", context, display_FR_AIS, ax1, 3*M_PI/4, 0);
        Handle(AIS_AnimationAxisRotation) base_display_FR_AARotation = new AIS_AnimationAxisRotation("base_display_FR", context, base_display_FR_AIS, ax1, 3*M_PI/4, 0);
        Handle(AIS_AnimationAxisRotation) connection_BD_FR_AARotation = new AIS_AnimationAxisRotation("connection_BD_FR", context, connection_BD_FR_AIS, ax1, 3*M_PI/4, 0);

        display_AARotation->SetOwnDuration(0.5);
        base_display_AARotation->SetOwnDuration(0.5);
        connection_BD_AARotation->SetOwnDuration(0.5);
        display_FR_AARotation->SetOwnDuration(0.5);
        base_display_FR_AARotation->SetOwnDuration(0.5);
        connection_BD_FR_AARotation->SetOwnDuration(0.5);

        display_animation->Add(display_AARotation);
        base_display_animation->Add(base_display_AARotation);
        connection_BD_animation->Add(connection_BD_AARotation);
        display_FR_animation->Add(display_FR_AARotation);
        base_display_FR_animation->Add(base_display_FR_AARotation);
        connection_BD_FR_animation->Add(connection_BD_FR_AARotation);

        display_animation->StartTimer(0, 1.0, true);
        base_display_animation->StartTimer(0, 1.0, true);
        connection_BD_animation->StartTimer(0, 1.0, true);
        display_FR_animation->StartTimer(0, 1.0, true);
        base_display_FR_animation->StartTimer(0, 1.0, true);
        connection_BD_FR_animation->StartTimer(0, 1.0, true);

        while (!display_animation->IsStopped())
        {
            display_animation->UpdateTimer();
            base_display_animation->UpdateTimer();
            connection_BD_animation->UpdateTimer();
            display_FR_animation->UpdateTimer();
            base_display_FR_animation->UpdateTimer();
            connection_BD_FR_animation->UpdateTimer();
            context->UpdateCurrentViewer();
        }
        open_status = 1;
    }
}
void Model::OnePCClose(){
    if(open_status == 1){
        gp_Trsf end_trsf_all;
        gp_Trsf start_trsf_display, start_trsf_base_display, start_trsf_connection_BD;
        gp_Trsf start_trsf_display_FR, start_trsf_base_display_FR, start_trsf_connection_BD_FR;
        gp_Ax1 ax1(gp_Pnt(0, 120, 8), gp_Vec(1, 0, 0));
        end_trsf_all.SetRotation(ax1, 3*M_PI/4);
        Handle(AIS_Animation) display_animation = new AIS_Animation("display");
        Handle(AIS_Animation) base_display_animation = new AIS_Animation("base_display");
        Handle(AIS_Animation) connection_BD_animation = new AIS_Animation("connection_BD");
        Handle(AIS_Animation) display_FR_animation = new AIS_Animation("display_FR");
        Handle(AIS_Animation) base_display_FR_animation = new AIS_Animation("base_display_FR");
        Handle(AIS_Animation) connection_BD_FR_animation = new AIS_Animation("connection_BD_FR");

        start_trsf_display = display_AIS->LocalTransformation();
        start_trsf_base_display = base_display_AIS->LocalTransformation();
        start_trsf_connection_BD = connection_BD_AIS->LocalTransformation();
        start_trsf_display_FR = display_FR_AIS->LocalTransformation();
        start_trsf_base_display_FR = base_display_FR_AIS->LocalTransformation();
        start_trsf_connection_BD_FR = connection_BD_FR_AIS->LocalTransformation();

        Handle(AIS_AnimationAxisRotation) display_AARotation = new AIS_AnimationAxisRotation("display", context, display_AIS, ax1, 0, 3*M_PI/4);
        Handle(AIS_AnimationAxisRotation) base_display_AARotation = new AIS_AnimationAxisRotation("base_display", context, base_display_AIS, ax1, 0, 3*M_PI/4);
        Handle(AIS_AnimationAxisRotation) connection_BD_AARotation = new AIS_AnimationAxisRotation("connection_BD", context, connection_BD_AIS, ax1, 0, 3*M_PI/4);
        Handle(AIS_AnimationAxisRotation) display_FR_AARotation = new AIS_AnimationAxisRotation("display_FR", context, display_FR_AIS, ax1, 0, 3*M_PI/4);
        Handle(AIS_AnimationAxisRotation) base_display_FR_AARotation = new AIS_AnimationAxisRotation("base_display_FR", context, base_display_FR_AIS, ax1, 0, 3*M_PI/4);
        Handle(AIS_AnimationAxisRotation) connection_BD_FR_AARotation = new AIS_AnimationAxisRotation("connection_BD_FR", context, connection_BD_FR_AIS, ax1, 0, 3*M_PI/4);

        display_AARotation->SetOwnDuration(0.5);
        base_display_AARotation->SetOwnDuration(0.5);
        connection_BD_AARotation->SetOwnDuration(0.5);
        display_FR_AARotation->SetOwnDuration(0.5);
        base_display_FR_AARotation->SetOwnDuration(0.5);
        connection_BD_FR_AARotation->SetOwnDuration(0.5);

        display_animation->Add(display_AARotation);
        base_display_animation->Add(base_display_AARotation);
        connection_BD_animation->Add(connection_BD_AARotation);
        display_FR_animation->Add(display_FR_AARotation);
        base_display_FR_animation->Add(base_display_FR_AARotation);
        connection_BD_FR_animation->Add(connection_BD_FR_AARotation);

        display_animation->StartTimer(0, 1.0, true);
        base_display_animation->StartTimer(0, 1.0, true);
        connection_BD_animation->StartTimer(0, 1.0, true);
        display_FR_animation->StartTimer(0, 1.0, true);
        base_display_FR_animation->StartTimer(0, 1.0, true);
        connection_BD_FR_animation->StartTimer(0, 1.0, true);

        while (!display_animation->IsStopped())
        {
            display_animation->UpdateTimer();
            base_display_animation->UpdateTimer();
            connection_BD_animation->UpdateTimer();
            display_FR_animation->UpdateTimer();
            base_display_FR_animation->UpdateTimer();
            connection_BD_FR_animation->UpdateTimer();
            context->UpdateCurrentViewer();
        }
        open_status = 0;
    }
}

TopoDS_Shape Model::OnePCLogo(const Standard_Real Size){
    // 创建一个字体
    StdPrs_BRepFont aFont("Arial", Font_FA_Bold, Size, Font_StrictLevel_Any);
    // aFont.SetFontAspect(Font_FA_Bold);

    // 定义文本字符串和位置
    NCollection_String aTextString("ONEPC");

    Handle(Font_TextFormatter) formatter = new Font_TextFormatter();

    // 使用Perform方法将文本呈现为BRep形状
    StdPrs_BRepTextBuilder aTextBuilder;
    TopoDS_Shape aTextShape = aTextBuilder.Perform(aFont, aTextString, gp_Ax3(), Graphic3d_HTA_LEFT, Graphic3d_VTA_BOTTOM);
    gp_Vec vec = gp_Vec(0, 0, 3);
    BRepPrimAPI_MakePrism res = BRepPrimAPI_MakePrism(aTextShape, vec);
    return res.Shape();
}


TopoDS_Shape Model::rounded_cube(gp_Vec centerpos, Standard_Real length, Standard_Real width, Standard_Real height, Standard_Real radius){
    centerpos = gp_Vec(-length/2, -width/2, 0)+centerpos;
    TColgp_Array1OfPnt points(1, 8);
    points.SetValue(1, gp_Pnt(radius, 0, 0).Translated(centerpos));
    points.SetValue(2, gp_Pnt(length-radius, 0, 0).Translated(centerpos));
    points.SetValue(3, gp_Pnt(length, radius, 0).Translated(centerpos));
    points.SetValue(4, gp_Pnt(length, width-radius, 0).Translated(centerpos));
    points.SetValue(5, gp_Pnt(length-radius, width, 0).Translated(centerpos));
    points.SetValue(6, gp_Pnt(radius, width, 0).Translated(centerpos));
    points.SetValue(7, gp_Pnt(0, width-radius, 0).Translated(centerpos));
    points.SetValue(8, gp_Pnt(0, radius, 0).Translated(centerpos));

    // 使用多段线连接这些点
    BRepBuilderAPI_MakeEdge* edge1 = new BRepBuilderAPI_MakeEdge(points[1], points[2]);
    BRepBuilderAPI_MakeEdge* edge2 = new BRepBuilderAPI_MakeEdge(points[3], points[4]);
    BRepBuilderAPI_MakeEdge* edge3 = new BRepBuilderAPI_MakeEdge(points[5], points[6]);
    BRepBuilderAPI_MakeEdge* edge4 = new BRepBuilderAPI_MakeEdge(points[7], points[8]);

    // 定义圆弧相关参数
    gp_Ax2 axis(gp_Pnt(radius, radius, 0).Translated(centerpos), gp_Dir(0, 0,1));
    Standard_Real startParam = M_PI / 1.0;  // 开始参数
    Standard_Real endParam = 2*M_PI-M_PI/2; // 结束参数，这里是四分之一圆弧
    // 创建完整的圆弧
    Handle(Geom_Circle) circle = new Geom_Circle(axis, radius);
    // 对圆弧进行修剪，得到四分之一圆弧
    Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(circle, startParam, endParam);
    // 创建线的边缘
    TopoDS_Edge arc1 = BRepBuilderAPI_MakeEdge(trimmedCurve);


    gp_Ax2 axis2(gp_Pnt(length-radius, radius, 0).Translated(centerpos), gp_Dir(0, 0,1));
    Standard_Real startParam2 = 2*M_PI-M_PI/2;  // 开始参数
    Standard_Real endParam2 = 0; // 结束参数，这里是四分之一圆弧
    // 创建完整的圆弧
    Handle(Geom_Circle) circle2 = new Geom_Circle(axis2, radius);
    // 对圆弧进行修剪，得到四分之一圆弧
    Handle(Geom_TrimmedCurve) trimmedCurve2 = new Geom_TrimmedCurve(circle2, startParam2, endParam2);
    // 创建线的边缘
    TopoDS_Edge arc2 = BRepBuilderAPI_MakeEdge(trimmedCurve2);

    gp_Ax2 axis3(gp_Pnt(length-radius, width-radius, 0).Translated(centerpos), gp_Dir(0, 0,1));
    Standard_Real startParam3 = 0;  // 开始参数
    Standard_Real endParam3 = M_PI/2; // 结束参数，这里是四分之一圆弧
    // 创建完整的圆弧
    Handle(Geom_Circle) circle3 = new Geom_Circle(axis3, radius);
    // 对圆弧进行修剪，得到四分之一圆弧
    Handle(Geom_TrimmedCurve) trimmedCurve3 = new Geom_TrimmedCurve(circle3, startParam3, endParam3);
    // 创建线的边缘
    TopoDS_Edge arc3 = BRepBuilderAPI_MakeEdge(trimmedCurve3);

    gp_Ax2 axis4(gp_Pnt(radius, width-radius, 0).Translated(centerpos), gp_Dir(0, 0,1));
    Standard_Real startParam4 = M_PI/2;  // 开始参数
    Standard_Real endParam4 = M_PI; // 结束参数，这里是四分之一圆弧
    // 创建完整的圆弧
    Handle(Geom_Circle) circle4 = new Geom_Circle(axis4, radius);
    // 对圆弧进行修剪，得到四分之一圆弧
    Handle(Geom_TrimmedCurve) trimmedCurve4 = new Geom_TrimmedCurve(circle4, startParam4, endParam4);
    // 创建线的边缘
    TopoDS_Edge arc4 = BRepBuilderAPI_MakeEdge(trimmedCurve4);
    //创建一个闭合线
    BRepBuilderAPI_MakeWire wireMaker(arc1, edge1->Edge(), arc2, edge2->Edge());
    wireMaker.Add(arc3);
    wireMaker.Add(edge3->Edge());
    wireMaker.Add(arc4);
    wireMaker.Add(edge4->Edge());

    // 从闭合曲线生成一个面
    BRepBuilderAPI_MakeFace makeFace(wireMaker, Standard_True);
    TopoDS_Face face = makeFace.Face();

    // 在平面上生成一个体，这里我们使用BRepPrimAPI_MakePrism将面拉伸成为一个体
    BRepPrimAPI_MakePrism prism = BRepPrimAPI_MakePrism(face, gp_Vec(0, 0, height));
    return prism.Shape(); // 获取拉伸后的形状
}

TopoDS_Shape Model::make_connection_BD(gp_Vec centerpos, Standard_Real length, Standard_Real width, Standard_Real height){
    centerpos = gp_Vec(0, 0, 0)+centerpos;
    TColgp_Array1OfPnt points(1, 4);
    points.SetValue(1, gp_Pnt(0, 0, width/2).Translated(centerpos));
    points.SetValue(2, gp_Pnt(0, 0, -width/2).Translated(centerpos));
    points.SetValue(3, gp_Pnt(0, length, -width/2).Translated(centerpos));
    points.SetValue(4, gp_Pnt(0, length, width/2).Translated(centerpos));

    // 使用多段线连接这些点
    BRepBuilderAPI_MakeEdge* edge1 = new BRepBuilderAPI_MakeEdge(points[2], points[3]);
    BRepBuilderAPI_MakeEdge* edge2 = new BRepBuilderAPI_MakeEdge(points[3], points[4]);
    BRepBuilderAPI_MakeEdge* edge3 = new BRepBuilderAPI_MakeEdge(points[4], points[1]);

    // 定义圆弧相关参数
    gp_Ax2 axis(gp_Pnt(0, 0, 0).Translated(centerpos), gp_Dir(1, 0, 0));
    Standard_Real startParam = 0;  // 开始参数
    Standard_Real endParam = M_PI; // 结束参数，这里是四分之一圆弧
    // 创建完整的圆弧
    Handle(Geom_Circle) circle = new Geom_Circle(axis, width/2);
    // 对圆弧进行修剪，得到四分之一圆弧
    Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(circle, startParam, endParam);
    // 创建线的边缘
    TopoDS_Edge arc1 = BRepBuilderAPI_MakeEdge(trimmedCurve);
    // context->Display(new AIS_Shape(arc1), Standard_True);

    //创建一个闭合线
    BRepBuilderAPI_MakeWire wireMaker(edge1->Edge(), edge2->Edge(), edge3->Edge(), arc1);

    // 从闭合曲线生成一个面
    BRepBuilderAPI_MakeFace makeFace(wireMaker, Standard_True);
    TopoDS_Face face = makeFace.Face();

    // 在平面上生成一个体，这里我们使用BRepPrimAPI_MakePrism将面拉伸成为一个体
    BRepPrimAPI_MakePrism prism = BRepPrimAPI_MakePrism(face, gp_Vec(height, 0, 0));
    return prism.Shape(); // 获取拉伸后的形状
}

TopoDS_Compound Model::linear_duplicate(TopoDS_Shape& shape, Standard_Integer numX, Standard_Integer numY, Standard_Real xSpacing, Standard_Real ySpacing){

    // 创建一个新的复合形状
    TopoDS_Compound compound;
    BRep_Builder builder;
    builder.MakeCompound(compound);

    // 线性阵列立方体
    for (int i = 0; i < numX; ++i)
    {
        for (int j = 0; j < numY; ++j)
        {
            // 计算当前立方体的平移向量
            gp_Vec translation(i * xSpacing, j * ySpacing, 0);

            // 创建一个平移变换
            gp_Trsf trsf;
            trsf.SetTranslation(translation);

            // 应用变换到当前立方体
            TopLoc_Location location(trsf);
            TopoDS_Shape transformedBox = shape;
            transformedBox.Location(location);

            // 将变换后的立方体添加到复合形状中
            builder.Add(compound, transformedBox);
        }
    }
    return compound;

}

TopoDS_Shape Model::slide_shape(gp_Pnt centerpos, Standard_Real radius, Standard_Real height){
    TColgp_Array1OfPnt points(1, 3);
    points.SetValue(1, centerpos.Translated(gp_Vec(-radius, 0, 0)));
    points.SetValue(2, centerpos.Translated(gp_Vec(0, -radius, 0)));
    points.SetValue(3, centerpos.Translated(gp_Vec(-radius, -radius, 0)));
    // 使用多段线连接这些点
    BRepBuilderAPI_MakeEdge* edge1 = new BRepBuilderAPI_MakeEdge(points[1], points[3]);
    BRepBuilderAPI_MakeEdge* edge2 = new BRepBuilderAPI_MakeEdge(points[3], points[2]);

    // 定义圆弧相关参数
    gp_Ax2 axis(centerpos, gp_Dir(0, 0,1));
    Standard_Real startParam = M_PI / 1.0;  // 开始参数
    Standard_Real endParam = 2*M_PI-M_PI/2; // 结束参数，这里是四分之一圆弧
    // 创建完整的圆弧
    Handle(Geom_Circle) circle = new Geom_Circle(axis, radius);
    // 对圆弧进行修剪，得到四分之一圆弧
    Handle(Geom_TrimmedCurve) trimmedCurve = new Geom_TrimmedCurve(circle, startParam, endParam);
    // 创建线的边缘
    TopoDS_Edge arc1 = BRepBuilderAPI_MakeEdge(trimmedCurve);
    //创建一个闭合线
    BRepBuilderAPI_MakeWire wireMaker(arc1, edge1->Edge(), edge2->Edge());

    // 从闭合曲线生成一个面
    BRepBuilderAPI_MakeFace makeFace(wireMaker, Standard_True);
    TopoDS_Face face = makeFace.Face();

    // 在平面上生成一个体，这里我们使用BRepPrimAPI_MakePrism将面拉伸成为一个体
    BRepPrimAPI_MakePrism prism = BRepPrimAPI_MakePrism(face, gp_Vec(0, 0, height));
    return prism.Shape(); // 获取拉伸后的形状
}

std::map<std::string, Handle(AIS_Shape)> Model::getOnePC_AIS(){
    return OnePC_AIS_compound;
}
