clear;

data = load('TcpCubic-cwnd.txt');
X = data(:,1);
Y = data(:,2);

plot(X, Y);
hold on;

clear;

data = load('TcpBbr-cwnd.txt');
X = data(:,1);
Y = data(:,2);

plot(X, Y);
hold on;

clear;

data = load('TcpVegas-cwnd.txt');
X = data(:,1);
Y = data(:,2);

plot(X, Y);
hold on;

clear;

data = load('TcpDctcp-cwnd.txt');
X = data(:,1);
Y = data(:,2);

plot(X, Y);
hold on;