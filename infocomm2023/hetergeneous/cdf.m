% clc;
% clear all;
% a = load ("geant/dgr.txt");
% b = load ("geant/dw.txt");
% c = load ('geant/dwe.txt');
% d = load ("geant/ecmp.txt");
% e = load ("geant/lfid.txt");
% h1 = cdfplot(a);
% hold on
% h2 = cdfplot(b);
% hold on
% h3 = cdfplot(c);
% hold on
% h4 = cdfplot(d);
% hold on
% h5 = cdfplot(e);
% hold on
% % axis([23,60,0,1.1])
% % set(gca,'YTick',[0:10:500])
% legend('DGR','DW','DWE','ECMP','LFID');
% xlabel ('Delay Requirement (ms)')
% ylabel ('On time rate');
% 


clc
clear
close all
topos = ["abilene" "att" "cernet" "geant"];
toposName = ["Abilene" "AT&T" "CERNET" "GEANT"];
prots = ["dgr" "dw" "dwe" "ecmp" "lfid"];
axisx = [20 40 20 60 10 35 15 35];
for i = 1:length(topos)
    subplot (1,length(topos),i)
    for k = 1:length(prots)
        kt = k;
        if k == 2
            kt = k + 3;
        end
        filename =strcat (topos{i}, "/", prots{kt}, ".txt");
        x = load (filename);
        h = cdfplot (x);
%         set (h, "LineWidth",2);
        grid off
        hold on
    end
    axis([axisx(i*2-1),axisx(i*2),0,1.05]);
    xlabel ('Delay Requirment (ms)')
    ylabel ('On Time Rate');
    title(toposName{i});
end

ldg = legend ("DGR", "DW", "DWE", "ECMP", "LFID", Location="bestoutside");

