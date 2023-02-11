function [VOLT] = read_volt(XMEGAD)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

write(XMEGAD,'555','char');
read(XMEGAD,5,'char');
read(XMEGAD,5,'char');
read(XMEGAD,5,'char');
read(XMEGAD,5,'char');
read(XMEGAD,5,'char');
read(XMEGAD,5,'char');
read(XMEGAD,5,'char');

write(XMEGAD,'555','char');
a1 = read(XMEGAD,5,'char');
a2 = read(XMEGAD,5,'char');
a3 = read(XMEGAD,5,'char');
a4 = read(XMEGAD,5,'char');
a5 = read(XMEGAD,5,'char');
a6 = read(XMEGAD,5,'char');
a7 = read(XMEGAD,5,'char');

write(XMEGAD,'555','char');
b1 = read(XMEGAD,5,'char');
b2 = read(XMEGAD,5,'char');
b3 = read(XMEGAD,5,'char');
b4 = read(XMEGAD,5,'char');
b5 = read(XMEGAD,5,'char');
b6 = read(XMEGAD,5,'char');
b7 = read(XMEGAD,5,'char');

c1 = (str2double(a1)+str2double(b1)./2);
c2 = (str2double(a2)+str2double(b2)./2);
c3 = (str2double(a3)+str2double(b3)./2);
c4 = (str2double(a4)+str2double(b4)./2);
c5 = (str2double(a5)+str2double(b5)./2);
c6 = (str2double(a6)+str2double(b6)./2);
c7 = (str2double(a7)+str2double(b7)./2);

VOLT = [c1, c2, c3, c4, c5, c6, c7];
VOLT = ((VOLT*0.0005032*2)-0.25);
VOLT = VOLT +(0.296*VOLT);

end