function [GAIN_AVR] = GET_GAIN(device,cycles)
%gets the avrage GAIN

%allocate for speed
Gain1 = zeros(1,cycles);
Gain2 = zeros(1,cycles);
Gain3 = zeros(1,cycles);
Gain4 = zeros(1,cycles);

%biderectional gain read
for i = 1:cycles
GAINAB = read(device,'holdingregs',2010+1,4,1,'uint16'); %GAIN AB
GAINBA = read(device,'holdingregs',2014+1,4,1,'uint16'); %SNR BA

Gain1(i) = (GAINAB(1) + GAINBA(1))/2;
Gain2(i) = (GAINAB(2) + GAINBA(2))/2;
Gain3(i) = (GAINAB(3) + GAINBA(3))/2;
Gain4(i) = (GAINAB(4) + GAINBA(4))/2;
end


%calculate avrage gain
GAIN_AVR1 = 0;
GAIN_AVR2 = 0;
GAIN_AVR3 = 0;
GAIN_AVR4 = 0;

for k = 1:cycles
    GAIN_AVR1 = GAIN_AVR1 + Gain1(k);
    GAIN_AVR2 = GAIN_AVR2 + Gain2(k);
    GAIN_AVR3 = GAIN_AVR3 + Gain3(k);
    GAIN_AVR4 = GAIN_AVR4 + Gain4(k);
end

GAIN_AVR1 = GAIN_AVR1/cycles;
GAIN_AVR2 = GAIN_AVR2/cycles;
GAIN_AVR3 = GAIN_AVR3/cycles;
GAIN_AVR4 = GAIN_AVR4/cycles;

%return 1 array
GAIN_AVR(1) = GAIN_AVR1;
GAIN_AVR(2) = GAIN_AVR2;
GAIN_AVR(3) = GAIN_AVR3;
GAIN_AVR(4) = GAIN_AVR4;
end