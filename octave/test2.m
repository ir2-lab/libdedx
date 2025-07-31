## Test error due to log-log interp
#
# This does the following
# 1. get stopping cross-section for a projectile/target combination
# 2. Define a logarithmic energy range Ef for interpolation
# 3. Use splines to get Se(Ef)
# 4. Do log-log interp to points in-between Ef points
# 5. Get the difference from spline interp at these points
#
clear

% Cl - C
Z1 = 17;
Z2 = 6;

% Fe - Fe
Z1 = 26;
Z2 = 26;

S = zeros(3,121);

[E, S(1,:)] = dedx(Z1, Z2, "SRIM96");
[E, S(2,:)] = dedx(Z1, Z2, "SRIM13");
[E, S(3,:)] = dedx(Z1, Z2, "DPASS22");

# corteo style log range
function X = corteospace(Emin,Emax,N)
  L = 2^N;
  D = L*(Emax-Emin);
  i = 0:D;
  m = 1 + mod(i,L)/L;
  E = Emin + (i-mod(i,L))/L;
  X = m.*2.^E;
endfunction

# Define fine logarithim energy range
# Ef = logspace(0,6,25*6+1); 
Ef = corteospace(0,20,3);

# Show the properties of Ef range
n=length(Ef);
disp(["Points  = " num2str(n)])
disp(["Points/decade = " num2str((n-1)/6)])

# get the values of S at Ef with log-splines
Sf = exp(spline(log(E),log(S),log(Ef)));

# get points in-between Ef
Ef2 = exp( 0.5*(log(Ef(1:end-1)) + log(Ef(2:end))) );

# get "correct" values at Ef2 with splines
Sf2 = exp(spline(log(Ef),log(Sf),log(Ef2)));

# get interpolated values at Ef2 
method = 'linear'; # log-log interp
# method = 'nearest'; # no interp
S1(1,:) = exp(interp1(log(Ef),log(Sf(1,:)),log(Ef2),method));
S1(2,:) = exp(interp1(log(Ef),log(Sf(2,:)),log(Ef2),method));
S1(3,:) = exp(interp1(log(Ef),log(Sf(3,:)),log(Ef2),method));

# plot Se(E)
figure 1
clf
loglog(E,S,'o',Ef,Sf,'.-')
xlabel('keV/u')
ylabel('10^{-15} eV-cm^2')
title(sprintf("Z1 = %d, Z2 = %d",Z1,Z2))
legend('Data','Spline interp')

figure 2
dY = abs((S1-Sf2)./Sf2).';
semilogx(Ef2,dY)
xlabel('keV/u')
ylabel('|S-S_i|/S (%)')
title('Relative interpolation error')
disp(['mean |S-Si|/S = ' num2str(mean(dY),'%.1e ')])
disp(['max  |S-Si|/S = ' num2str(max(dY),'%.1e ')])

