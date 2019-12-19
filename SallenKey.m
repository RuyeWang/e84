function main
    close all
    R1=200; R2=R1; R3=90000; C=10^(-7);
    Rp=R1*R2/(R1+R2) 
    
    w=1:100:10^8;        
    
    H=-(R3/R1)./(2-j*(1./(w*C*Rp)-w*C*R3));
    semilogx(w,20*log(abs(H)))
    hold on
    semilogx(w,abs(H))
    hold off
   
    wn=1/(C*sqrt(Rp*R3))
    dw=2/(C*R3)
    Q=sqrt(R3/Rp)/2

end