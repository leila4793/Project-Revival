function [elapsedTime, errorCube] = fxBtcPixel(dataset, M, alpha)

%A: normalized train matrix
%Y: all samples
[xDim, yDim, zDim] = size(dataset.HyperCube);
A = dataset.NormalizedTrainMatrix;
Y = dataset.AllSamples;
numClasses = dataset.NumClasses;
numTrains = dataset.NumTrains;

errorCube = zeros(xDim, yDim, numClasses);

[~, K] = size(A);
errVector = zeros(1,1,numClasses);

k = 1;
percent = 0;
tic;
for i = 1:xDim
    for j = 1:yDim
        CV = abs(A' * Y(:,k));
        [~, sortedLabels] = sort(CV, 'descend');
        support = sortedLabels(1:M);
        
        D = A(:, support);
        x = zeros(K, 1);
        x(support) = (D' * D + alpha * eye(M))^(-1) * D' * Y(:,k);
        
        startIndex = 1;
        
        for classIndex = 1:numClasses
            endIndex = startIndex + numTrains(classIndex) - 1;
            yp = A(:,startIndex : endIndex)*x(startIndex : endIndex);
            errVector(1,1,classIndex) = norm(Y(:,k)-yp,2);
            startIndex = endIndex + 1;
        end
        errorCube(i,j,:) = errVector;
        k = k + 1;
    end
    percent = percent + 1;
    clc;
    disp(percent/xDim*100);
end
elapsedTime = toc;

end