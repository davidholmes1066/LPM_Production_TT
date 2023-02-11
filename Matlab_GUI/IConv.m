function [I] = IConv(Iraw)
I = ((((Iraw./4096)*3.3)-0.2498)./50./3.3)*75000;
end