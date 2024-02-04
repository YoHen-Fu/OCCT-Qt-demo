function ShowMesh(MeshInf, app)

coord = MeshInf.coord;                     %---坐标信息
coord(:, 2:end) = coord(:, 2:end)*1e-3;  %---坐标单位变换为国际单位制：m
element_s = MeshInf.element_S;             %---获取面单元信息
element_l = MeshInf.element_L;             %---获取线单元信息

patch(app.UIAxes, 'vertices', coord(:, 2:3), 'faces', element_s(:, 3:end),'FaceColor',[1 1 1]);
axis(app.UIAxes, 'equal');
end
