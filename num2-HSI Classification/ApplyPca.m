function [guidanceImage] = ApplyPca(img, dim)

[r,c,b]=size(img);

im=reshape(img,[r*c b]);

im = im';

pixels = size(im,2);

m = mean(im,2);
im_hat = im - m*ones(1,pixels);

C = (im * im')/pixels - m * m';

[V,D] = eig(C);

[~, index] = sort(diag(D),'descend');

V = V(:,index(1:dim));

im_reduced = mat2gray(V'*im_hat)';

guidanceImage =reshape(im_reduced,[r,c,dim]);

end
