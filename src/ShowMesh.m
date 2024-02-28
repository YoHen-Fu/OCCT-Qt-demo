function ShowMesh(MeshInf, app, format)

coord = MeshInf.coord;                     %---坐标信息
coord(:, 2:end) = coord(:, 2:end)*1e-3;  %---坐标单位变换为国际单位制：m
element_s = MeshInf.element_S;             %---获取面单元信息
element_l = MeshInf.element_L;             %---获取线单元信息

if(format == "all")
    patch(app.UIAxes, 'vertices', coord(:, 2:3), 'faces', element_s(:, 3:end),'FaceColor',[1 1 1]);
    axis(app.UIAxes, 'equal');
elseif(format == "faces")
    patch(app.UIAxes, 'vertices', coord(:, 2:3), 'faces', element_s(str2num(app.NumOfS.Value), 3:end),'FaceColor',[0.5 1 1]);
elseif(format == "lines")
    patch(app.UIAxes, 'vertices', coord(:, 2:3), 'faces', element_l(str2num(app.NumOfL.Value), 3:end),'EdgeColor',[1 0 0]);
elseif(format == "nodes")
    patch(app.UIAxes, 'vertices', coord(:, 2:3), 'faces', str2num(app.NumOfN.Value)','Marker','o', 'MarkerFaceColor','black');

elseif(format == "facegroups")
    patch(app.UIAxes, 'vertices', coord(:, 2:3), 'faces', element_s(str2num(app.IdOfSGroup.Value)==element_s(:,1), 3:end),'FaceColor',[0.5 1 1]);
elseif(format == "linegroups")
    patch(app.UIAxes, 'vertices', coord(:, 2:3), 'faces', element_l(str2num(app.IdOfLGroup.Value)==element_l(:,1), 3:end),'EdgeColor',[1 0 0]);
end

end
