% clc
% clear
% close all
% tcp = ["Bbr" "Cubic" "Dctcp" "Vegas"];
% prots = ["OSPF" "DDR"];
% throughput = [];
% time = [];
% for i = 1:length(prots)
%     for k = 1:length(tcp)
%         filename = strcat(prots{i}, "-Tcp", tcp{k}, "-throughput.txt");
%         data = load('DDR-TcpCubic-throughput.txt');
%         X = data(:,1);
%         Y = data(:,2);
%         throughput = [throughput;Y];
%         time = [time;X];
%     end
% end
% 
% figure;
% 
% 
% for i = 1:length(prots)
%     for k = 1:length(tcp)
%         first = (i-1)*200 + (k-1)*50 + 1;
%         last = first + 49;
%         x = newbudget (first:last);
%         y = newrate (first:last);
%         plot (x, y,'LineWidth',2);
%         ylabel ('On-time Arrival Rate (%)');
% 		xlabel ('Time Limit (ms)');
%         ylim([-0.05 1.05])
%         grid on;
%         hold on;
%     end
% end
% 
% 
% % legend('Orientation','horizontal');
% ldg = legend ("ECMP", "LFID", "DGR", "DDR", Location="north");
% set(ldg,'Orientation','horizon')
% set(ldg,'Box','off');
% set(gca, 'FontSize',11);












clear;

data = load('DDR-TcpCubic-throughput.txt');
X = data(:,1);
Y = data(:,2);

plot(X, Y);
hold on;

clear;

data = load('DDR-TcpBbr-throughput.txt');
X = data(:,1);
Y = data(:,2);

plot(X, Y);
hold on;

clear;

data = load('DDR-TcpVegas-throughput.txt');
X = data(:,1);
Y = data(:,2);

plot(X, Y);
hold on;

clear;

data = load('DDR-TcpDctcp-throughput.txt');
X = data(:,1);
Y = data(:,2);

plot(X, Y);
hold on;