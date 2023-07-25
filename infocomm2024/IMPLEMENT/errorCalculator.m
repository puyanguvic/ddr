function err = errorCalculator(queue, link, scale)
    size = ceil(queue/scale);
    C = zeros(size,size);
    begin = 0;
    state = 1;
    for i = 1: 100000
        r = rand();
        r = link* (r-0.5)*2;
        begin = begin + r;
        if begin < 0
            begin = 0.001;
        end
        if begin > queue
            begin = queue - 0.001;
        end
        predict = ceil(begin/scale);
        C(state, predict) = C(state, predict) + 1;
        state = predict;
    end

    CM = zeros (size, size);
    for i = 1:size
        for j = 1:size
            s = sum(C(:,j),"all");
            CM(i,j) = C(i,j)/s;
        end
    end
    
    PredCM = eye (size);
    step = ceil(link/scale);
    for i = 1 : step
        PredCM = PredCM*CM;
    end

    err = 0;
    for i = 1:size
        for j = 1:size
            bias = abs(j - i);
            err = err + PredCM(i,j)*(bias + 0.5)*scale;
        end
    end
    
    err = err/size;
end