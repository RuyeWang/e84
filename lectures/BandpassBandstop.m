f=0:10:10^5;
w=f*2*pi;
% R=1000;
% C=10^(-6);
% L=10^(-3);
% Zc=1./(j*w*C);
% Zl=j*w*L;
% wn=1./sqrt(L*C)
% d=Zl+Zc+R;
% semilogx(w,20*log10(abs(Zc./d)),w,20*log10(abs(Zl./d)),w,20*log10(abs(R./d)))


R=150;
Rl=10;
Rl=30;
Rl=25;
Rl=7;
C=10^(-5);
C=10^(-6);
L=10^(-2);
L=4.3*10^(-3);
L=2*10^(-3);
L=0.7*10^(-3);
wn=1./sqrt(L*C)
fn=wn/2/pi
Zc=1./(j*w*C);
Zl=j*w*L;
Zcl=(Zc.*Zl)./(Zc+Zl);
d=Zcl+R;
Hbp=Zcl./d;
Hbs=R./d;
subplot(4,1,1)
plot(f,abs(Hbp),f,abs(Hbs))
xlim([0 20000])
subplot(4,1,2)
semilogx(f,20*log(abs(Hbp)),f,20*log(abs(Hbs)))

r=sqrt(1/(R*C)^2+4/(L*C));
w1=(r-1/R/C)/2;
w2=(r+1/R/C)/2;
dw=w2-w1;
f1=w1/2/pi;
f2=w2/2/pi;
df=f2-f1;
fprintf('f1=%.0f, fn=%.0f, f2=%.0f, df=%.0f\n',f1,fn,f2,df)

Zl=Zl+Rl;
Zcl=(Zc.*Zl)./(Zc+Zl);
d=Zcl+R;
Hbp=Zcl./d;
Hbs=R./d;
subplot(4,1,3)
plot(f,abs(Hbp),f,abs(Hbs))
xlim([0 20000])
subplot(4,1,4)
semilogx(f,20*log(abs(Hbp)),f,20*log(abs(Hbs)))