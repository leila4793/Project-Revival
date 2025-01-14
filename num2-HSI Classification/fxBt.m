%   Distribution code Version 1.0 -- 01/01/2015 by Mehmet Altan Toksöz, Copyright 2015, Middle East Technical University, Turkey.
%
%   The Code is created based on the method described in the following papers: 
%   [1] M. A. Toksoz and I. Ulusoy, “Hyperspectral image classification via basic thresholding classifier,” 
%   IEEE Transactions on Geoscience and Remote Sensing, 2016, doi:10.1109/TGRS.2016.2535458.
%   [2] M. A. Toksoz and I. Ulusoy, “Classification via ensembles of basic thresholding
%   classifiers,” IET Computer Vision, 2016, doi:10.1049/ietcvi.2015.0077.
%   Please cite them.
%
%   The code and the algorithm are for non-comercial use only.


function [btcResult, errMatrix] = fxBt(mappedTrains, labels, A, Y, M, alpha)
% This function implements the basic thresholding classifier for any
% classification applications such as face recognition, hyperspectral
% image classification, etc.
%
% INPUTS:
% labels: 1 X N vector of integers consisting of class labels
% of the input training samples where N denotes the number of
% training samples.
% A: B X N dictionary whose columns represent L2 normalized training
% samples where B represents the number of features.
% Y: B X L matrix consisting of testing samples where L represents
% the number of testing samples. For fast computation, it is
% recommended that L is less than 10000. If it is larger, then Y can
% be partitioned.
% M: Threshold parameter (integer) which is less than B.
% alpha: Regularization parameter between 0 and 1.
%
% OUTPUTS:
% errMatrix: C X L error matrix containing residual for each sample
% btcResult: 1 X L decision vector containing predicted labels



[~, N] = size(A);
[~, L] = size(Y);
C = max(labels);
X = zeros(N, L);
mappedAy = A' * Y;
[~, sortedLabels] = sort(abs(mappedAy), 'descend');
I = alpha * eye(M);
for k = 1:L
    support = sortedLabels(1:M, k);
    X(support, k) = (mappedTrains(support,support) + I) \ mappedAy(support,k);
end
errMatrix = zeros(C, L);
for classIndex = 1:C
    ind = (labels == classIndex);
    Yp = A(:,ind)*X(ind, :);
    errMatrix(classIndex,:) = sqrt(sum((Y-Yp).^2,1));
end
[~, btcResult] = min(errMatrix, [], 1);
end


