function [indexes] = generateTrainIndexes(ground, percent)

groundLabels = ground(2,:);

numClasses = max(groundLabels);


indexes = [];
for i = 1:numClasses
   classIndexes = find(groundLabels == i);
   
    numPixels = length(classIndexes);
    numTrains = ceil(numPixels * percent / 100);
    
    if(numTrains < 10)
        numTrains = 10;
    end
    rperm = randperm(numPixels);
    
    indexes = [indexes classIndexes(rperm(1:numTrains))];
    
end
indexes = indexes';
end