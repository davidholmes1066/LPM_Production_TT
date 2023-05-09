clearvars;
close all;
xmega = serialport('COM10',115200,'Timeout',10);

write(xmega, 10, "uint8");
test = read(xmega, 1, "uint16");