X = categorical({'Abliene', 'AT&T', 'CERNET', 'GEANT'});
X = reordercats(X,{'Abliene', 'AT&T', 'CERNET', 'GEANT'});
y = [326 2845 3303 1720;
     914 11866 10618 5197];
y = y';
Y = y./1000;

h = bar (X, Y);
hold on

ylabel ('CPU time (ms)');
ylim([0 13])

legend('OSPF','DDR');

for i = 1:numel(Y)
    for j = 1:size(Y, 2)
        text(X(i), Y(i, j) + 0.5, num2str(Y(i, j)), ...
            'HorizontalAlignment', 'center', 'VerticalAlignment', 'bottom');
    end
end

