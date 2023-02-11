function [LPU_object] = SET_BUS(COM,BaudRate)
%Setsup MODBUS RTU object for the LPU
LPU_object = modbus('serialrtu',COM,'BaudRate',BaudRate,'Timeout',2,'StopBits',2); %create modbus object
end