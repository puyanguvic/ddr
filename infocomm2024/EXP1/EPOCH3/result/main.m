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

budget(501) = 6.8;
budget(714) = 27.5;

% output data
fileoutput = fopen('data.txt', 'w');
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
%     disp('数组A和B相同');
% else
%     disp('数组A和B不相同');
% end


figure;
for i = 1:length(topos)
    subplot (1, length(topos),i)
    for k = 1:length(prots)
        first = (i-1)*200 + (k-1)*50 + 1;
        last = first + 49;
        x = newbudget (first:last);
        y = newrate (first:last);
        plot (x, y,'LineWidth',2);
		xlabel ('Time Limit (ms)');
        ylabel ('Arrival Rate (%)');
        hold on;
    end
end

% ylabel ('Arrival Rate (%)');
ldg = legend ("ECMP", "LFID", "DGR", "DDR", Location="bestoutside");
