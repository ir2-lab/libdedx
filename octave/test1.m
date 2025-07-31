## TEST1
#
# Plot stopping cross-section Se for various 
# projectile/target combinations, for the 3 parametrizations/models
clear

function plot_dedx(Z1,Z2, label)
  S = zeros(3,121);

  [E, S(1,:)] = dedx(Z1, Z2, "SRIM96");
  [E, S(2,:)] = dedx(Z1, Z2, "SRIM13");
  [E, S(3,:)] = dedx(Z1, Z2, "DPASS22");

  loglog(E, S, '.-')

  xlabel('E/M1 (keV/u)')
  ylabel('S_e (10^{-15} eV-cm^2)')
  title(sprintf("%s, Z1 = %d, Z2 = %d",label, Z1,Z2))
  legend("SRIM96","SRIM13","DPASS22")
endfunction

figure 1
clf
% Cl - C
plot_dedx(17,6,'Cl in C')

figure 2
clf
% N - Ar
plot_dedx(7,18,'N in Ar')

figure 3
clf
plot_dedx(26,26,'Fe in Fe')



