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



for i = 1:length(topos)
    subplot (1, length(topos),i)
    for k = 1:length(prots)
        first = (i-1)*200 + (k-1)*50 + 1;
        last = first + 49;
        x = budget (first:last);
        y = rate (first:last);
        plot (x, y,'-*','LineWidth',2);
        hold on;
    end
end

ldg = legend ("ECMP", "KSHORT", "DGR", "DDR", Location="bestoutside");