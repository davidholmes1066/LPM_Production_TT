function [GAIN] = GET_GAIN(device,cycles)
%gets the avrage GAIN
%biderectional gain read
for i = 1:cycles
GAINAB = read(device,'holdingregs',2010+1,4,1,'uint16'); %GAIN AB
GAINBA = read(device,'holdingregs',2014+1,4,1,'uint16'); %GAIN BA

AB1(i) = GAINAB(1);
BA1(i) = GAINBA(1);
AB2(i) = GAINAB(2);
BA2(i) = GAINBA(2);
AB3(i) = GAINAB(3);
BA3(i) = GAINBA(3);
AB4(i) = GAINAB(4);
BA4(i) = GAINBA(4);
end


AB1_AVR = 0;
BA1_AVR = 0;
AB2_AVR = 0;
BA2_AVR = 0;
AB3_AVR = 0;
BA3_AVR = 0;
AB4_AVR = 0;
BA4_AVR = 0;

for i = 1:cycles
    AB1_AVR = AB1_AVR + AB1(i);
    BA1_AVR = BA1_AVR + BA1(i);
    AB2_AVR = AB2_AVR + AB2(i);
    BA2_AVR = BA2_AVR + BA2(i);
    AB3_AVR = AB3_AVR + AB3(i);
    BA3_AVR = BA3_AVR + BA3(i);
    AB4_AVR = AB4_AVR + AB4(i);
    BA4_AVR = BA4_AVR + BA4(i);
end

    GAIN(1) = AB1_AVR/cycles;
    GAIN(2) = BA1_AVR/cycles;
    GAIN(3) = AB2_AVR/cycles;
    GAIN(4) = BA2_AVR/cycles;
    GAIN(5) = AB3_AVR/cycles;
    GAIN(6) = BA3_AVR/cycles;
    GAIN(7) = AB4_AVR/cycles;
    GAIN(8) = BA4_AVR/cycles;
end