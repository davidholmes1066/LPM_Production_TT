function [Volt] = VConv(Vraw)
Volt = (((Vraw./4096)*3.3)-0.24955)*2.79;
end