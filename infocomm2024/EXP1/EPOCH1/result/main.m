clc
clear
close all
topos = ["abilene" "att" "cernet" "geant"];
prots = ["OSPF" "KSHORT" "DGR" "DDR"];
rate = [];
budget = [];
for i = 1:length(topos)
    topo_rate = [];
    topo_budget = [];
    for k = 1:length(prots)
        tmp_rate = [];
        tmp_budget = [];
        for j = 1: 50
            filename = strcat(topos{i}, "/", prots{k}, "-", num2str(j), ".txt");
            [b, r] = data_processor(filename);
            tmp_rate = [tmp_rate; r];
            tmp_budget = [tmp_budget; b];
        end
        topo_rate = [topo_rate; tmp_rate];
        topo_budget = [topo_budget; tmp_budget];
    end
    rate = [rate; topo_rate];
    budget = [budget; topo_budget];
end


% 
% for i = 1:length(topos)
%     subplot (1,length(topos),i)
%     plot (x,d((i-1)*length(prots) + 1,:), '-s', 'LineWidth',2, 'MarkerSize',8); 
%     hold on;
%     plot (x,d((i-1)*length(prots) + 2,:), '-o', 'LineWidth',2, 'MarkerSize',8); 
%     hold on;
%     plot (x,d((i-1)*length(prots) + 3,:), '-^', 'LineWidth',2, 'MarkerSize',8); 
%     hold on;
%     plot (x,d((i-1)*length(prots) + 4,:), '-d', 'LineWidth',2, 'MarkerSize',8); 
%     hold on;
%     plot (x,d((i-1)*length(prots) + 5,:), '-x', 'LineWidth',2, 'MarkerSize',8); 
%     hold on;
%     axis([0,10.5,0,300])
% end
% ldg = legend ("DGR", "DW", "DWE", "ECMP", "LFID", Location="bestoutside");

% 打开一个文件用于输出
fileID = fopen('exp.txt', 'w');

% 设置输出格式为3位小数+制表符
formatSpec = '%f\t';

% 输出数组 A 和 B 交叉到文件
for i = 1:50:800
    % 输出 rate 数组的数据，每行 50
    fprintf(fileID, formatSpec, rate(i:i+49));
    fprintf(fileID, '\n');
    % 输出 budget 数组的数据，每行 50
    fprintf(fileID, formatSpec, budget(i:i+49));
    fprintf(fileID, '\n\n');
end

% 关闭文件
fclose(fileID);


figure;
for i = 1:length(topos)
    subplot (1, length(topos),i)
    for k = 1:length(prots)
        first = (i-1)*200 + (k-1)*50 + 1;
        last = first + 49;
        x = budget (first:last);
        y = rate (first:last);
        plot (x, y,'LineWidth',2);
		xlabel ('Time Limit (ms)');
        hold on;
    end
end

ylabel ('Arrival Rate (%)');
ldg = legend ("ECMP", "KSHORT", "DGR", "DDR", Location="bestoutside");