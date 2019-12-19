n=100;
dt=1/100;
t0=0:dt:1-dt;
t1=1:dt:2-dt;
t2=2:dt:3-dt;
t3=3:dt:4-dt;
t4=4:dt:5-dt;
t5=5:dt:6-dt;

x0=ones(1,n);
x1=zeros(1,n);
x2=x0;
x3=x1;
x4=x0;
x5=x1;
x=[x0 x1 x2 x3 x4 x5];

tau=0.5;
v0=(1-exp(-1/tau))/(1-exp(-2/tau))
v1=(exp(-1/tau)-exp(-2/tau))/(1-exp(-2/tau))
v0+v1
y0=1+(v1-1)*exp(-(t1-1)/tau);
y1=v0*exp(-t0/tau);
y2=y0;
y3=y1;
y4=y0;
y5=y1;
y=[y0 y1 y2 y3 y4 y5];
subplot(3,1,1)
plot(0:dt:6-dt,x,0:dt:6-dt,y)
ylim([-0.5 1.5])

y0=1-cos(2*pi*t0);
y1=zeros(1,n);
y2=y0;
y3=y1;
y4=y0;
y5=y1;
y=[y0 y1 y2 y3 y4 y5];
subplot(3,1,2)
plot(0:dt:6-dt,y,0:dt:6-dt,x)
ylim([-0.5 2.5])

y0=1-cos(pi*t0);
y1=-2*cos(pi*t1);
y2=1-3*cos(pi*t2);
y3=-4*cos(pi*t3);
y4=1-5*cos(pi*t4);
y5=-6*cos(pi*t5);
y=[y0 y1 y2 y3 y4 y5];
subplot(3,1,3)
plot(0:dt:6-dt,y,0:dt:6-dt,x)
ylim([-6 6])