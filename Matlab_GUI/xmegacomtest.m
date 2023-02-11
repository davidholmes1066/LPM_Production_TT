clearvars;

xmega = serialport('COM4',1200);
data = read(xmega, 2, "uint8");


