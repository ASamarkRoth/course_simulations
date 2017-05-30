% Choose which data set you wish to experiment with.
% Below is the list of the function calls generating different 
% data files:

%data_file();
%loadatt48();
%loadbays29();
%loadeil101();
%loadeil535();
%loadgr96();
%loadpcb442();
%loadpr76();
%loadst70();


% we choose the second data file for this example
loadst70();

%% prepare the distance matrix and keep it in d
load('cities.mat');
xy = cities';

%% parameters:
%xy                           - x and y coordinates of cities
%popSize                      - size of population
%numIter                      - maximum number of iterations
%showProg and showResult      - to turn on/off plots
%showWaitbar                  - to turn on/off wait bar


userConfig = struct('xy', xy, 'popSize', 100, 'numIter', 1000, 'showProg',true,'showResult',true,'showWaitbar',true);
resultStruct = tsp_ga(userConfig);