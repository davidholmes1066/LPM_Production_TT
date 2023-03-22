clearvars;

xmega = serialport('COM5',115200,'Timeout',100);

write(xmega, 1, "uint8");
responce(1) = read(xmega,1,"uint8");

pause(1);

write(xmega, 2, "uint8");
responce(2) = read(xmega,1,"uint8");


    