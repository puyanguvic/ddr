



clc
clear
close all
topos = ["abilene" "att" "cernet" "geant"];
prots = ["dgr" "dw" "dwe" "ecmp" "lfid" "dgrv2"];
d = [];
l = [];
for i = 1:length(topos)
    topo_d = [];
    topo_l = [];
    for j = 1:10
        tmp_d = [];
        tmp_l = [];
        for k = 1:length(prots)
            filename = strcat(topos{i}, "/", prots{k}, "-", num2str(j), "Mbps.txt");
            tmp_d = [tmp_d; avg(filename)];
            tmp_l = [tmp_l; lr(filename)];
        end
        topo_d = [topo_d tmp_d];
        topo_l = [topo_l tmp_l];
    end
    d = [d; topo_d];
    l = [l; topo_l];
end

x = [1:1:10];

for i = 1:length(topos)
    subplot (1,length(topos),i)
    plot (x,d((i-1)*length(prots) + 1,:), '-s', 'LineWidth',2, 'MarkerSize',8); 
    hold on;
    plot (x,d((i-1)*length(prots) + 2,:), '-o', 'LineWidth',2, 'MarkerSize',8); 
    hold on;
    plot (x,d((i-1)*length(prots) + 3,:), '-^', 'LineWidth',2, 'MarkerSize',8); 
    hold on;
    plot (x,d((i-1)*length(prots) + 4,:), '-d', 'LineWidth',2, 'MarkerSize',8); 
    hold on;
    plot (x,d((i-1)*length(prots) + 5,:), '-x', 'LineWidth',2, 'MarkerSize',8); 
    hold on;
    axis([0,10.5,0,300])
end
ldg = legend ("DGR", "DW", "DWE", "ECMP", "LFID", Location="bestoutside");

% for i = 1:length(topos)
%     subplot (1, length(topos),i)
%     for k = 1:length(prots)
%         plot (x,l((i-1)*length(prots) + k,:), '-*','LineWidth',2);
%         hold on;
%     end
% end
% 
% ldg = legend ("dgr", "dw", "dwe", "ecmp", "lfid", Location="bestoutside");

