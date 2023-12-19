clc
clear
close all
% set(gca, 'FontSize',11);
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

% add(0,0) to third subfig
budget(501) = 6.8;
% budget(201) = 0;
% budget(251) = 0;
% budget(301) = 21.4;
% budget(351) = 0;

% output data
fileoutput = fopen('data1.txt', 'w');
formatSpec = '%f\t';



for i = 1:50:800
    fprintf(fileoutput, formatSpec, rate(i:i+49));
    fprintf(fileoutput, '\n');
    fprintf(fileoutput, formatSpec, budget(i:i+49));
    fprintf(fileoutput, '\n');
end

fclose(fileoutput);

% input data
fileinput = fopen('data.txt', 'r');
formatSpec = '%f';
data = fscanf(fileinput, '%f');
fclose(fileinput);

for i = 1:50:800
    newrate(i:i+49) = data(2*i-1:2*i-1+49);
    newbudget(i:i+49) = data(2*i+49:2*i+49+49);
end

newrate = newrate' ;
newbudget = newbudget' ;

% result = isequal(newrate, rate);
% 
% if result
%     disp('A = B');
% else
%     disp('A NOT = B');
% end

topo_label = ["Abilene - Basic route delay 23ms" "AT&T - Basic route delay 20ms" "CERNET - Basic route delay 5ms" "GEANT - Basic route delay 25ms"];

toponame = ["Abilene" "AT&T" "CERNET" "GEANT"];
% figure;



for i = 1:length(topos)
%     title(toponame(i));
    subplot (1, length(topos),i);
    for k = 1:length(prots)
        first = (i-1)*200 + (k-1)*50 + 1;
        last = first + 49;
        x = newbudget (first:last);
        y = newrate (first:last);
        plot (x, y,'LineWidth',2);		
        ylim([-0.05 1.05])
        grid on;
        hold on;
    end
%     title(toponame(i));
xlabel ({'Delay requirements (ms)', topo_label(i)});

end

ylabel ('On-time delivery ratio (%)');
% legend('Orientation','horizontal');
ldg = legend ("ECMP", "LFID", "DGRP", "DDR", Location="east");
set(ldg,'Orientation','horizon')
set(ldg,'Box','off');



