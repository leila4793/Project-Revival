 function D = imdiv(I)
Ix = diff([I I(:, end)]')';
Iy = diff([I; I(end, :)]);
[X,Y] = meshgrid(1:size(I, 2), 1:size(I, 1));
D = divergence(X, Y, Ix, Iy);
 end