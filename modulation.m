
function modulation

    kp=3;
    kf=100;
    
    fc=100;
    fs=10;
    wc=2*pi*fc;
    ws=2*pi*fs;
    t=0:0.0001:1;
    c=cos(wc*t);
    s=cos(ws*t)+2*cos(ws*t/2)+3*cos(ws*t/3);
    r=(sin(ws*t)+2^2*sin(ws*t/2)+3*3*sin(ws*t/3))/ws;
    subplot(5,1,1)
    plot(t,c)    
    title('Carrier')
    subplot(5,1,2)
    plot(t,s);
    title('Signal')
    subplot(5,1,3)
    plot(t,s.*c)
    title('Amplitude Modulation')
    subplot(5,1,4)
    plot(t,cos(wc*t+kp*s))
    title('Phase Modulation')
    subplot(5,1,5)
    plot(t,cos(wc*t+kf*r))
    title('Frequency Modulation')
    
end