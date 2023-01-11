clear all;
clc;
% % ydata = load ("abilene1to10background.delay");
% ydata = load ("abilene1to10.txt");
% bins = 10;
% maxdat = max(ydata);
% mindat = min(ydata);
% bin_space = (maxdat - mindat)/bins;
% xtick = mindat : bin_space : (maxdat - bin_space);
% distribution = hist(ydata, bins);
% pdf = bins*distribution / ((sum(distribution)) * (maxdat - mindat));
% figure;
% plot (xtick, pdf);

% sum(pdf)*bin_space

X = load ("abilene1to10background.delay");
Y = load ("abilene1to10.txt");
[counts, centers] = hist (X, 1000);
plot (centers, cumsum (counts)/10000,"LineWidth",2);
hold on
[counts2,centers2] = hist (Y, 1000);
plot (centers2, cumsum (counts2)/10000, "LineWidth",2);
hold on
xlabel ('Delay (ms)')
ylabel ('CDF');
grid off
ldg = legend ("Before link failure", "After link failure");