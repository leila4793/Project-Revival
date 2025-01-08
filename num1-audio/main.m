clc
close all
clear all
[s,fs]=wavread('project1.wav');
i1=size(s);
figure
for i=1:512:i1
    if (i+511)>= i1
        f1=s(i:i1);
    end
f1=s(i:i+511);
ff1=fft(f1);
ff2=abs(ff1);
subplot(1,3,1)
stem(f1); title('original signal');
subplot(1,3,2)
stem(ff1); title('fft signal');
subplot(1,3,3)
stem(ff2); title('abs(fft) signal');
pause(0.5)
end