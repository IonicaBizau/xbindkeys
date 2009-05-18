Summary: Binds keys or mouse buttons to shell commands under X.
Name: xbindkeys
Version: 1.8.3
Release: 1
License: GPL v2
Group: User Interface/X
Source: http://hocwp.free.fr/xbindkeys/xbindkeys-%{version}.tar.gz
URL: http://hocwp.free.fr/xbindkeys/xbindkeys.html
BuildRoot: %{_tmppath}/%{name}-%{version}-root
Requires: tk

%description
xbindkeys is a program that allows you to launch shell commands with your keyboard or mouse under X. It links commands to keys or mouse buttons using a simple configuration file, and is independant of the window manager.

%prep
%setup -q

%build
./configure --mandir="%{_mandir}" --bindir="%{_bindir}"
make CFLAGS="$RPM_OPT_FLAGS"

%install
rm -rf %{buildroot}
make install DESTDIR="%{buildroot}"

%clean
rm -rf %{buildroot}

%files
%defattr(-, root, root)
%doc AUTHORS BUGS ChangeLog COPYING INSTALL NEWS README TODO
%attr(0755, root, root) %{_bindir}/xbindkeys
%attr(0755, root, root) %{_bindir}/xbindkeys_show
%attr(0644, root, root) %{_mandir}/man?/*

%changelog
* Sat Jan 24 2009 Philippe Brochard <hocwp@free.fr>
- build for 1.8.3

* Mon Apr 16 2007 Philippe Brochard <hocwp@free.fr>
- build for 1.8.2

* Sun Apr 1 2007 Philippe Brochard <hocwp@free.fr>
- build for 1.8.1

* Thu Jan 18 2007 Philippe Brochard <hocwp@free.fr>
- build for 1.8.0

* Sat Oct 07 2006 Philippe Brochard <hocwp@free.fr>
- build for 1.7.4

* Sat May 27 2006 Philippe Brochard <hocwp@free.fr>
- build for 1.7.3

* Wed Oct 20 2004 Philippe Brochard <hocwp@free.fr>
- build for 1.7.2

* Thu Jan 08 2004 Philippe Brochard <hocwp@free.fr>
- build for 1.7.1

* Fri Nov 15 2002 David Zambonini <dave@alfar.co.uk>
- build for 1.6.3

* Wed Nov 13 2002 David Zambonini <dave@alfar.co.uk>
- second 1.6.2 release - fixed default attributes on docs (oops!)

* Tue Nov 12 2002 David Zambonini <dave@alfar.co.uk>
- build for 1.6.2. wheee

* Tue Nov 12 2002 David Zambonini <dave@alfar.co.uk>
- build for 1.6.1. that was quick

* Sun Nov 10 2002 David Zambonini <dave@alfar.co.uk>
- build for 1.6

* Sat Nov 01 2002 David Zambonini <dave@alfar.co.uk>
- Added additional documents, tweaked description

* Mon Oct 30 2002 David Zambonini <dave@alfar.co.uk>
- Initial revision.

