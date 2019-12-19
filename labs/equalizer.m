% This program demonstrates various filtering effects of a sound signal

function equalizer
    close all;

    load handel;            % load Handel's Hallelujah Chorus
    N=size(y,1);            % size of data
    F=Fs;                     % sampling frequency
    t0=1/F;                  % sampling period
    T=N*t0;                  % total time period
    f0=F/N;                  % fundamental frequency
    f=-F/2:f0:F/2-f0;    % frequency axis
    t=0:t0:T-t0;            % time axis
    signal=y;
    fprintf('N=%d, F=%.0f, t0=%f, T=%.2f, f0=%f\n',N,F,t0,T,f0);

    B=8;                        % number of frequency bands
    bw=N/2/B;               % bandwidth of each band

    while 1
        type=input('filter type: Ideal (0), Butherworth (1) ')
        x=signal;      
        subplot(B+1,3,1)
        plot(t,real(x))             % plot signal in time
        xlim([0 0.03]);            % show a short piece of time signal
        X=fft(x);                    % spectrum of x
        subplot(B+1,3,2)
        plot(f,fftshift(real(X)),f,fftshift(imag(X))); % plot spectrum of signal
        xlim([-F/2 F/2])
        pause;
        sound(x,F);                  % hear the original signal

        for b=1:B                     % for all B frequency bands

            fl=fix((b-1)*bw)+1;   % cutoff frequency (low)
            fh=fix(b*bw);            % cutoff frequency (high)
    
            W=filters(type,fl,fh,N);    % get filter (0 for ideal, 1 for Butterworth)
            Y=W.*X;                  % filtering
    
            y=ifft(Y);               % back to time by inverse transform

            subplot(B+1,3,3*b+3)
            plot(f,fftshift(W))        % plot filter in frequency domain
            xlim([-F/2 F/2])
            ylim([-0.2 1.2]);
    
            subplot(B+1,3,3*b+2)  % plot filtered signal in frequency domain
            plot(f,fftshift(real(Y)),f,fftshift(imag(Y))); 
            xlim([-F/2 F/2])

            subplot(B+1,3,3*b+1)
            plot(t,real(y))          % plot filtered signal in time domain
            xlim([0 0.03]);
            pause;
            sound(y,F);             % hear the filtered signal

        end
    end
end

function w=filters(type,fl,fh,N)
    n2=N/2;              % half-length of filter
    fprintf('fl=%.1f,  fh=%.1f, N=%d, n2=%d\n',fl,fh,N,n2);

    k=16;                  % order of Butterworth 
    w=zeros(N,1);
    switch type
        case 0              % ideal filter
            w(fl:fh)=1;  
            
        case 1              % Butterworth filter
            
            lp=sqrt(1./(1+((1:n2)/fh).^(2*k)));
            hp=1-sqrt(1./(1+((1:n2)/fl).^(2*k)));
            w(1:n2)=lp.*hp;   % BP = LP * HP
    end
    w(n2+1:N)=w(n2:-1:1);
end
