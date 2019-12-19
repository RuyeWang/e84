
w=0:1:10000;

L=10^(-2);
C=10^(-4);
R=100;
wn=1/sqrt(L*C)
Zl=j*w*L;
Zc=1./(j*w*C);
Zcl=(Zc.*Zl)./(Zc+Zl);

d=1./(Zcl+R);
H1=Zcl.*d;
H2=R.*d;

d=sqrt(1/(R*C)^2+4/(L*C));
w1=(d-1/R/C)/2
w2=(d+1/R/C)/2
w2-w1
dw=1/R/C


Lm1=20*log(abs(H1));
Lm2=20*log(abs(H2));

subplot(2,1,1)
plot(w,abs(H1),w,abs(H2))
xlim([0 2000])
legend('Bandpass','Bandstop')
subplot(2,1,2)
semilogx(w,Lm1,w,Lm2);
legend('Bandpass','Bandstop')
return

% 
% R=10^3;
% C=10^(-6);
% tau=R*C
% wc=1/tau
% Zc=1./(j*w*C);
% Hr=R./(R+Zc);
% Hc=Zc./(R+Zc);
% 
% figure(1)
% subplot(4,2,1)
% plot(w,abs(Hr),w,abs(Hc))
% xlim([0 10000])
% ylim([0 1.1])
% legend('abs(Hr)','abs(Hc)')
% 
% subplot(4,2,2)
% plot(w,angle(Hr),w,angle(Hc))
% xlim([0 10000])
% legend('angle(Hr)','angle(Hc)')
% subplot(4,2,3)
% semilogx(w,20*log(abs(Hr)),w,20*log(abs(Hc)))
% legend('Lm(Hr)','Lm(Hc)')
% subplot(4,2,4)
% semilogx(w,angle(Hr),w,angle(Hc))
% legend('angle(Hr)','angle(Hc)')

% wmax=10^7;
% w=0:1:wmax;
% R=10^2;
% C=10^(-6)
% L=10^(-3);
% wn=1/sqrt(L*C)
% zeta=R*sqrt(C/L)/2
% dw=2*zeta*wn
% Zc=1./(j*w*C);
% Zl=j*w*L;
% Z=Zc+Zl+R;
% Hr=R./Z;
% Hc=Zc./Z;
% Hl=Zl./Z;
% subplot(4,2,5)
% plot(w,abs(Hr),'r',w,abs(Hc),'g',w,abs(Hl),'b')
% xlim([0 100000])
% legend('abs(Hr)','abs(Hc)','abs(Hl)')
% subplot(4,2,6)
% plot(w,angle(Hr),'r',w,angle(Hc),'g',w,angle(Hl),'b')
% xlim([0 100000])
% legend('angle(Hr)','angle(Hc)','angle(Hl)')
% subplot(4,2,7)
% semilogx(w,20*log(abs(Hr)),'r',w,20*log(abs(Hc)),'g',w,20*log(abs(Hl)),'b')
% xlim([0 wmax])
% legend('Lm(Hr)','Lm(Hc)','Lm(Hl)')
% subplot(4,2,8)
% semilogx(w,angle(Hr),'r',w,angle(Hc),'y',w,angle(Hl),'b')
% xlim([0 wmax])
% legend('angle(Hr)','angle(Hc)','angle(Hl)')
% 
% return

L=0.001;   Rl=7;
L=0.00047; Rl=5;
L=0.0022;  Rl=24;
L=0.0047;  Rl=27;
C=10^(-8);
C=10^(-6);
C=10^(-5);
C=10^(-7);
R=10;
%R=300;

wn=1/sqrt(L*C)
fn=wn/2/pi

Q=wn*L/(R+Rl)
Q=sqrt(L/C)/R
zeta=1/2/Q
zeta=R*sqrt(C/L)/2
dw=wn/Q
dw=(R+Rl)/L
df=fn/Q

pause

return

fmax=10^6;
f=0:1:fmax;
w=2*pi*f;
Zc=1./(j*w*C);
Zl=j*w*L;
Z=Zc+Zl+R+Rl;
Hr=R./Z;
Hc=Zc./Z;
Hl=Zl./Z;

Tr=20*log(abs(Hr));
Tc=20*log(abs(Hc));
Tl=20*log(abs(Hl));

figure(1)
subplot(3,2,1)
plot(f,abs(Hr),f,abs(Hl),f,abs(Hc))
xlim([0 50000])
subplot(3,2,2)
semilogx(f,Tr,f,Tl,f,Tc)
xlim([0 100000])

syms w real;
assume(w > 0)

% R=100;
% L=0.001;
% C=10^(-6);

Zl=j*w*L+Rl;
Zc=1/(j*w*C);
Zlc=Zl*Zc/(Zl+Zc);
aZ=abs(Zlc);
aZ=rewrite(aZ,'sqrt')

dZ=diff(aZ==0,w)
w1=solve(dZ,w)
w0=1./sqrt(L*C)

w=f*2*pi;

Zc=1./(j*w*C);
Zl=j*w*L;

Zlc=Zl.*Zc./(Zl+Zc);

H1=R./(R+Zlc);
H2=Zlc./(R+Zlc);

Zlr=Rl+j*w*L;
Zlcr=Zlr.*Zc./(Zlr+Zc);

T1=R./(R+Zlcr);
T2=Zlcr./(R+Zlcr);

subplot(3,2,3)
plot(f,abs(H1),f,abs(T1))
xlim([0 50000])
title('band-stop (linear)')

subplot(3,2,4)
LmH1=20*log10(abs(H1));
LmT1=20*log10(abs(T1));
semilogx(f,LmH1,f,LmT1)
title('band-stop (Bode)')

subplot(3,2,5)
plot(f,abs(H2),f,abs(T2))
xlim([0 50000])
title('band-pass (linear)')

subplot(3,2,6)
LmH2=20*log10(abs(H2));
LmT2=20*log10(abs(T2));
semilogx(f,LmH2,f,LmT2)
title('band-pass (Bode)')



