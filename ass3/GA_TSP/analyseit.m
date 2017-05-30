function [] = analyseit(mat)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
  avgs = mean(mat,2);
  sigmas = zeros(20,1);
  for j = 1:20;
      sigmas(j) = std(mat(j,:));
  end

  figure
  hold on
  yyaxis left
  plot(100:100:2000, avgs)
  plot(100:100:2000, avgs+1.96*sigmas/sqrt(15))
  plot(100:100:2000, avgs-1.96*sigmas/sqrt(15))
  ylabel('Fitness value')
  xlabel('Number of generations')
  yyaxis right
  plot(100:100:2000, sigmas./avgs)
  ylabel('Normalised standard deviation')
  legend('Average','Upper 95% conf. int.', 'Lower 95% conf. int.')

end

