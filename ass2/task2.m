clc
clear all

 %############################################################################################
% 2.a
c = [-1;
     -5];
A = [2 -1;
     -1 1;
     1 4];
 b = [4;
      1;
      12];
 lb = [0;
       0];
 %options = optimoptions('intlinprog', 'Display', 'iter');
 options = optimoptions('intlinprog', 'Display', 'off');
 intcon = [1;
           2];
[x, fval, exitflag, output] = intlinprog(c', intcon, A, b, [], [], lb, [], options);

optimal_parameters = x
function_val = fval

 %############################################################################################
% 2.b
 options = optimoptions('linprog', 'Algorithm', 'dual-simplex', 'Display', 'off');
 [x,fval,exitflag,output,lambda] = linprog(c', A, b, [], [], lb, [], [], options);

lin_optimal_parameters = x
lin_function_val = fval

round_x = [1 2 1 2; 2 2 3 3];
for j = 1:4
  x = round_x(:,j)
  Ax = A*x
  b = b
  f = c'*x
end

 %############################################################################################
% 2.c - branch and bound

f = fopen('b2b.txt', 'w');

% Calculating initial upper and lower bound
 options = optimoptions('linprog', 'Algorithm', 'dual-simplex', 'Display', 'off');
 [x,fval,exitflag,output,lambda] = linprog(c', A, b, [], [], lb, [], [], options);

 x_orig = x
 f_ub = -fval
 x_lb = floor(x)
 f_lb = -c'*x_lb

 fprintf(f, 'Original problem \n');
 fprintf(f, '(x1, x2, fval) = (%d, %d, %d) \n', x(1), x(2), -fval);
 fprintf(f, '(lb, ub) = (%d, %d) \n', f_lb, f_ub);

% Branching on x1
 fprintf(f, '\n 1st branching \n');
 b_ind = 1;
 lb_a = [ceil(x(b_ind)); 0];

 [x,fval,exitflag,output,lambda] = linprog(c', A, b, [], [], lb_a, [], [], options);
 xa = x;
 are_ints = (round(x) - x)'*[1;1]
 if are_ints ~= 0
   if -fval < f_ub
     f_ub = -fval;
   end
   x_lb = floor(x)
   if -c'*x_lb > f_lb
     f_lb = -c'*x_lb
   end
 end

 fprintf(f, 'Branch a (x1 >= %d), and are both integers = %i \n', lb_a(b_ind), ~are_ints);
 fprintf(f, '(x1, x2, fval) = (%d, %d, %d) \n', x(1), x(2), -fval);
 fprintf(f, '(lb, ub) = (%d, %d) \n', f_lb, f_ub);

 ub_b = [floor(x_orig(b_ind)); 1e10];
 [x,fval,exitflag,output,lambda] = linprog(c', A, b, [], [], lb, ub_b, [], options);
 are_ints = (round(x) - x)'*[1;1]
 if are_ints ~= 0
   if -fval < f_ub
     f_ub = -fval;
   end
   x_lb = floor(x)
   if -c'*x_lb > f_lb
     f_lb = -c'*x_lb
   end
 end

 fprintf(f, 'Branch b (x1 <= %d), and are both integers = %i \n', ub_b(b_ind), ~are_ints);
 fprintf(f, '(x1, x2, fval) = (%d, %d, %d) \n', x(1), x(2), -fval);
 fprintf(f, '(lb, ub) = (%d, %d) \n', f_lb, f_ub);

 % 2nd branching from a on x2 as it is not an integer
 fprintf(f, '\n 2nd branching \n');
 'Second branching'
 b_ind = 2;
 lb_a = [0; ceil(xa(b_ind))]

 [x,fval,exitflag,output,lambda] = linprog(c', A, b, [], [], lb_a, [], [], options)
 if exitflag == 1
   if are_ints ~= 0
     if -fval < f_ub
       f_ub = -fval;
     end
     x_lb = floor(x)
     if -c'*x_lb > f_lb
       f_lb = -c'*x_lb
     end
   end

   fprintf(f, 'Branch a (x2 >= %d), and are both integers = %i \n', lb_a(b_ind), ~are_ints);
   fprintf(f, '(x1, x2, fval) = (%d, %d, %d) \n', x(1), x(2), -fval);
   fprintf(f, '(lb, ub) = (%d, %d) \n', f_lb, f_ub);
 else
   fprintf(f, 'Branch a (x2 >= %d), had no feasible solution \n', lb_a(b_ind));
 end

 ub_b = [1e10; floor(xa(b_ind))];
 [x,fval,exitflag,output,lambda] = linprog(c', A, b, [], [], lb, ub_b, [], options);
 are_ints = (round(x) - x)'*[1;1]
 if are_ints ~= 0
   if -fval < f_ub
     f_ub = -fval;
   end
   x_lb = floor(x)
   if -c'*x_lb > f_lb
     f_lb = -c'*x_lb
   end
 end

 fprintf(f, 'Branch b (x2 <= %d), and are both integers = %i \n', ub_b(b_ind), ~are_ints);
 fprintf(f, '(x1, x2, fval) = (%d, %d, %d) \n', x(1), x(2), -fval);
 fprintf(f, '(lb, ub) = (%d, %d) \n', f_lb, f_ub);


