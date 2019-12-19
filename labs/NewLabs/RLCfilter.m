w=0:1:100000;
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
R=280;

wn=1/sqrt(L*C)
fn=wn/2/pi

Q=wn*L/(R+Rl)
Q=sqrt(L/C)/(R+Rl)
zeta=1/2/Q
zeta=(Rl+R)*sqrt(C/L)/2
dw=wn/Q
dw=(R+Rl)/L
df=fn/Q


fmax=10^6;
f=0:1:fmax;
w=2*pi*f;
Zc=1./(j*w*C);
Zl=j*w*L;
Z=Zc+Zl+R+Rl;
Hr=R./Z;
Hr0=(R+Rl)./Z;
Hc=Zc./Z;
Hl=Zl./Z;

Tr=20*log(abs(Hr));
Tc=20*log(abs(Hc));
Tl=20*log(abs(Hl));
Tr0=20*log(abs(Hr0));


figure(1)
subaxis(1,3,1, 'Spacing', 0.02, 'Padding', 0.02, 'Margin', 0.02);
plot(f,abs(Hr),f,abs(Hl),f,abs(Hc),f,abs(Hr0),'--')
legend('Hr','Hl','Hc')
xlim([0 20000])
subplot(1,3,2)
subaxis(1,3,2, 'Spacing', 0.02, 'Padding', 0.02, 'Margin', 0.02);
semilogx(f,Tr,f,Tl,f,Tc,f,Tr0,'--')
legend('Hr','Hl','Hc')
xlim([0 100000])
subaxis(1,3,3, 'Spacing', 0.02, 'Padding', 0.02, 'Margin', 0.02);
semilogx(f,180*angle(Hr)/pi,f,180*angle(Hc)/pi,f,180*angle(Hl)/pi,f,180*angle(Hr0)/pi,'--')
legend('Hr','Hl','Hc')
xlim([0 100000])

figure(2)
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
Zl=j*w*L+Rl;

Zlc=Zl.*Zc./(Zl+Zc);

H1=R./(R+Zlc);
H2=Zlc./(R+Zlc);

Zlr=Rl+j*w*L;
Zlcr=Zlr.*Zc./(Zlr+Zc);

T1=R./(R+Zlcr);
T2=Zlcr./(R+Zlcr);

subaxis(2,3,1, 'Spacing', 0.02, 'Padding', 0.02, 'Margin', 0.02);
plot(f,abs(H1),f,abs(T1))
xlim([0 50000])
ylim([0 1])
title('band-stop (linear)')

subaxis(2,3,2, 'Spacing', 0.02, 'Padding', 0.02, 'Margin', 0.02);
LmH1=20*log10(abs(H1));
LmT1=20*log10(abs(T1));
semilogx(f,LmH1,f,LmT1)
title('Band-stop Lm')

subaxis(2,3,3, 'Spacing', 0.02, 'Padding', 0.02, 'Margin', 0.02);
semilogx(f,180*angle(H1)/pi,f,180*angle(T1)/pi)
title('Band-stop phase')


subaxis(2,3,4, 'Spacing', 0.02, 'Padding', 0.02, 'Margin', 0.02);
plot(f,abs(H2),f,abs(T2))
xlim([0 50000])
ylim([0 1])
title('band-pass (linear)')

subaxis(2,3,5, 'Spacing', 0.02, 'Padding', 0.02, 'Margin', 0.02);
LmH2=20*log10(abs(H2));
LmT2=20*log10(abs(T2));
semilogx(f,LmH2,f,LmT2)
title('band-pass Lm')

subaxis(2,3,6, 'Spacing', 0.02, 'Padding', 0.02, 'Margin', 0.02);
semilogx(f,180*angle(H2)/pi,f,180*angle(T2)/pi)
title('Band-stop phase')


