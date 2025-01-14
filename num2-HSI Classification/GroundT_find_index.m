function [out] = GroundT_find_index(GroundT)
num = max(max(GroundT));
GroundT_ind = [];
for c = 1 : num
    a = find(GroundT == c);
    b = double(c).*ones(1,length(a));
    c1 = [a';b];
    GroundT_ind = [GroundT_ind c1];
end
out = GroundT_ind;
end
    