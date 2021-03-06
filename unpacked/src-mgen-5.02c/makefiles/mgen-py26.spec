%define _topdir @TOPDIR@
%define name mgen-py26
%define release 1%{?dist}
%define version @VERSION@
%define buildroot %{_topdir}/%{name}-%{version}-root

BuildRoot: %{buildroot}
Summary: A packet generator for IP network performance testing
License: BSD
URL: https://www.nrl.navy.mil/itd/ncs/products/mgen
Name: %{name}
Version: %{version}
BuildRequires: gcc-c++
BuildRequires: python-devel
Requires: mgen
Release: %{release}
Source0: src-mgen-%{version}.tgz
Patch0: mgen-protolib-rhel.patch
Prefix: /usr

%description
MGEN provides the ability to perform IP network performance tests and measurements
using TCP and UDP/IP traffic. Test messages can be generated, received and logged.
MGEN offers control over all network parameters and timing of these messages. This
can be done via the command line or via an input file (for greater reproducibility).

gpsPub publishes GPS coordinates for use by MGEN in CORE-created containers.

This package provides Python 2.6 bindings for MGEN and gpsPub.

%prep
%setup -q -n src-mgen-%{version}

%build
python setup.py build

%install
python setup.py install --root %{buildroot}

%files
%defattr(0644, root, root)
/usr/lib/python*/site-packages/mgen.py*
/usr/lib/python*/site-packages/mgen-1.0-py*.egg-info
%{_libdir}/python*/site-packages/gpsPub-1.0-py*.egg-info
%attr(0755, root, root) %{_libdir}/python*/site-packages/_gpsPub.so
/usr/lib/debug/%{_libdir}/python*/site-packages/_gpsPub.so.debug
/usr/src/debug/src-mgen-5.02c/include/gpsPub.h
/usr/src/debug/src-mgen-5.02c/src/common/gpsPub.cpp
/usr/src/debug/src-mgen-5.02c/src/python/gpsPub_wrap.c

%changelog
* Mon Dec 31 2018 Ben Stern <bstern@fortian.com> - 5.02c-1
- Initial release
