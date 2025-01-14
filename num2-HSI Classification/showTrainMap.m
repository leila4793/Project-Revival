function [] = showTrainMap(no_lines, no_rows, train_SL, imageName)

mask = zeros(no_rows*no_lines, 1);

mask(train_SL(1,:))=train_SL(2,:);

results = reshape (mask, [no_lines, no_rows]);
map = label2color(results, lower(imageName));

figure, imshow(map);

end



