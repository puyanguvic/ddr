% clear all;
% clc;
% % ydata = load ("abilene1to10background.delay");
ydata = load ("att/dwe.txt");
% bins = 100;
% maxdat = max(ydata);
% mindat = min(ydata);
% bin_space = (maxdat - mindat)/bins;
% xtick = mindat : bin_space : (maxdat - bin_space);
% distribution = hist(ydata, bins);
% pdf = bins*distribution / ((sum(distribution)) * (maxdat - mindat));
% figure;
% plot (xtick, pdf);
% 
% sum(pdf)*bin_space
tabulate (ydata)