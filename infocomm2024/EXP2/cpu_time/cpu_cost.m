X = categorical({'Abilene', 'AT&T', 'CERNET', 'GEANT'});
X = reordercats(X,{'Abilene', 'AT&T', 'CERNET', 'GEANT'});
% y = [326 2845 3303 1720;
%      914 11866 10618 5197];
% y = y';
% Y = y./1000;

Y = [1 2.8; 1 4.17; 1 3.21; 1 3.02] 

b = bar (X, Y);
hold on

ylabel ('Normalized cost - CPU time');
ylim([0 4.5])

% xtips1 = b(1).XEndPoints;
% ytips1 = b(1).YEndPoints;
% labels1 = string(b(1).YData);
% text(xtips1,ytips1,labels1,'HorizontalAlignment','center',...
%     'VerticalAlignment','bottom')

xtips2 = b(2).XEndPoints;
ytips2 = b(2).YEndPoints;
labels2 = string(b(2).YData);
text(xtips2,ytips2,labels2,'HorizontalAlignment','center',...
    'VerticalAlignment','bottom')

ldg = legend ("OSPF", "DDR");
set(ldg,'Box','off');
set(gca, 'FontSize',14);

grid on;