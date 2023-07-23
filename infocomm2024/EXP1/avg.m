function [average] = avg(filename)
X = load(filename);
average = mean (X);
end