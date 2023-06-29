clearvars;
close all;

LPMCOM = 'COM5';                                                                   %LPM Com select                                                             
xmega = serialport('COM10',115200,'Timeout',10);                                    %Connect to xmega
write(xmega,1,"uint8");                                                             %Enable external supply
pause(10);                                                                          %Wait for LPM to start

LPM = SET_BUS(LPMCOM, 115200);                                                      %Connect to LPM

BatchNr = '911';                                                                    %Set batch nr
SNmr= '911';                                                                        %Set Serial nr

Filename = sprintf('%s_%s.xlsx', BatchNr, datestr(now,'dd-mm-yyyy'));               %Set Filename

filecheck = exist("testlog.xlsx", "file");                                          %Checks existance of template
findnew = exist(sprintf("./logfiles/%s",Filename), "file");                         %Checks existance of new logfile

if findnew == 0
    status = copyfile("testlog.xlsx", sprintf("./logfiles/%s",Filename),"f");       %Copy's file to logfile folder
end

write(xmega, 4, "uint8");                                                           %Measure external current
Ecurrent = read(xmega, 1, "single");                                                %Read external current measurement

write(xmega, 0, "uint8");                                                           %Supply battery
pause(1);                                                                           %wait for supply to stabalize
write(xmega, 3, "uint8");                                                           %Measure battery current
read(xmega, 1, "single");                                                           %Read battery current 

write(xmega, 5, "uint8");                                                           %Measure LPM voltage lines
v1 = read(xmega, 1, "single");
v2 = read(xmega, 1, "single");
v3 = read(xmega, 1, "single");
v4 = read(xmega, 1, "single");
v5 = read(xmega, 1, "single");
v6 = read(xmega, 1, "single");
v7 = read(xmega, 1, "single");

[frequencyD0, frequencyD1] = GET_HZ(LPM, xmega);                                   %Read frequency D0 and D1 plus plot

SNR = GET_SNR(LPM, 5);                                                             %Reads SNR and calculates avrage for all 4 paths
GAIN = GET_GAIN(LPM, 5);                                                           %Reads GAIN and calculates avrage for all 4 paths
VOS = GET_VOS(LPM, 5);                                                             %Reads VOS and calculates avrage for all 4 paths
PT = GET_PT(LPM);                                                                  %Reads temp and pressure

write(xmega, 2, "uint8");                                                          %Turn LPM power off