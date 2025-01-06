function [Ip] = rectangular_to_triangular(I,p)
%
%    [Ip] = RECTANGULAR_TO_TRIANGULAR(I,p)        returns an image
%                                                 intensities resampled
%                                                 at the node points of
%                                                 the triangular FEM grid           
%
%    Description: Given an input image and a set of node points, this
%    function returns the resampled image intensities at the input node
%    points. The resampling is performed by MATLABs interp2() using the
%    default bilinear interpolation.
%
%    Input : I  --> 2D matrix a gray scale image             (LENGTH X WIDTH)
%            p  --> 2D matrix of FEM grid nodes              (N_nodes X 2)
%          
%    Output: Ip --> 1D vector of resampled image intensities (N_nodes X 1)
%  
%
  
  [LENGTH,WIDTH] = size(I);
  
  %---Sample the image at the node points----%
  [Xidx,Yidx] = meshgrid(1:1:WIDTH,1:1:LENGTH); 
  
  Ip = interp2(Xidx,Yidx,I,p(:,1),p(:,2));
  temp_idx = find(isnan(Ip) == 1);
  Ip(temp_idx) = 0;
    
    
  
    