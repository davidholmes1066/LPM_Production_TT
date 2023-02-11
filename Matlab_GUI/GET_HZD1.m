function [out] = GET_HZD1(LPM,Xmega)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
write(LPM,"holdingregs",12193+2,9999,"int16");                           %set D0 to testmode
write(LPM,"holdingregs",12205+2,100,"int16");                            %set D0 to 100 Hz

write(Xmega,'444',"char");                                               %send command to read frequency
frequency = read(Xmega,5,"char");                                        %read frequency
low = str2double(frequency);                                             %convert to double
lowf = 1/((1/125000)*low);                                               %convert to F
errl = (lowf - 100);
if errl<0
    errl = errl*(-1);
end

write(LPM,"holdingregs",12205+2,1000,"int16");                           %set D0 to 1000 Hz

write(Xmega,'444',"char");                                               %send command to read frequency
frequency = read(Xmega,5,"char");                                        %read frequency
med = str2double(frequency);                                             %convert to double
medf = 1/((1/125000)*med);                                               %convert to F
errm = (medf - 1000);
if errm<0
    errm = errm*(-1);
end

write(LPM,"holdingregs",12205+2,2500,"int16");                            %set D0 to 2500 Hz

write(Xmega,'444',"char");                                              %send command to read frequency
frequency = read(Xmega,5,"char");                                         %read frequency
high = str2double(frequency);                                             %convert to double
highf = 1/((1/125000)*high);                                                %convert to F
errh = (highf - 2500);
if errh<0
    errh = errh*(-1);
end

if errl>=errm && errl>=errh
    out = errl;
elseif errm >= errl && errm >= errh
    out = errm;
elseif errh >= errl && errh >= errm
    out = errh;
end

end