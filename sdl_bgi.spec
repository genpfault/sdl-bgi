Summary:    BGI-compatible 2D graphics C library
Name:       SDL_bgi
Version:    1.0.1
Release:    1
License:    ZLib
Group:      Libraries
Source:     %{name}-%{version}.tar.gz
URL:        http://libxbgi.sourceforge.net/
Requires:   libsdl-gfx1.2-dev
Prefix:     %{_prefix}
BuildRoot:  %{_tmppath}/%{name}-%{version}-buildroot

%description
This package contains a Borland Graphics Interface (BGI) emulation
library based on SDL. This library strictly emulates most BGI
functions, making it possible to compile SDL versions of programs
written for Turbo/Borland C. RGB extensions and basic mouse support
are also implemented.

%prep
%setup -q -n %{name}-%{version}

%build
cd src
make

%install
rm -rf $RPM_BUILD_ROOT
cd src
mkdir -p $RPM_BUILD_ROOT/%{_libdir}
mkdir -p $RPM_BUILD_ROOT/%{_includedir}
mkdir -p $RPM_BUILD_ROOT/%{_includedir}/SDL/
cp libSDL_bgi.so $RPM_BUILD_ROOT/%{_libdir}
cp SDL_bgi.h $RPM_BUILD_ROOT/%{_includedir}/SDL/

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(644,root,root,755)
%doc 00_README Changelog Functions.txt INSTALL LICENSE sdl_bgi.spec
%doc src/test/ TODO Using.txt VERSION
%attr(755,root,root) %{_libdir}/*
%attr(644,root,root) %{_includedir}/SDL/*

%changelog
* Thu Nov 6 2014 Guido Gonzato <guido.gonzato at gmail.com>
This is a generic rpm, buildable on Ubuntu
