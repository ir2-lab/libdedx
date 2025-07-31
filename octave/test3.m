## Test low-E extrapolation 
#
# Below 1 keV/u the stopping XS is S ~ E^(1/2) (like a friction term)
#
# 1. Get stopping cross sections for a Z1-Z2 combination
# 2. Compute low-E extrapolation for 0<= E < 50 keV/u
# 3. Plot extrap + data vs E^1/2
#
clear

% Cl - C
Z1 = 17;
Z2 = 6;

% N - Ar
# Z1 = 7;
# Z2 = 18;

% Fe - Fe
# Z1 = 1;
# Z2 = 26;

S = zeros(3,121);

[E, S(1,:)] = dedx(Z1, Z2, "SRIM96");
[E, S(2,:)] = dedx(Z1, Z2, "SRIM13");
[E, S(3,:)] = dedx(Z1, Z2, "DPASS22");

Emax = 50;
i = find(E<=Emax);
E = E(i);
S = S(:,i);

n = 0.5;
sqrtx = linspace(0,floor(sqrt(Emax)),31);
Sx = S(:,1)*sqrtx;
%loglog(A(:,1),A(:,2),'.',x,A(1,2)*x.^0.5)

figure 1
clf
plot(sqrt(E),S,'.-',sqrtx,Sx)
xlabel('(keV/u)^{1/2}')
ylabel('10^{-15} eV-cm^2')
title(sprintf("Z1 = %d, Z2 = %d",Z1,Z2))
legend("SRIM96","SRIM13","DPASS22")

