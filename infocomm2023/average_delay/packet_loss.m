clc;
clear all;
x = load('data/dgr_packet_loss.txt');
y = load('data/ospf_packet_loss.txt');
z = load('data/dgr2_packet_loss.txt')
rate = 1:1:10;
plot (x);
hold on;
plot (y);
hold on;
plot (z);
legend('DGR','OSPF', 'DGR2');
xlabel ('Sending Rate (Mbps)')
ylabel ('packet loss');