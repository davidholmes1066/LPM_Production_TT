function [frequencyD0, frequencyD1] = GET_HZ(LPM,xmega)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
write(LPM,"holdingregs",12193+1,9999,"int16");                          %Set D0 to testmode
write(LPM,"holdingregs",12205+1,1000,"int16");                          %Set D0 to 1000 Hz
write(LPM,"holdingregs",12209+1,0,"int16");                             %Turn error mode off

write(LPM,"holdingregs",12193+2,0,"int16");                             %Set D1 to off
write(LPM,"holdingregs",12205+2,0,"int16");                             %Set D1 to 0 Hz
write(LPM,"holdingregs",12209+2,0,"int16");                             %Turn error mode off

pause(3);

write(xmega,6,"uint8");                                                 %Measure frequency
fsamples = read(xmega,1,"single");                                      %Read frequency in clck clycles
frequencyD0 = fsamples.*2.6782751422;                                   %Calculate frequency NEEDS CALLIBRATING

write(LPM,"holdingregs",12193+1,0,"int16");                             %Set D0 to off
write(LPM,"holdingregs",12205+1,0,"int16");                             %Set D0 to 0 Hz

write(LPM,"holdingregs",12193+2,9999,"int16");                          %Set D1 to Testmode
write(LPM,"holdingregs",12205+2,1000,"int16");                          %Set D1 to 1000 Hz

pause(1);

write(xmega,6,"uint8");                                                 %Measure frequency
fsamplesd = read(xmega,1,"single");                                     %Read frequency in clck clycles
frequencyD1 = fsamplesd.*2.6782751422;                                  %Calculate frequency NEEDS CALLIBRATING

end