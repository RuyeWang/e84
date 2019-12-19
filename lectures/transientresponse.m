f=99
v0=1;
vs=1;
t=0:0.01:10;
w=zeros(1,size(t,1));
tau=4;
for i=1:4
    subplot(4,1,i)
    phi=(i-1)*pi/2;
    x=vs*cos(2*pi*f*t+phi);
    y=(v0-vs*cos(phi))*exp(-t/tau);
    z=x+y;
    plot(t,x,'r--',t,y,'b--',t,z,'k',t,w)
    axis([0,10,-3,3])
    title(['Vs=',num2str(vs),', V0=',num2str(v0),', psi-phi=',num2str(phi*180/pi)]);
end
