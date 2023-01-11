clc
clear
close all
topoNames = ["Abilene" "AT&T" "CERNET" "GEANT"];
topos = ["abilene" "att" "cernet" "geant"];
prots = ["dgr" "dw" "dwe" "ecmp" "lfid"];

for i = 1:length(topos)
    subplot (2,length(topos),i)
    for k = 1:length(prots)
        filename =strcat ("tcp/",topos{i}, "/", prots{k}, ".txt");
        X = load (filename);
%         h2 = cdfplot(X);
        [counts, centers] = hist (X, 1000);
        plot (centers, cumsum (counts)/10000, "LineWidth",2)
        hold on
    end
    axis([10,350,0,1.05])
    set (gca,'XScale','log', 'YScale', 'line');
    xlabel ('Delay (ms)', 'FontSize',18)
    ylabel ('CDF','FontSize',18);
    grid off
    title(topoNames{i});
end
% ldg = legend ("DGR", "DW", "DWE", "ECMP", "LFID", Location="bestoutside");

for i = 1:length(topos)
    subplot (2,length(topos),i+4)
    for k = 1:length(prots)
        filename =strcat ("udp/",topos{i}, "/", prots{k}, ".txt");
        X = load (filename);
%         h2 = cdfplot(X);
        [counts, centers] = hist (X, 1000);
        plot (centers, cumsum (counts)/10000,"LineWidth",2);

        hold on
    end
    axis([10,350,0,1.05])
    set (gca,'XScale','log', 'YScale', 'line');
    xlabel ('Delay (ms)','FontSize',18)
    ylabel ('CDF','FontSize',18);
    grid off
    title(topoNames{i});
end
ldg = legend ("DGR", "DW", "DWE", "ECMP", "LFID", Location="bestoutside");