function [btcResult, errMatrix] = fxBtc(labels, A, Y, M, alpha)
[~, Len] = size(Y);
step = 5e3;
times = fix(Len/step);
C = max(labels);
btcResult = zeros(1,Len);
errMatrix = zeros(C,Len);


mappedTrains = A'*A;

for ii = 1:times
    index = (ii-1)*step+1:ii*step;
    [res, err] = fxBt(mappedTrains, labels, A, Y(:,index), M, alpha);
    btcResult(index) = res;
    errMatrix(:,index) = err;

end
index = step*times+1:Len;

[res, err] = fxBt(mappedTrains,labels, A, Y(:,index), M, alpha);
btcResult(index) = res;
errMatrix(:,index) = err;


btcResult = btcResult';


end