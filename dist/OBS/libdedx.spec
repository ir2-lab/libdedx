# set next variable to 1 if debug package is needed, else leave it to nil
# only for rpm builds
%define         debug_build                 %{nil}

Name:           libdedx
Version:        0
Release:        0
Summary:        Electronic Stopping
License:        GPL-3.0-or-later
Url:            https://github.com/ir2-lab/libdedx.git

Source0:        %{name}.tar.gz

%if "%{_vendor}" == "debbuild"
Packager:       M. Axiotis <psaxioti@gmail.com>
   %if 0%{?ubuntu_version} >= 2204 || 0%{?debian_version} >= 1100
BuildRequires:  debhelper-compat = 13
   %else
BuildRequires:  debhelper-compat = 12
   %endif
BuildRequires:  debbuild-macros
%endif

BuildRequires:  cmake >= 3.10
BuildRequires:  %{!?_debbuild:gcc-c++}      %{?_debbuild:g++}

%description
A compilation of different parametrizations for the electronic stopping of ions in matter.

%package        devel
Summary:        Development files for %{name}

Requires:       %{name}

%description    devel
Development files for libdedx.

###############################################################################################################################

%if "%{_vendor}" != "debbuild" && "%{debug_build}" == "1"
%debug_package
%endif

###############################################################################################################################

%prep
%setup -q -n    %{name}

%build
%cmake \
   -DCMAKE_INSTALL_PREFIX=%{_prefix} \
%if "%debug_build" == "1"
   -DCMAKE_BUILD_TYPE=RelWithDebInfo \
%else
   -DCMAKE_BUILD_TYPE=Release \
%endif
   -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
   %{nil}
%cmake_build

%install
%cmake_install

%if "%{_vendor}" == "debbuild" || "%{debug_build}" != "1"
strip --strip-unneeded %{buildroot}%{_libdir}/%{name}.so
%endif

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
