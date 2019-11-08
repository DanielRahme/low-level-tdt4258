Fs = 44.1e3;

f = zeros(1,12);
Th = zeros(1,12);
for N = 0:11
    f(N+1) = 16.35*2^(N/12);
    Th(N+1) = round(Fs/f(N+1)/2);
end
disp(f)
disp(Th)

 csvwrite('periods0.dat',Th)