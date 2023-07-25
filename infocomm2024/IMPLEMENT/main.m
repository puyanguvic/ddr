queue = 20;
link = 4;
X = [];
Y = [];
for i=0.1:0.1:20
    scale = i;
    err = errorCalculator (queue, link, scale);
    X = [X i];
    Y = [Y err];
end

plot(X,Y);
