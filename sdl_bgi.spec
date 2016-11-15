Summary:    BGI-compatible 2D graphics C library
Name:       SDL_bgi
Version:    2.0.8
Release:    1
License:    ZLib
Group:      Libraries
Source:     %{name}-%{version}.tar.gz
URL:        http://libxbgi.sourceforge.net/
Requires:   libsdl2-dev
Prefix:     %{_prefix}
BuildRoot:  %{_tmppath}/%{name}-%{version}-buildroot

%description 
This package contains a Borland Graphics Interface (BGI) emulation
library based on SDL2. This library strictly emulates most BGI
functions, making it possible to compile SDL versions of programs
written for Turbo/Borland C. ARGB extensions and basic mouse support
are also implemented; further, native SDL2 functions may be used in
SDL_bgi programs.

%prep
%setup -q -n %{name}-%{version}

%build
cd src
make

%install
rm -rf $RPM_BUILD_ROOT
cd src
mkdir -p $RPM_BUILD_ROOT/usr/lib/
mkdir -p $RPM_BUILD_ROOT/%{_includedir}
mkdir -p $RPM_BUILD_ROOT/%{_includedir}/SDL2/
/usr/bin/install -m 644 lib%{name}.so.%{version} $RPM_BUILD_ROOT/usr/lib/
/usr/bin/install -m 644 SDL_bgi.h $RPM_BUILD_ROOT/%{_includedir}/SDL2/

%clean
rm -rf $RPM_BUILD_ROOT

%post
/bin/ln -sf /usr/include/SDL2/SDL_bgi.h /usr/include/graphics.h
cd /usr/lib; /bin/ln -sf lib%{name}.so.%{version} /usr/lib/lib%{name}.so

%%postun
/bin/rm -f /usr/include/graphics.h
/bin/rm -f /usr/lib/lib%{name}.so

%files
%defattr(644,root,root,755)
%doc AUTHORS ChangeLog doc/ INSTALL LICENSE README
%doc sdl_bgi.spec test/ TODO VERSION
%attr(644,root,root) /usr/lib/*
%attr(644,root,root) %{_includedir}/SDL2/*

%changelog
* Thu Nov 6 2014 Guido Gonzato <guido.gonzato at gmail.com>
This is a generic rpm, buildable on Ubuntu
