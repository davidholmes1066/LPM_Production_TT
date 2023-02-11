function [Device] = open_TTL(COM)

Device = serialport(COM, 115200, 'Timeout',5);          %Opens COM PORT of test tool controll board

end