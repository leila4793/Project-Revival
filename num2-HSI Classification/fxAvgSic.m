function [avgsic] = fxAvgSic(mappedTrains, labels, A, Y, M, alpha)

[~, K] = size(A);
[~, Len] = size(Y);
C = max(labels);

X = zeros(K, Len);

mappedAy = A' * Y;
[~, sortedLabels] = sort(abs(mappedAy), 'descend');

I = alpha * eye(M);

for k = 1:Len
    support = sortedLabels(2:M+1, k);
   % D = A(:, support);
  %  X(support, k) = (D' * D + alpha * eye(M)) \ (D' * Y(:,k));
   X(support, k) = (mappedTrains(support,support) + I) \ mappedAy(support,k);%(D' * Y(:,k));
end

errMatrix = zeros(C, Len);

for classIndex = 1:C
    ind = find(labels == classIndex);
    Yp = A(:,ind)*X(ind, :);
    errMatrix(classIndex,:) = sqrt(sum(abs(Y-Yp).^2,1));
end
[res, sorted] = sort(errMatrix);

res1 = res(1,:);
res2 = res(2,:);

ind = (labels ~= sorted(1,:));
res2(ind) = res1(ind);


trueResiduals = errMatrix(labels + (0:K-1)*C);


avgsic = mean(trueResiduals./res2);

end


