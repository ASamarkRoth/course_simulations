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
 %options = optimoptions('intlinprog', 'Display', 'iter');
 options = optimoptions('intlinprog', 'Display', 'off');
 intcon = [1;
           2];
[x, fval, exitflag, output] = intlinprog(c', intcon, A, b, [], [], lb, [], options)
