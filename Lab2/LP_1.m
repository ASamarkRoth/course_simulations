clc
clear all

c = [-100;
     -150];
A = [8000 4000;
     15   30 ];
 b = [40000;
      200];
 lb = [0;
       0];
 %options = optimoptions('linprog', 'Algorithm', 'interior-point', 'Display', 'iter');
 %options = optimoptions('linprog', 'Algorithm', 'interior-point', 'Display', 'off');
 %options = optimoptions('linprog', 'Algorithm', 'dual-simplex', 'Display', 'iter');
 options = optimoptions('linprog', 'Algorithm', 'dual-simplex', 'Display', 'off');
 [x,fval,exitflag,output,lambda] = linprog(c', A, b, [], [], lb, [], [], options)
