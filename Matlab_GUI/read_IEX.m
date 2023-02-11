function [IEX_AVR] = read_IEX(XMEGA)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
SUM = 0;
write(XMEGA,'222',"char");
IEXC{1} = read(XMEGA,5,"char");
%disp(IEXC{1});
write(XMEGA,'222',"char");
IEXC{2} = read(XMEGA,5,"char");
%disp(IEXC{2});
write(XMEGA,'222',"char");
IEXC{3} = read(XMEGA,5,"char");
%disp(IEXC{3});
write(XMEGA,'222',"char");
IEXC{4} = read(XMEGA,5,"char");
%disp(IEXC{4});
write(XMEGA,'222',"char");
IEXC{5} = read(XMEGA,5,"char");
%disp(IEXC{5});
write(XMEGA,'222',"char");
IEXC{6} = read(XMEGA,5,"char");
%disp(IEXC{6});

for i=2:6
    IEX(i-1) = str2double(IEXC(i));
end

for i=1:5
    SUM = SUM + IEX(i);
end
    IEX_AVR = SUM./5;
    IEX_AVR = IEX_AVR*0.0002516;
    IEX_AVR = IEX_AVR + (IEX_AVR * 0.296)-0.25;
    IEX_AVR = ((IEX_AVR/165)*1000000);
    %insert calculation to get current
    SUM = 0;
end