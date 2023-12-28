function MeshInfExtract(file, output_path)

%%
%  网格参数
% numEntityBlocks()
% numNodes
% minNodeTag
% maxNodeTag

node_flag = 0;
element_flag = 0;

fid = fopen(file);

data = fgetl(fid);
while(data ~= -1)
    if(data == "$Nodes" && ~node_flag)
        FormatString=repmat('%f',1,4);
        data = textscan(fid,FormatString,1);
        [numEntityBlocks, numNodes, minNodeTag, maxNodeTag] = data{:};

        coord = zeros(maxNodeTag - minNodeTag, 4);    %网格结点坐标
        index_coord = 1;
        while(index_coord ~= maxNodeTag-minNodeTag+2)
            FormatString=repmat('%f',1,4);
            data = textscan(fid,FormatString,1);
            [entityDim, entityTag, parametric, numNodesInBlock] = data{:};
            data = cell2mat( textscan(fid, FormatString, 2*numNodesInBlock) );

            coord(index_coord:index_coord+numNodesInBlock-1, :) = ...
                cat(2, data(1:numNodesInBlock, 1), data(numNodesInBlock+1:end, 1:3) );
            index_coord = index_coord + numNodesInBlock;
        end
        node_flag = 1;
        data = fgetl(fid);
        data = fgetl(fid);
        data = fgetl(fid);
    end

    if(data == "$Elements" && ~element_flag)
        FormatString=repmat('%f',1,4);
        data = textscan(fid,FormatString,1);
        [~, numElements, minElementTag, maxElementTag] = data{:};

        element = zeros(maxElementTag - minElementTag, 5);    %单元
        index_element = 1;
        while(index_element ~= maxElementTag-minElementTag+2)
            FormatString=repmat('%f',1,4);
            data = textscan(fid,FormatString,1);
            [entityDim, entityTag, elementType, numElementsInBlock] = data{:};
            FormatString=repmat('%f',1,5);
            data = cell2mat( textscan(fid, FormatString, numElementsInBlock) );

            element(index_element:index_element+numElementsInBlock-1, :) = ...
                data(1:numElementsInBlock, :);
            index_element = index_element + numElementsInBlock;
        end
        element_flag = 1;
    end
    data = fgetl(fid);
end
% save('meshInf.mat', 'coord', 'element');
fclose(fid);
element_P = element(isnan(element(:, 3)), 1:2);  %点单元
element_L = element(isnan(element(:, 4))&~isnan(element(:, 3)), 1:3);  %线单元
element_S = element(isnan(element(:, 5))&~isnan(element(:, 4))&~isnan(element(:, 3)), 1:4);  %面单元
element_V = element(~isnan(element(:, 5)), 1:5);  %面单元
save(output_path+"\meshInf.mat", 'coord', 'element_P', 'element_L', 'element_S', 'element_V');
end