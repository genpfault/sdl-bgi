# sdl_bgi.spec - tested on Fedora 26+

%global debug_package %{nil} 

Summary:       BGI-compatible 2D graphics C library
Name:          SDL_bgi
Version:       2.3.0
Release:       1
License:       ZLib
Group:         Libraries
Source:        %{name}-%{version}.tar.gz
URL:           http://libxbgi.sourceforge.net/
BuildRequires: SDL2-devel
Prefix:        %{_prefix}
BuildRoot:     %{_tmppath}/%{name}-%{version}-buildroot

%description 
SDL_bgi is a Borland Graphics Interface (BGI) emulation library based
on SDL2. This library strictly emulates most BGI functions, making it
possible to compile SDL2 versions of programs written for Turbo/Borland
C. ARGB extensions and basic mouse support are also implemented;
further, native SDL2 functions may be used in SDL_bgi programs.

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
/usr/bin/install -m 644 graphics.h $RPM_BUILD_ROOT/%{_includedir}
/usr/bin/install -m 644 SDL_bgi.h  $RPM_BUILD_ROOT/%{_includedir}/SDL2/
/usr/bin/install -m 644 lib%{name}.so $RPM_BUILD_ROOT/%{_libdir}

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(644,root,root,755)
%doc AUTHORS BUGS ChangeLog doc/ INSTALL.md LICENSE README.md
%doc sdl_bgi.spec test/ TODO VERSION
%attr(644,root,root) %{_libdir}/lib%{name}.so
%attr(644,root,root) %{_includedir}/SDL2/SDL_bgi.h
%attr(644,root,root) %{_includedir}/graphics.h

%changelog
* Tue Jan 2 2018 Guido Gonzato <guido.gonzato at gmail.com>
Slightly improved portability.

* Sun Oct 22 2017 Guido Gonzato <guido.gonzato at gmail.com>
Slightly improved portability.

* Thu Nov 6 2014 Guido Gonzato <guido.gonzato at gmail.com>
This is a generic rpm, also buildable on Ubuntu
