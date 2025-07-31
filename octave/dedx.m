# -*- texinfo -*-
# @deftypefn {Function File} {[@var{E}, @var{S}]=} dedx (@var{Z1}, @var{Z2}, @var{model})
#
# Return the electronic stopping cross-section table for projectile @var{Z1}, target @var{Z2} and 
# model/parametrization defined by @var{model}.
#
# 1 <= Z1,Z2 <= 92
# 
# @var{model} can take up the values "SRIM96", "SRIM13" and "DPASS22".
#
# @var{E} is the projectile energy in keV/u, [1x121] vector
# @var{S} is the stopping cross-section in 10^{-15} eV-cm^2, [1x121] vector
#
# @end deftypefn
#

function [e, s] = dedx (Z1,Z2,model)

if nargin<3,
  model = "SRIM96";
endif

if !ischar(model),
  error("dedx2: 3rd arg must be a string");
endif

k=-1;
model_types = {"SRIM96", "SRIM13", "DPASS22"};

for i=1:length(model_types)
  if strcmp(model_types{i},model),
    k = i-1;
    break;
  endif
endfor

if k<0,
  error("dedx: 3rd arg is not a valid stopping model");
endif

[e, s] = __dedx__(Z1,Z2,k);
