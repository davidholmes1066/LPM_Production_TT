clearvars;

xmega = serialport('COM10',115200,'Timeout',1);
write(xmega, 30, "uint8");

for i= 1:4
   voltl(i) = read(xmega,1,"uint8");
   volth(i) = read(xmega,1,"uint8");
end


