function [xmega] = xmega(COM)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
BAUD = 115200;

xmega = serialport(COM,BAUD);
configureTerminator(xmega,"CR/LF");
xmega.Timeout = 2;

flush(xmega);
end