
r0=5.78; 
r1=24.1;
r2=44; 
r3=0.0178; 
r4=0.02; 
V=3;

R = r4 + parallel( r3, (r2+parallel(r0,r1)) );
i4=V/R
[i2, i3] = currentdiv(i4, r2+parallel(r0,r1), r3 );
[i0, i1] = currentdiv(i2, r0,r1 );
fprintf('i4=%.3f, i3=%.3f, i2=%.3f, i1=%.3f, i0=%.3f\n',i4,i3,i2,i1,i0);
