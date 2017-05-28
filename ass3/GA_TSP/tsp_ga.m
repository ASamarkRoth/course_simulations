
function varargout = tsp_ga(varargin)

% Initialize default configuration (do not change this part)
defaultConfig.xy          = 10*rand(50,2); % xy is the coordinate matrix
defaultConfig.dmat        = [];            % dmat is the distance matrix
defaultConfig.popSize     = 200;
defaultConfig.numGen      = 2000;
defaultConfig.crossProb   = 0.25;
defaultConfig.mutProb     = 0.5;
defaultConfig.eliteFract  = 0.02;

% Interpret user configuration inputs (do not change this part)
if ~nargin
    userConfig = struct();
elseif isstruct(varargin{1})
    userConfig = varargin{1};
else
    try
        userConfig = struct(varargin{:});
    catch
        error('Expected inputs are either a structure or parameter/value pairs');
    end
end

% Override default configuration with user inputs (do not change this part)
configStruct = get_config(defaultConfig,userConfig);

% Extract configuration
xy          = configStruct.xy;   % xy is the coordinate matrix
dmat        = configStruct.dmat; % dmat is the distance matrix
popSize     = configStruct.popSize;
numGen      = configStruct.numGen;
crossProb   = defaultConfig.crossProb;
mutProb     = defaultConfig.mutProb;
eliteFract  = defaultConfig.eliteFract;

if isempty(dmat)
    nPoints = size(xy,1);
    a = meshgrid(1:nPoints);
    dmat = reshape(sqrt(sum((xy(a,:)-xy(a',:)).^2,2)),nPoints,nPoints);
end

% Verify Inputs (do not change this part)
[N,dims] = size(xy);
[nr,nc] = size(dmat);
if N ~= nr || N ~= nc
    error('Invalid XY or DMAT inputs!')
end
n = N; % make sure you do not use this varaible n for other puposes (e.g. for loop iteration)




%%%%%%%%%%%%%%%%% Initialize the Population %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%% You don't need to change this part%%%%%%%%%%%%%%%%%%%%%%%
pop = zeros(popSize,n); % Matrix pop maintains the current population
pop(1,:) = (1:n);
for k = 2:popSize
    pop(k,:) = randperm(n);
end
%%%%%%%%%%%%%%%%% End of Population Initialization %%%%%%%%%%%%%%%%%%%%%%%%



totalDist = zeros(1,popSize);
% totalDist is the vector of distances.Each element of this vector corresponds
% to the total distance (i.e. length) of a member of the population.


%Storing average and best fitnesses.
avgs = zeros(numGen, 1);
bests = zeros(numGen, 1);

%% Starting GA iterations. In each iteration, a new generation is created %%%%%%
for iter = 1:numGen

    %iter = iter
    % Function calcToursDistances evaluates Each population member and
    % calculates the total distance of each member
    totalDist = calcToursDistances(pop, popSize, dmat, n);



    % Elite selection: you should use the information in matrix totalDist
    % to select a fraction eliteFract of the best members of the current
    % population pop. Keep these elite members in matrix elitePop.
    % Your elite selection code goes here:
    [s, inds] = sort(totalDist);
    tmp_pop = pop(inds,:);

    elitePop = tmp_pop(1:round(eliteFract*popSize),:);

    % ...
    % ...
    %%%%%%% end of elite selection %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



    % Selection of new population: use totalDist to clacluate the fitness
    % and the cummulative probability distribution of the current population
    % pop. Then apply a roulette wheel selection to create a new population.
    % Keep this new population in matrix newPop.
    % Your roulette wheel selection code goes here:

    q_i = get_cdf(totalDist, popSize);

    newPop = zeros(popSize,n);
    for j = 1:popSize
        newPop(j,:) = pop(get_chromo_index(q_i, rand),:);
    end


    %%%%%%% end of roulette wheel selection %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



    % Update distance vector totalDist according to the selected population
    % newPop. Your code for updating totalDist goes here:

    totalDist = calcToursDistances(newPop, popSize, dmat, n);

    %%%%%% end of totalDist update %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    % Use the updated totalDist to calculate the new fitness values and
    % cummulative probability distribution. Your code goes here:

    q_i = get_cdf(totalDist, popSize);

    %%%%%% end of fitness and probabilty distribution update %%%%%%%%%%%%%%

    % Cross-over and mutation operator: implement the cross-over procedure
    % described in the home assignment. Use the cross-over
    % probability crossProb to obtain the parents that should
    % be selected purely random from the population newPop.
    % Your code goes here:

    % Selecting chromosomes that should reproduce
    parents = [];
    for j = 1:popSize
        if rand < crossProb;
            parents = [parents j];
        end
    end

    [r, c] = size(parents);
    if mod(c, 2) ~ 0;
        parents = parents(1:end-1);
    end

    [r, c] = size(parents);

    newOffSpring = [];
    for j = 1:c/2
        offspring = Crossover(newPop(parents(j),:), newPop(parents(j+1),:));

        % Now mutation!?
        r = rand();
        if r <= mutProb;
            routeInsertionPoints = sort(ceil(n*rand(1,2)));
            I = routeInsertionPoints(1);
            J = routeInsertionPoints(2);

            % 2-opt mutation (simply swaps two cities)
            offspring([I J]) = offspring([J I]);

        end

        % now, you should replace one of the parents invloved in
        % cross-over with this mutated offspring, then update the
        % population newPop.

        if rand() < 0.5;
          newPop(j,:) = offspring;
        else
          newPop(j+1,:) = offspring;
        end

    end


    %%%%%%% End of cross-over and mutation operator %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    % Now, it is time to replace the worst members of newPop with the elite
    % members you stored in matrix elitePop (see Elite selection in the begining
    % of this iteration).
    % Your code goes here:

    totalDist = calcToursDistances(newPop, popSize, dmat, n);

    [s, inds] = sort(totalDist, 'descend');
    newPop(inds(1:round(eliteFract*popSize)),:) = elitePop;

    %%%%%%% End of elite replacement %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    % Finally, the new population newPop should become the current population.
    pop = newPop;    % Uncomment this line when you finished all previous
                       % steps.

    % Storing the best dist and the average dist in their respective containers
    avgs(iter) = mean(totalDist);
    bests(iter) = min(totalDist);

end
%%%%%% End of GA ietartions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% Now, we find the best route in the last generation (you don't need to
% change this part). The best route is returned in optRoute, and its
% distance in minDist.
totalDist = calcToursDistances(pop, popSize, dmat, n);
[minDist,index] = min(totalDist);
optRoute = pop(index,:);

% Return Output (you don't need to change this part)
if nargout
    resultStruct = struct( ...
        'optRoute',    optRoute, ...
        'avgs',    avgs, ...
        'bests',    bests, ...
        'minDist',     minDist);

    varargout = {resultStruct};
end

end


% The following code is for configuation of user input (do not change
% this). Subfunction to override the default configuration with user inputs
function config = get_config(defaultConfig,userConfig)

% Initialize the configuration structure as the default
config = defaultConfig;

% Extract the field names of the default configuration structure
defaultFields = fieldnames(defaultConfig);

% Extract the field names of the user configuration structure
userFields = fieldnames(userConfig);
nUserFields = length(userFields);

% Override any default configuration fields with user values
for i = 1:nUserFields
    userField = userFields{i};
    isField = strcmpi(defaultFields,userField);
    if nnz(isField) == 1
        thisField = defaultFields{isField};
        config.(thisField) = userConfig.(userField);
    end
end

end

% User added help functions below:
function index = get_chromo_index(q_i, rnd)
    j = 1;
    while(rnd > q_i(j))
        j = j+1;
    end
    index = j;
end

function q_i = get_cdf(totalDist, popSize)
    fit_pop = 1./totalDist;
    TotFit = sum(fit_pop);
    prob_i = fit_pop/TotFit;
    q_i = zeros(popSize,1);
    for j = 1:popSize
        q_i(j) = sum(prob_i(1:j));
    end
end

function O = Crossover(P1, P2)
    [r, c] = size(P1);
    K = floor(0.3*c);
    length(P1);
    T1 = P1;
    T2 = P2;
    O = [];

    j = 0;
    while T1 ~ [];
        j = j +1;
        lT11 = min([K length(T1)]);
        T11 = T1(1:lT11);
        T12 = T1(lT11+1:end);
        O = [O T11];
        T1 = setdiff(T1,T11, 'stable');
        T2 = setdiff(T2,T11, 'stable');
        X = T1;
        T1 = T2;
        T2 = X;
    end
end
