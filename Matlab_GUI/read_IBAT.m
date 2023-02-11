function [IBAT_AVR] = read_IBAT(XMEGA)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
SUM = 0;
write(XMEGA,'111','char');
IBATC{1} = read(XMEGA,5,"char");
%disp(IBATC{1});
write(XMEGA,'111',"char");
IBATC{2} = read(XMEGA,5,"char");
%disp(IBATC{2});
write(XMEGA,'111',"char");
IBATC{3} = read(XMEGA,5,"char");
%disp(IBATC{3});
write(XMEGA,'111',"char");
IBATC{4} = read(XMEGA,5,"char");
%disp(IBATC{4});
write(XMEGA,'111',"char");
IBATC{5} = read(XMEGA,5,"char");
%disp(IBATC{5});
write(XMEGA,'111',"char");
IBATC{6} = read(XMEGA,5,"char");
%disp(IBATC{6});

for i=2:6
    IBAT(i-1) = str2double(IBATC{i});
end

for i=1:5
    SUM = SUM + IBAT(i);
end
    IBAT_AVR = SUM./5;
    IBAT_AVR = IBAT_AVR*0.0002516;
    IBAT_AVR = IBAT_AVR + (IBAT_AVR * 0.296)-0.25;
    IBAT_AVR = ((IBAT_AVR/500)*1000000);
    %insert calculation to get current
    SUM = 0;
end