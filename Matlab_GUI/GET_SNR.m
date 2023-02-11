function [SNR_AVR] = GET_SNR(device,cycles)
%Reads SNR from LPU

%preallocating SNRn arrays for speed
SNR1 = zeros(1,cycles);
SNR2 = zeros(1,cycles);
SNR3 = zeros(1,cycles);
SNR4 = zeros(1,cycles);

%capture SNR from all three paths
for k = 1:cycles
    SNRAB = read(device,'holdingregs',1095+1,4,1,'single'); %SNR AB
    SNRBA = read(device,'holdingregs',1103+1,4,1,'single'); %SNR BA

    %calculate biderectional SNR
    SNR1(k) = (SNRAB(1) + SNRBA(1))/2;
    SNR2(k) = (SNRAB(2) + SNRBA(2))/2;
    SNR3(k) = (SNRAB(3) + SNRBA(3))/2;
    SNR4(k) = (SNRAB(4) + SNRBA(4))/2;
end


%calculate avrage snr per path
SNR_AVR1 = 0;
SNR_AVR2 = 0;
SNR_AVR3 = 0;
SNR_AVR4 = 0;

for l = 1:cycles
    SNR_AVR1 = SNR_AVR1 + SNR1(l);
    SNR_AVR2 = SNR_AVR2 + SNR2(l);
    SNR_AVR3 = SNR_AVR3 + SNR3(l);
    SNR_AVR4 = SNR_AVR4 + SNR4(l);
end

SNR_AVR1 = SNR_AVR1/cycles;
SNR_AVR2 = SNR_AVR2/cycles;
SNR_AVR3 = SNR_AVR3/cycles;
SNR_AVR4 = SNR_AVR4/cycles;

%return 1 array
SNR_AVR(1) = SNR_AVR1;
SNR_AVR(2) = SNR_AVR2;
SNR_AVR(3) = SNR_AVR3;
SNR_AVR(4) = SNR_AVR4;
end