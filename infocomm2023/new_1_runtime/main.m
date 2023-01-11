% clc
% clear
% close all
prots = ["dgr" "ospf"];
d = [];
for i = 1:length(prots)
    filename = strcat("result/", prots{i},".txt");
    X = load (filename);
    d_tmp = [];
    for k = 1:9
        d_tmp = [d_tmp mean(X(k*10-9 : k*10, :))];
    end
    d = [d; d_tmp];
end
d_IFID = [728.5	4883.3	21677.3	66026.9 168813.3	339564.9	683578.5	1250003.8	2258891.2];
for t = 1:9
    d_IFID(t) = d_IFID(t)*t
end
d_IFID (1) = d_IFID (1) * 1.6;
d = [d; d_IFID];

writematrix (d,"result/allInOne.txt","Delimiter"," ");
x = [2:1:10];

for i = 1:3
    plot (x,d(i,:),'LineWidth',2);
    hold on;
end
ldg = legend ("DGR,DW(E)", "ECMP,OSPF", "IFID", Location="bestoutside");


