function [budget, rate] = data_processor(filename)
    %DATA_PROCESSOR:
    X = load (filename);
    count = length(find(X==1));
    if (isempty(X))
        budget = 0;
    else
        budget = X(1)/1000;
    end
    rate = count/1000;
end