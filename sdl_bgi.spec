Summary:    BGI-compatible 2D graphics C library
Name:       SDL_bgi
Version:    2.0.1
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
mkdir -p $RPM_BUILD_ROOT/%{_includedir}/SDL2/
cp libSDL_bgi.so $RPM_BUILD_ROOT/%{_libdir}
cp SDL_bgi.h $RPM_BUILD_ROOT/%{_includedir}/SDL2/

%clean
rm -rf $RPM_BUILD_ROOT

%post
if [ -r /usr/include/graphics.h ]; then
  echo "graphics.h detected - skipping";
else 
  echo "creating symbolic link graphics.h -> SDL_bgi.h";
  ln -sf /usr/include/SDL2/SDL_bgi.h /usr/include/graphics.h
fi

%%postun
if [ -r /usr/include/graphics.h ]; then
  echo "Warning - /usr/include/graphics.h not deleted."
fi

%files
%defattr(644,root,root,755)
%doc 00_README Changelog doc/ INSTALL LICENSE sdl_bgi.spec
%doc src/test/ TODO VERSION
%attr(755,root,root) %{_libdir}/*
%attr(644,root,root) %{_includedir}/SDL2/*

%changelog
* Tue May 12 2015 Guido Gonzato <guido.gonzato at gmail.com>
Version 2.0.1.

* Thu Apr 30 2015 Guido Gonzato <guido.gonzato at gmail.com>
Version 2.0.

* Thu Nov 6 2014 Guido Gonzato <guido.gonzato at gmail.com>
This is a generic rpm, buildable on Ubuntu

