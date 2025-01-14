function v = convert_im_to_vector(im)
sz = size(im);
v = reshape(im, [prod(sz(1:2)) sz(3)]);