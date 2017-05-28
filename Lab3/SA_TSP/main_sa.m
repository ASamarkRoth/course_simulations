
clc;
clear all;

% Choose which data file you wish to experiment with.
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
loadatt48();
% now load the saved matrix of cities coordinates
load('cities.mat');


%The input arguments are
% INPUTCITIES         - The cordinates for n cities are represented as 2
%                       rows and n columns and is passed as an argument for
%                       SIMULATEDANNEALING.
% INITIAL_TEMPERATURE - The initial temperature to start the
%                       simulatedannealing process.
% COOLING_RATE        - Cooling rate for the simulatedannealing process. 
%                       Cooling rate should always be less than one.
% iter_per_temperature - number of iterations per temperature value
% THRESHOLD           - Threshold is the stopping criteria and it is the
%                       number of iterations.
% NUMBEROFCITIESTOSWAP- Specify the maximum number of pair of cities to
%                       swap. As temperature decreases the number of cities
%                       to be swapped decreases and eventually reaches one
%                       pair of cities.
%simulatedannealing(inputcities,initial_temperature,...
%    cooling_rate,iter_per_temperature,threshold,numberofcitiestoswap)

simulatedannealing(cities,2000,0.97,10, 10000,10);