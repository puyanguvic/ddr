



% clc
% clear
% close all
topos = ["abilene" "att" "cernet" "geant"];
prots = ["dgr" "dw" "dwe" "ecmp" "ospf"];
% d = [];
% l = [];
% for i = 1:length(topos)
%     topo_d = [];
%     topo_l = [];
%     for j = 1:10
%         tmp_d = [];
%         tmp_l = [];
%         for k = 1:length(prots)
%             filename = strcat(topos{i}, "/", prots{k}, "-", num2str(j), "Mbps.txt");
%             tmp_d = [tmp_d; avg(filename)];
%             tmp_l = [tmp_l; lr(filename)];
%         end
%         topo_d = [topo_d tmp_d];
%         topo_l = [topo_l tmp_l];
%     end
%     d = [d; topo_d];
%     l = [l; topo_l];
% end

x = [1:1:10];

% for i = 1:length(topos)
%     subplot (1,length(topos),i)
%     for k = 1:length(prots)
%         plot (x,d((i-1)*length(prots) + k,:), '-o', 'LineWidth',2); 
%         hold on;
%     end
% end
% ldg = legend ("dgr", "dw", "dwe", "ecmp", "ospf", Location="bestoutside");

for i = 1:length(topos)
    subplot (1, length(topos),i)
    for k = 1:length(prots)
        plot (x,l((i-1)*length(prots) + k,:), '-*','LineWidth',2);
        hold on;
    end
end

ldg = legend ("dgr", "dw", "dwe", "ecmp", "ospf", Location="bestoutside");

