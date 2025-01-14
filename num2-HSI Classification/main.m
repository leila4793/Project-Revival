clc
clear all
close all
% %%
data = [];
imageName = 'Indian_pines';
img = importdata([imageName,'.mat']);
img_pca = ApplyPca(img,30);
GroundT = importdata([imageName,'_gt.mat']);
[rows, columns, numberOfColorChannels] = size(img_pca);
for i = 1: numberOfColorChannels
    img_ = img_pca(:,:,i);
     data = [data, double(img_(:))];
end
numberOfClasses = max(max(GroundT));
GroundT = GroundT_find_index(GroundT);
indexes = kmeans(data, numberOfClasses);
class_kmean = [];
for i = 1:numberOfClasses
    calss = reshape(indexes == i, rows, columns);
    class_kmean(:,:,i) = calss;
end
allClasses = cat(3, class_kmean);
allClasses = allClasses(:, :, 1:numberOfClasses);
figure;
plotRows = ceil(sqrt(size(allClasses, 3)));
fontSize = 10;
indexedImageK = zeros(rows, columns, 'uint8'); 
for c = 1 : numberOfClasses
	subplot(plotRows, plotRows, double(c));
	thisClass = allClasses(:, :, double(c));
	imshow(thisClass);
	caption = sprintf('Image of\nClass %d Indexes', c);
	title(caption, 'FontSize', fontSize);
end
set(gcf, 'Units', 'Normalized', 'OuterPosition', [0.5, 0.04, 0.5, 0.96]);
[no_lines, no_rows, no_bands] = size(img_pca);
class_kmean = convert_im_to_vector(img_pca);
class_kmean = class_kmean';

[indexes_svm] = generateTrainIndexes(GroundT, 10);
trainIndexes = GroundT(1,indexes_svm);
trainLabels = GroundT(2,indexes_svm);
groundIndexes = GroundT(1,:);
groundLabels = GroundT(2,:);
testIndexes = groundIndexes;
testLabels = groundLabels;
testIndexes(indexes_svm) = [];
testLabels(indexes_svm) = [];
trainSamples = class_kmean(:, trainIndexes)';

testSamples = class_kmean(:, testIndexes)';
numLabel = [];
numTestLabel = [];
for jj =1 : numberOfClasses
    numLabel = [numLabel; length(find(trainLabels==jj))];
    numTestLabel = [numTestLabel; length(find(testLabels==jj))];
end
disp(numLabel)
disp(numTestLabel)


img = class_kmean';
trainSamples = trainSamples';

trainSamples = normcol(trainSamples);

[avgBeta, bestM] = averageBeta(trainLabels, trainSamples, 1e-4);
figure;
plot(avgBeta);
disp(bestM)

kernel_func='rbf';
% [itrfin] = multisvm( trainSamples,trainLabels, data(1:round(.7*size(data,1)),:), kernel_func );
tic;
[classresult, errMatrix] = fxBtc(trainLabels, trainSamples, img', 100, 1e-4);

elapsedTime =toc;
disp(['time = ',num2str(elapsedTime)]);


% evaluate the results
[classOA,classAA,classkappa,classCA]=confusion(testLabels, classresult(testIndexes)');
sprintf('%0.2f\n',classCA*100)

showMap(no_lines, no_rows, groundIndexes, 10.*classresult(groundIndexes), imageName);

mask = zeros(size(classresult));
mask(groundIndexes)=1;
classresult = 10.*mask.*classresult;
classresult = reshape(classresult,no_lines,no_rows);
figure
GroundT = importdata([imageName,'_gt.mat']);
subplot(1,2,1)
imshow(10.*GroundT,[]); title('GroundT image')
subplot(1,2,2)
imshow(classresult,[]); title('after classification results')




