function [PT] = GET_PT(device)
%read P and T
PT = zeros(1,2);

P = read(device,"holdingregs",1309+1,1,1,"single");
T = read(device,"holdingregs",1311+1,1,1,"single");

PT(1) = P;
PT(2) = T;
end