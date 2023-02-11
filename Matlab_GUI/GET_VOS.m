function [VOS_AVR] = GET_VOS(device, cycles)
%Read the VOSRAW

%allocate for speed
VOS1 = zeros(1,cycles);
VOS2 = zeros(1,cycles);
VOS3 = zeros(1,cycles);
VOS4 = zeros(1,cycles);

%biderectional gain read
for i = 1:cycles
VOS = read(device,'holdingregs',1023+1,4,1,'uint16'); %GAIN AB

VOS1(i) = VOS(1);
VOS2(i) = VOS(2);
VOS3(i) = VOS(3);
VOS3(i) = VOS(4);
end


%calculate avrage gain
VOS_AVR1 = 0;
VOS_AVR2 = 0;
VOS_AVR3 = 0;
VOS_AVR4 = 0;

for k = 1:cycles
    VOS_AVR1 = VOS_AVR1 + VOS1(k);
    VOS_AVR2 = VOS_AVR2 + VOS2(k);
    VOS_AVR3 = VOS_AVR3 + VOS3(k);
    VOS_AVR4 = VOS_AVR4 + VOS4(k);
end

VOS_AVR1 = VOS_AVR1/cycles;
VOS_AVR2 = VOS_AVR2/cycles;
VOS_AVR3 = VOS_AVR3/cycles;
VOS_AVR4 = VOS_AVR4/cycles;

%return 1 array
VOS_AVR(1) = VOS_AVR1;
VOS_AVR(2) = VOS_AVR2;
VOS_AVR(3) = VOS_AVR3;
VOS_AVR(4) = VOS_AVR4;
end