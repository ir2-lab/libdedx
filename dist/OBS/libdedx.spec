%global         debug_package     %{nil}
%global         __strip           /bin/true

Name:           libdedx
Version:        0
Release:        0
Summary:        Electronic Stopping
License:        GPL-3.0-or-later
Url:            https://github.com/ir2-lab/libdedx.git

Source0:        %{name}.tar.gz

%if "%{_vendor}" == "debbuild"
Packager:       M. Axiotis <psaxioti@gmail.com>
%endif

BuildRequires:  cmake >= 3.10

%if "%{_vendor}" == "debbuild"
   %if 0%{?ubuntu_version} >= 2204 || 0%{?debian_version} >= 1100
BuildRequires:  debhelper-compat = 13
   %else
BuildRequires:  debhelper-compat = 12
   %endif
BuildRequires:  debbuild-macros

BuildRequires:  g++
%else
BuildRequires:  gcc-c++
%endif

%description
A compilation of different parametrizations for the electronic stopping of ions in matter.

%package        devel
Summary:        Development files for %{name}

Requires:       %{name}

%description    devel
Development files for C++ Monte-Carlo code for simulating ion transport in materials with an emphasis on the calculation of material damage.

%prep
%setup -q -n    %{name}

%build
%cmake \
   -DCMAKE_INSTALL_PREFIX=%{_prefix} \
   -DCMAKE_BUILD_TYPE=Release \
   -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
   %{nil}
%cmake_build

%install
%cmake_install

strip --strip-unneeded %{buildroot}%{_libdir}/%{name}.so

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%doc README.md
%{_libdir}/%{name}.so

%files devel
%{_includedir}/%{name}.h
%dir %{_libdir}/cmake/%{name}
%{_libdir}/cmake/%{name}/%{name}*.cmake

%changelog
