function [avgBeta, bestM] = averageBeta(labels, A, alpha)

[B, N] = size(A);
C = max(labels);
avgBeta = zeros(1, B-1);
mappedTrains = A' * A;
[~, sortedLabels] = sort(abs(mappedTrains), 'descend');

for m=1:B-1
    X = zeros(N, N);
    I = alpha * eye(m);
    for k = 1:N
        support = sortedLabels(2:m+1, k);
        X(support, k) = (mappedTrains(support,support) + I) \...
            mappedTrains(support,k);
    end
    
    errMatrix = zeros(C, N);
    for classIndex = 1:C
        ind = (labels == classIndex);
        Yp = A(:,ind)*X(ind, :);
        errMatrix(classIndex,:) = sqrt(sum(abs(A-Yp).^2,1));
    end
    
    [res, sorted] = sort(errMatrix);
    res1 = res(1,:);
    res2 = res(2,:);
    ind = (labels ~= sorted(1,:));
    res2(ind) = res1(ind);
    trueResiduals = errMatrix(labels + (0:N-1)*C);
    avgBeta(m) = mean(trueResiduals./res2);
    
end

[~, bestM] = min(avgBeta); 

end


