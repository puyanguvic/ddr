clc;
clear all;
X = load ("abilene/dgr-1Mbps.txt");
Y = load ('abilene/ospf-1Mbps.txt');
Z = load ("abilene/ecmp-1Mbps.txt");
H = load ("abilene/dgr-1Mbps_2.txt");
h1 = cdfplot(X);
hold on
h2 = cdfplot(Y);
hold on
h3 = cdfplot(Z)
hold on
% h4 = cdfplot(H);
% hold on
axis([0,200,0,1])
% set(gca,'YTick',[0:10:500])
% legend('DGR','OSPF');
xlabel ('delay')
ylabel ('CDF');