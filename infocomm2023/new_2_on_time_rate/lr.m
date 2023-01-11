function [lossRate] = lr(filename)
X = load(filename);
n = length(X);
lossRate = 1 - n/10000;
end