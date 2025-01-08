function [out] =  triangular_to_rectangular(Ip,i,p,t,LENGTH,WIDTH)

[Xidx,Yidx] = meshgrid(1:1:WIDTH,1:1:LENGTH);
h = fspecial('gaussian',[5*ceil(1) 5*ceil(1)],1);
x = [Xidx(:) Yidx(:)];
N_pts = size(x,1);

N_tri   = size(t,1);
N_nodes = size(p,1);
N_cells = size(x,1);


d = imfilter(i,h);
out = wdenoise(d);

end
    
  
    