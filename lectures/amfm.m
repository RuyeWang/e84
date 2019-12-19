function amfm

close all
n=2048;
T=1
t0=T/n
F=1/t0
f0=1/T

t=0:t0:T-t0;

fc=60;
c=cos(2*pi*fc*t);
size(t)
figure(1)
subaxis(5,2,1);
plot(t,c)
title('Carrier')

f1=4;
f2=5.3;
f3=8.7;
f4=9.9;
a1=5;
a2=2;     a2=0;
a3=1;     a3=0;
a4=0.7;   a4=0;
s=a1*cos(2*pi*f1*t)+a2*cos(2*pi*f3*t)+a3*cos(2*pi*f3*t)+a4*cos(2*pi*f4*t);
s=s/max(s);
subaxis(5,2,3)
plot(t,s)
title('Signal')

x1=c.*(1+s);
subaxis(5,2,5)
plot(t,x1);
title('Amplitude Modulation (AM)')

kp=9;
x2=cos(2*pi*fc*t+kp*s);
subaxis(5,2,7)
plot(t,x2)
title('Phase Modulation (PM)')

kf=4;
s1=(a1*f1*sin(2*pi*f1*t)+a2*f2*sin(2*pi*f3*t)+a3*f3*sin(2*pi*f3*t)+a4*f4*sin(2*pi*f4*t))/(2*pi);
x3=cos(2*pi*fc*t+kf*s1);
subaxis(5,2,9)
plot(t,x3)
title('Frequency Modulation (FM)')

f=-F/2:f0:F/2-f0;
S=fftshift(fft(s));
C=fftshift(fft(c));
X1=fftshift(fft(x1));
X2=fftshift(fft(x2));
X3=fftshift(fft(x3));

frange=[-F/16 F/16];

subaxis(5,2,2)
plot(f,real(C),f,imag(C));
xlim(frange);
title('Spectrum of carrier')

subaxis(5,2,4)
plot(f,real(S),f,imag(S));
xlim(frange);
title('Spectrum of signal')

subaxis(5,2,6)
plot(f,real(X1),f,imag(X1));
xlim(frange);
title('Spectrum of AM')


subaxis(5,2,8)
plot(f,real(X2),f,imag(X2));
xlim(frange);
title('Spectrum of PM')

subaxis(5,2,10)
plot(f,real(X3),f,imag(X3));
xlim(frange);
title('Spectrum of FM')

figure(2)
subaxis(3,2,1)
plot(t,x1)
title('Received signal')

X1=fftshift(fft(x1));

subaxis(3,2,2)
plot(f,real(X1),f,imag(X1));
xlim(frange);
title('Spectrum of AM')

for i=1:n
    if x1(i)<0
        x1(i)=0;
    end
end
    
subaxis(3,2,3)
plot(t,x1,t,1+s,':r',t,0)  
ylim([-2 2]);
title('Rectified signal')

subaxis(3,2,5)
plot(t,1+s,t,0)
ylim([-2 2]);
title('Lowpass filtered signal')

end

    