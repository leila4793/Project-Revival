function [normalizedTrainMatrix] = fxNormalizeDataset(trainMatrix)

%normalization
normTrainMatrix = sqrt(diag(trainMatrix' * trainMatrix));
normTrainMatrix(normTrainMatrix == 0) = 0.001;
normalizedTrainMatrix = bsxfun(@rdivide, trainMatrix, normTrainMatrix');


end