function [PathL] = GET_PL(device)
%Reads the path lengths from the LPU
PathL = read(device,'holdingregs',11151+1,4,1,'single'); %SNR AB
end