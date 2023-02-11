%MODBUS RTU RS485 To LPU
close all;
clearvars; %start fresh

%#defines
COM = "COM11"; %select comport
BaudRate = 115200; %select baudrate
pathl = [0.10 0.10 0.10 0.10]; %pathlengths in test setup
cycles = 5; %number for avrages

%checksum if COMx in use
oldserial = instrfind('Port', COM); %checks if COMx is in use

if (~isempty(oldserial)) %send warning if COMx is used
    disp('Warning: Port in use!')
    delete(oldserial);  
end

xmega = open_TTL('COM17');
write(xmega, 20, "uint8");

pause(7);

device = SET_BUS(COM,BaudRate);                                             %SET MODBUS object
SET_LPM(device,pathl);                                                      %sets pathlenghs to 0.1

%signal setup for the test setup;
PathL = GET_PL(device);                                                     %gets pathlenths
SNR_AVR = GET_SNR(device,cycles);                                           %gets avrage snr per path
GAIN_AVR = GET_GAIN(device,cycles);                                         %gets avrage gain per path
VOS_AVR = GET_VOS(device,cycles);                                           %gets avrage VOS per Path

write(xmega,20,"uint8");                                                    %Enable Battery supply + current
Bcurrent = VConv((read(xmega,1,"uint16")./50)./10);                         %Reads battery current NEEDS CALLIBRATING

write(xmega,30,"uint8");
v1 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v2 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v3 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v4 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v5 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v6 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v7 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING

pause(4);

[sampled0, fd0, sampled1, fd1] = GET_HZ(device, xmega);                     %frequency test

plot(sampled0);
hold on
plot(sampled1);
hold off

