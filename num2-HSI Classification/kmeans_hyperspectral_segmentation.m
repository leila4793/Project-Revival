% Uses kmeans() to classify 6 spectral images into 6 unique color classes.  Those images will of course not be the same as the input images.
clc;    % Clear the command window.
close all;  % Close all figures (except those of imtool.)
clear;  % Erase all existing variables. Or clearvars if you want.
workspace;  % Make sure the workspace panel is showing.
format long g;
format compact;
fontSize = 13;

%===============================================================================
% Read in a demo image.
% Specify the folder where the files live.
myFolder = pwd;
% Check to make sure that folder actually exists.  Warn user if it doesn't.
if ~isdir(myFolder)
  errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
  uiwait(warndlg(errorMessage));
  return;
end
% Get a list of all files in the folder with the desired file name pattern.
filePattern = fullfile(myFolder, 'kmeans hyperspectral*.bmp'); % Change to whatever pattern you need.
theFiles = dir(filePattern);
numberOfFiles = length(theFiles);
plotRows = ceil(sqrt(numberOfFiles));
for k = 1 : length(theFiles)
  baseFileName = theFiles(k).name;
  fullFileName = fullfile(myFolder, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  % Load image array with imread()
  spectralImage = imread(fullFileName);
  % It is supposed to be monochrome because it's a spectral image. Get the dimensions of the image.
  % numberOfColorChannels should be = 1 for a gray scale image, and 3 for an RGB color image.
  [rows, columns, numberOfColorChannels] = size(spectralImage);
  if numberOfColorChannels > 1
	  % It's not really gray scale like we expected - it's color.
	  % Use weighted sum of ALL channels to create a gray scale image.
	  spectralImage = rgb2gray(spectralImage);
	  % ALTERNATE METHOD: Convert it to gray scale by taking only the green channel,
	  % which in a typical snapshot will be the least noisy channel.
	  % grayImage = grayImage(:, :, 2); % Take green channel.
  end
  subplot(plotRows, plotRows, k);
  imshow(spectralImage);  % Display image.
  drawnow; % Force display to update immediately.
  title(baseFileName, 'FontSize', fontSize);
  
  % Load into columns of an array for kmeans()
  % Get the data for doing kmeans.  We will have one column for each color channel.
  if k == 1
	  data = double(spectralImage(:));
  else
	  data = [data, double(spectralImage(:))];
  end
end
% Enlarge figure to half screen.
set(gcf, 'Units', 'Normalized', 'OuterPosition', [0, 0.04, 0.5, 0.96]);
drawnow;
hp = impixelinfo();

% %----------------------------------------------------------------------------------------
% % Ask user how many color classes they want.
% defaultValue = 6;
% titleBar = 'Enter an integer value';
% userPrompt = 'Enter the number of color classes to find ';
% caUserInput = inputdlg(userPrompt, titleBar, 1, {num2str(defaultValue)});
% if isempty(caUserInput),return,end % Bail out if they clicked Cancel.
% % Round to nearest integer in case they entered a floating point number.
% numberOfClasses = round(str2double(cell2mat(caUserInput)));
% % Check for a valid integer.
% if isnan(numberOfClasses) || numberOfClasses < 2 || numberOfClasses > 6
% 	% They didn't enter a number.
% 	% They clicked Cancel, or entered a character, symbols, or something else not allowed.
% 	numberOfClasses = defaultValue;
% 	message = sprintf('I said it had to be an integer.\nTry replacing the user.\nI will use %d and continue.', numberOfClasses);
% 	uiwait(warndlg(message));
% end
numberOfClasses = 6; % Assume 6 for demo.

%----------------------------------------------------------------------------------------
%  KMEANS CLASSIFICATION RIGHT HERE!!!
% Each row of data represents one pixel.  Each column of data represents one image.
% Have kmeans decide which cluster each pixel belongs to.
indexes = kmeans(data, numberOfClasses);

%----------------------------------------------------------------------------------------
% Let's convert what class index the pixel is into images for each class index.
% Assume 6 clusters.
class1 = reshape(indexes == 1, rows, columns);
class2 = reshape(indexes == 2, rows, columns);
class3 = reshape(indexes == 3, rows, columns);
class4 = reshape(indexes == 4, rows, columns);
class5 = reshape(indexes == 5, rows, columns);
class6 = reshape(indexes == 6, rows, columns);
% Let's put these into a 3-D array for later to make it easy to display them all with a loop.
allClasses = cat(3, class1, class2, class3, class4, class5, class6);
allClasses = allClasses(:, :, 1:numberOfClasses); % Crop off just what we need.
% OK!  WE'RE ALL DONE!.  Nothing left now but to display our classification images.
figure;
plotRows = ceil(sqrt(size(allClasses, 3)));
fontSize = 20;
% Display the classes, both binary and masking the original.
% Also make an indexes image so we can display each class in a unique color.
indexedImageK = zeros(rows, columns, 'uint8'); % Initialize another indexed image.
for c = 1 : numberOfClasses
	% Display binary image of what pixels have this class ID number.
	subplot(plotRows, plotRows, c);
	thisClass = allClasses(:, :, c);
	imshow(thisClass);
	caption = sprintf('Image of\nClass %d Indexes', c);
	title(caption, 'FontSize', fontSize);
end
% Enlarge figure to full screen.
set(gcf, 'Units', 'Normalized', 'OuterPosition', [0.5, 0.04, 0.5, 0.96]);

message = sprintf('Done with Classification');
helpdlg(message);

