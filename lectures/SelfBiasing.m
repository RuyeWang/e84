Vcc=12;
R1=100;
R2=36;
Re=1;
Rc=2;
Vbe=0.7;


Rb=R1*R2/(R1+R2)
Vbb=Vcc*R2/(R1+R2)
Ic=(Vbb-Vbe)/Re
Vc=Vcc-Rc*Ic
Ve=Re*Ic
Vce=Vc-Ve
pause

beta=50

Ib=(Vbb-Vbe)/((beta+1)*Re+Rb);
Ic=beta*Ib
Ie=Ic+Ib;
Vc=Vcc-Ic*Rc
Ve=Ie*Re
Vce=Vc-Ve
Vb=Ve+Vbe
