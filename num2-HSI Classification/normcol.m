function [B] = normcol(A)
B = A./(ones(size(A,1),1)*sqrt(sum(A.^2,1)));
end