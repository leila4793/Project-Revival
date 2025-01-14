function [] = showMap(no_lines, no_rows, indexes, labels, imageName)

mask = zeros(no_rows*no_lines, 1);

mask(indexes)=labels;

results = reshape (mask, [no_lines, no_rows]);
map = label2color(results, lower(imageName));

figure, imshow(map);

end



