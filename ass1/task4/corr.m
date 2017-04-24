function corr(x, maxorder)
%corr Analyse data using autoregressive approach.
%   Returns mean and variance of data considering dependencies
% Usage: corr(x, maxorder)
%   x is the data
%   maxorder is the maximal autoregressive order

noargs = nargin;

if noargs == 0
	error('Not enough input arguments.');
elseif noargs == 1
	maxorder = 25;
elseif noargs > 2
  error('Incorrect number of inputs.');
end
maxorder = fix(maxorder);

nrofsamples = max(size(x));
g = [1.0, 2.771808, 1.894306, -0.486382, -0.272398, 0.194832];
d = [2.24, 1.96, 1.64];
alpha = [ 0.0125, 0.025, 0.05];
b = zeros(maxorder + 1, maxorder + 1);
s = zeros(maxorder + 1, 1);
w = zeros(maxorder + 1, 1);
v = zeros(maxorder + 1, 1);
r = zeros(maxorder + 1, 1);

xbar = sum(x);
x2bar = sum(x.^2) - xbar.^2 / nrofsamples;
x2bar = x2bar / (nrofsamples - 1);
xbar = xbar / nrofsamples;

tmpX = x - xbar;

for order = 0:maxorder,
	r(order+1) = sum(tmpX(1:nrofsamples-order).*tmpX(order+1:nrofsamples));
end
r(1:maxorder+1) = r(1:maxorder+1) ./ (nrofsamples - [0:maxorder]');

b(1,1) = 1.0;
s(1) = r(1);

for order = 0:maxorder - 1,
	nextorder = order + 1;
	tmpB = b(order+1,1:order+1);
	v(order + 1) = sum(tmpB.*(r(1:order+1)'));
	w(order + 1) = sum(tmpB.*fliplr(r(nextorder-order+1:nextorder+1)'));

	b(nextorder+1,nextorder+1) = -w(order+1)/v(order+1);
	b(nextorder+1,1) = 1.0;
	
	if order > 0
	coeff = 1:order;
	b(nextorder+1,coeff+1) = b(order+1,coeff+1)+(b(nextorder+1,nextorder+1)*b(order+1,nextorder+1-coeff));
	end
	s(nextorder+1) = sum(b(nextorder+1,1:nextorder+1).*r(1:nextorder+1)');
end

% test hypothesis
autoreg = 0;
for order = 0:maxorder-1,
	degoffreedom = maxorder - order;
% Calculate CHI2
	index = 1:6;
	chi2 = sum((degoffreedom.^((3-index)./2)).*g(index));
	stat = nrofsamples * (1.0 - (s(maxorder+1)/s(order+1)));
	if stat < chi2
		autoreg = 1;
		break
	end
end

icpl = xbar - (1.96 * sqrt(x2bar./nrofsamples));
icpu = xbar + (1.96 * sqrt(x2bar./nrofsamples));
disp(sprintf('Sample mean                = %f', xbar));
disp(sprintf('Sample population variance = %f', r(1)));
disp(sprintf('Final sample size          = %d', nrofsamples));

if autoreg == 0
	disp('No auto regressive order found')
else
	bsum = sum(b(order+1,1:order+1));
	vxbar = s(order+1)/(nrofsamples*bsum*bsum);
	svx = sqrt(vxbar);

  index = 0:order;
	lz = sum((order-2*index).*b(order+1,index+1));

	df = round(nrofsamples / (1.0 + (2.0*lz)/bsum) - 1.0);
	if df < 1
		disp('Computed equivalent degrees of freedom < 1,');
		disp('set = 1 for computation of confidence level');
		df = 1;
	end
	disp(sprintf('Autoregressive order                = %d', order));
	disp(sprintf('Sample variance of sample mean      = %f', vxbar));
	disp(sprintf('Unbiased sample population variance = %f', r(1)+vxbar));
	disp(sprintf('Equivalent degrees of freedom       = %d\n', df));
	
	for level = 1:3,
		y = d(level);
		y3 = y.^3;
		y5 = y3 * y * y;
		y7 = y5 * y * y;
		y9 = y7 * y * y;
		h(1) = (y3+y)*0.25;
		h(2) = ((5.0*y5)+(16.0*y3)+(3.0*y))/96.0;
      	h(3) = ((((3.0 * y7) + (19.0 * y5)) + (17.0 * y3)) - (15.0 * y)) / 384.0;
        h(4) = (((((79.0 * y9) + (776.0 * y7)) + (1482.0 * y5)) - (1920.0 * y3)) - (945.0 * y)) / 92160.0;
   
      	qu = 0.0;
   		for index = 1:4,
   			term = h(index);
   			for index1 = 1:index,
   				term = term / df;			
   			end		
   			qu = qu + term;
   		end
   		
   		qu = qu + y;
   		cpl = xbar - (qu*svx);
   		cpu = xbar + (qu*svx);
		disp(sprintf('%0.4f Lower confidence point = %1.5f', alpha(level), cpl));
		disp(sprintf('%0.4f Upper confidence point = %1.5f', alpha(level), cpu));
 		disp(sprintf('    Computed critical t-value = %1.2f\n', qu));
   	end
   	
   	if order == 0
   		disp('No correlation found, autoregressive order 0')
   	else
   		disp('Sample autoregressive coefficients')
		for i = 1:order+1,
			disp(sprintf('B[%d] = %f',i - 1,b(order+1,i)));
		end
   	end
	disp(sprintf('Sample residual variance = %f', s(order+1)));
	r = r./r(1);
	r(1) = 1.0;
% a nice plot of r!
	plot([0:maxorder], r,'*')
	title('Correlation coefficients versus order');
	axis([0 maxorder -1.0 1.0]);
end




