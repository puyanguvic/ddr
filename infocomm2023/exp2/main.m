clc
clear
close all
topos = ["abilene" "att" "cernet" "geant"];
prots = ["dgr" "dw" "dwe" "ecmp" "ospf"];

for i = 1:length(topos)
    subplot (1,length(topos),i)
    for k = 1:length(prots)
        kt = k;
        if k == 2
            kt = k + 3;
        end
        filename =strcat (topos{i}, "/", prots{kt}, ".txt");
        x = load (filename);
        h1 = cdfplot(x);
        hold on
    end
    axis([0,150,0,1]);
    xlabel ('delay')
    ylabel ('CDF');
    title(topos{i});


end
ldg = legend ("dgr", "dw", "dwe", "ecmp", "ospf", Location="bestoutside");

