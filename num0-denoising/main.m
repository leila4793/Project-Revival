clc
clear
close all

mex mex_make_local_basis.c
mex mex_fast_map_points_to_triangles.c
mex mex_interpolate_element.c

pde_model
p = load('p.mat'); p = p.p;
e = load('e.mat'); e = e.e;
t = load('t.mat'); t = t.t;


[p,t] = fix_mesh(p,t);

I = double(imread('office_noisy.jpg')); 

[LENGTH,WIDTH] = size(I);

[Ip] = rectangular_to_triangular(I,p);

I_diffused = load('I_diffused.mat'); I_diffused = I_diffused.I_diffused;
clear_imge = triangular_to_rectangular(Ip,I,p,t,LENGTH,WIDTH);

figure
subplot(1,2,1)
imshow(I,[])
title('Original Image')
subplot(1,2,2)
imshow(clear_imge,[])
title('Diffused Image')
  
