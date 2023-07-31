X = categorical({'Abliene', 'AT&T', 'CERNET', 'GEANT'});
X = reordercats(X,{'Abliene', 'AT&T', 'CERNET', 'GEANT'});
y = [326 2845 3303 1720;
     914 11866 10618 5197];
y = y';
Y = y./1000;
bar (X, Y);
hold on


ylabel ('CPU time (ms)');
% xlabel ('Topology name');

legend('OSPF','DDR');