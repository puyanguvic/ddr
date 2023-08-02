queue = 20;
link = 4;
X = [];
Y = [];
for i=0.1:0.1:19.9
    scale = i;
    err = errorCalculator (queue, link, scale);
    X = [X i];
    Y = [Y err];
end

[min_Y1, min_index] = min(Y);
min_X1 = X(min_index);

figure
plot(X,Y,'-','LineWidth',2);
hold on


% clear;

queue = 20;
link = 6;
X = [];
Y = [];
for i=0.1:0.1:19.9
    scale = i;
    err = errorCalculator (queue, link, scale);
    X = [X i];
    Y = [Y err];
end

[min_Y2, min_index] = min(Y);
min_X2 = X(min_index);


plot(X,Y,'LineWidth',2);
hold on

% clear;

queue = 20;
link =8;
X = [];
Y = [];
for i=0.1:0.1:19.9
    scale = i;
    err = errorCalculator (queue, link, scale);
    X = [X i];
    Y = [Y err];
end

[min_Y3, min_index] = min(Y);
min_X3 = X(min_index);


plot(X,Y,'LineWidth',2);
hold on

% clear;

queue = 20;
link = 10;
X = [];
Y = [];
for i=0.1:0.1:19.9
    scale = i;
    err = errorCalculator (queue, link, scale);
    X = [X i];
    Y = [Y err];
end

[min_Y4, min_index] = min(Y);
min_X4 = X(min_index);


plot(X,Y,'LineWidth',2);
hold on

plot(min_X1, min_Y1, 'o','MarkerSize',10);
hold on

plot(min_X2, min_Y2, 'o','MarkerSize',10);
hold on

plot(min_X3, min_Y3, 'o','MarkerSize',10);
hold on

plot(min_X4, min_Y4, 'o','MarkerSize',10);
hold on

ldg = legend('delay = 4ms', 'delay = 6ms',  'delay = 8ms', 'delay = 10ms')
% set(ldg,'Box','off');

set(gca,'FontSize',12);
set(gca,'XTick',0:2:20)

xlabel ('Scale (ms)')
ylabel ('Error (ms)')

grid on;
