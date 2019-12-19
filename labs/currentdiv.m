function [out1,out2] = currentdiv(in,r1,r2)
w=in/(r1+r2);
out1=r2*w;
out2=r1*w;
