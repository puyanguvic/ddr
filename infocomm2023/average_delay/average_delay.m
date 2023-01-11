clc;
clear all;
x = load('data/dgr_average_delay.txt');
y = load('data/ospf_average_delay.txt');
z = load('data/dgr2_average_delay.txt');
rate = 1:1:10;
% plot (rate,x,'-b*');
semilogy (x);
hold on;
semilogy (y);
hold on;
semilogy (z);
% plot (rate, y,'-ro');
% axis([0,23,0,20])
% set(gca,'XTick',[0:1:10])
% set(gca,'YTick',[0:10:500])
legend('DGR','OSPF','DGR2');
xlabel ('Sending Rate (Mbps)')
ylabel ('Average Delay');