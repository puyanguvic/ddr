function ret = avg(filename)
X = load(filename);
ret [];
for i = 1:9
   ret = [ret mean(X(i*10-9 : i*10, :))]
end
end
