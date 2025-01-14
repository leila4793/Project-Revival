function [] = showGround(no_lines, no_rows, ground, imageName)

mask = zeros(no_rows*no_lines, 1);

mask(ground(1,:))=ground(2,:);

results = reshape (mask, [no_lines, no_rows]);
map = label2color(results, lower(imageName));

figure, imshow(map);

end