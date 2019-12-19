
function activefilter

w=1000:10000:10000000000;


w1=10^6;
w2=10^8;
w3=10^4;
BPplot(w,w1,w2,w3,1)

BSplot(w,w1,w2,w3,2)


end


function BPplot(w,w1,w2,w3,k)
h1=w1./(j*w+w1);
h2=w2./(j*w+w2);
h3=j*w./w3;
h=h1.*h2.*h3;
subplot(2,1,k);
loglog(w,abs(h1),w,abs(h2),w,abs(h3),w,abs(h))
end

function BSplot(w,w1,w2,w3,k)
h1=j*w+w1;
h2=j*w+w2;
h3=w3./(j*w);
h=h1.*h2.*h3;
subplot(2,1,k);
loglog(w,abs(h1),w,abs(h2),w,abs(h3),w,abs(h))
end
