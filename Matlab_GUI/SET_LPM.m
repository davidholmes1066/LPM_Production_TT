function [ ] = SET_LPM(device,pathl)

write(device,"holdingregs", 11151, pathl,1,'single');             %set pathl
write(device,"holdingregs", 12561-1, 11,1,'uint32');              %eneble all 4 channels


%PTsetup
write(device,"holdingregs", 12681-1, 1000,1,'uint16');%
write(device,"holdingregs", 11501, 985,1,'single');
write(device,"holdingregs", 11503, 0.004,1,'single');
write(device,"holdingregs", 11505, -1.0,1,'single');
write(device,"holdingregs", 12682-1, 1,1,'uint16');%
write(device,"holdingregs", 11507, 156.04,1,'single');
write(device,"holdingregs", 11509, 7.0,1,'single');
write(device,"holdingregs", 11511, 0.0039,1,'single'); 
write(device,"holdingregs", 11513, 0.9958,1,'single'); 
write(device,"holdingregs", 11515, 0.9 ,1,'single'); 
write(device,"holdingregs", 11517, -0.2,1,'single'); 
write(device,"holdingregs", 11519, 0.0,1,'single'); 
write(device,"holdingregs", 11521, 1.0,1,'single'); 
write(device,"holdingregs", 11523, 0.0,1,'single');
write(device,"holdingregs", 11525, 1.0,1,'single');
write(device,"holdingregs", 11527, -20.0,1,'single');
write(device,"holdingregs", 11529, 60.0,1,'single');
write(device,"holdingregs", 11531, 0.0,1,'single');
write(device,"holdingregs", 11533, 20.0,1,'single');
end