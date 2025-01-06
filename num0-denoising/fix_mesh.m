function [p,t] = fix_mesh(p,t)

p = p';
t = t';
t = t(:,1:3);
end

  