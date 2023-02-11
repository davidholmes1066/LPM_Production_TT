clearvars;
close all;
COM = "COM17";                                                              %Select COM ATXMega256A3U
READ_VOLT = 1;                                                              %Command to read voltages
READ_I_BAT = 2;                                                             %Command to read Battery current
READ_I_EXT = 3;                                                             %Command to read External current
READ_D0 = 4;                                                                %Command to read frequency_D0
READ_D1 = 5;                                                                %Command to read frequency_D1


xmega = open_TTL(COM);                                                      %Open COMx


% write(xmega,10,"uint8");                                                    %Enable external supply + current
% Ecurrent = VConv((read(xmega,1,"uint16")./50)./3.3);                        %Reads external current NEEDS CALLIBRATING
% 
% pause(2);
% 
% write(xmega,20,"uint8");                                                    %Enable Battery supply + current
% Bcurrent = VConv((read(xmega,1,"uint16")./50)./10);                         %Reads battery current NEEDS CALLIBRATING

write(xmega,30,"uint8");
v1 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v2 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v3 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v4 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v5 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v6 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING
v7 = VConv(read(xmega,1,"uint16"));                                         %NEEDS CALLIBRATING


% pause(2);
% 
% write(xmega,50,"uint8");                                                    %Measure frequency
% samples = read(xmega, 5000, "uint16");
% fsamples = read(xmega,1,"single");                                          %Read frequency in clck clycles
% frequency = fsamples.*2.6782751422;                                         %Calculate frequency NEEDS CALLIBRATING
% 
% plot(samples);
