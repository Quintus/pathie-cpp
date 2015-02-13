README
======

This is the Pathie project. It aims to provide a C++ library that covers
all needs of pathname manipulation and filename fiddling, without
having to worry about the underlying platform. That is, it is a glue
library that allows you to create platform-independent filename
handling code with special regard to Unicode path names.

Supported systems
-----------------

Currently supported platforms are Linux with GCC and Windows with GCC
(via MinGW). Any other compiler or system might or might not work.

It is recommended to use at least GCC 4.9.0 due to full implementation
of C++11.

The library
-----------

The entire world is using UTF-8 as the primary Unicode encoding. The
entire world? No, a little company from Redmond resists the temptation
and instead uses UTF-16LE, causing cross-platform handling of Unicode
pathes to be a nightmare.

One of the main problems the author ran into was compiler-dependant
code that was not marked as such. Many sites on the Internet claim
Unicode path handling on Windows is easy, but in fact, it only is if
you define “development for Windows” as “development with MSVC”,
Microsoft’s proprietary C/C++ compiler, which provides nonstandard
interfaces to allow for handling UTF-16LE filenames. The pathie
library has been developed with a focus on MinGW and crosscompilation
from Linux to Windows and thus does not suffer from this problem. It
is guaranteed to work both with native g++ on Linux, native g++ via
MinGW on Windows, and crosscompilation MinGW g++ on Linux. I will
accept patches for other compilers, but I will not thoroughly test on
them.

Even more, Pathie’s author is of the strong opinion that UTF-8 is the
correct encoding to use, and is not willing to do differently on
Windows. Visit the [UTF8 Everywhere](http://www.utf8everywhere.org)
website if you need good arguments for this. However, the Windows
world of course disagrees, and this is where the pathie library comes
in. It allows you to do your entire pathname handling in UTF-8, and
transparently converts to whatever pathname encoding the underlying
system uses, such as UTF-16LE on Windows, without having you
notice.

Usage
-----

First thing is to include the main header:

~~~~~~~~~~~~~~~~~~{.cpp}
#include <pathie.hpp>
~~~~~~~~~~~~~~~~~~

This will include any other headers as well. There should never be a
need to include pathie’s complement headers.

Now consider the simple task to get all children of a directory, which
have Unicode filenames. Doing that manually will result in you having
to convert between UTF-8 and UTF-16 all the time. With pathie, you can
just do this:

~~~~~~~~~~~~~~~~~~~{.cpp}
std::vector<Pathie::Path> children = your_path.children();
~~~~~~~~~~~~~~~~~~~

Done. Retrieving the parent directory of your directory is easy as
hell:

~~~~~~~~~~~~~~~~~~~{.cpp}
Pathie::Path yourpath("foo/bar/baz");
Pathie::Path parent = yourpath.parent();
~~~~~~~~~~~~~~~~~~~

But Pathie is much more than just an abstraction of different filepath
encodings. It is a utility library for pathname manipulation, i.e. it
allows you to do things like finding the parent directory, resolving
symlinks (yes, including NTFS symlinks on Windows), expanding relative
to absolute pathes, decomposing a filename into basename, dirname, and
extension, and so on. See the documentation of the central
Pathie::Path class on what you can do.

~~~~~~~~~~~~~~~~~~~~~~{.cpp}
// Assume current directory is /tmp
Pathie::Path p("foo/bar/../baz");
p.expand(); // => /tmp/foo/baz
~~~~~~~~~~~~~~~~~~~~~~

Or my personal favourite:

~~~~~~~~~~~~~~~~~~~{.cpp}
Pathie::Path p1("/tmp/foo/bar");
Pathie::Path p2("/tmp/bar/foo");
Pathie::Path p3 = p1.relative(p2); // => ../../foo/bar
~~~~~~~~~~~~~~~~~~~

It also provides you with commonly used pathes like the user’s
configuration directory or the path to the running executable.

~~~~~~~~~~~~~~~~~~~~{.cpp}
Pathie::Path exepath = Pathie::Path::exe();
~~~~~~~~~~~~~~~~~~~~

And best of it? All arguments you pass to Pathie are UTF-8, and
everything you get back is encoded in UTF-8 as well.

Still, if you interface directly with the Windows API or other external
libraries, you might want to retrieve the native representation from a
Path or construct a Path from the native representation. Pathie
doesn’t want to be in your way then:

~~~~~~~~~~~~~~~~~~~~{.cpp}
// Contruct from native
wchar_t* utf16 = Win32ApiCall();
Path mypath = Path::from_native(utf16); // also accepts std::wstring

// Retrieve native (Note C++’ish std::wstring rather than
// raw wchar_t*)
std::wstring native_utf16 = mypath.native();
~~~~~~~~~~~~~~~~~~~~

On UNIX, these methods work with normal strings in the underlying
filesystem encoding. In most cases, that will be UTF-8, but some
legacy systems may still use something like ISO-8859-1 in which case
that will differ. Therefore, only use the `Path` constructor if you
are constructing from a UTF-8 string!

### Stream replacements

Pathie mainly provides you with the means to handle pathes, compose,
and decompose them. Inspired by the [boost::nowide
library](http://cppcms.com/files/nowide/html/) library, however, it
also provides you with drop-in replacements for `std::ifstream` and
`std::ofstream` that flawlessly work on Unicode pathes even on
Windows. See the documentation of Pathie::ofstream for more
information.

~~~~~~~~~~~~~~~~~{.cpp}
Pathie::Path p("Bärenstark.txt");
Pathie::ofstream file(p);
file << "Some content" << std::endl;
file.close()
~~~~~~~~~~~~~~~~~

Dependencies and linking
------------------------

Pathie is standalone, that is, it requires no other libraries except
for those provided by your operating system. Note that there’s a
caveat with this on Windows, which does provide the `shlwapi.lib`
library by default, but does not automatically link it in. Be sure to
add that library to your linking command when compiling for
Windows.

It is recommended to link in pathie as a dynamic library, because
there are some problems with it when linked statically on certain
operating systems (see _Caveats_ below). If you are sure you aren’t
affected by those problems, it is possible to link in pathie
statically.

Caveats
-------

This library assumes that under all UNIX systems out there (I also
consider Mac OSX to be a UNIX system) the file system root always is
`/` and the directory separator also always is `/`. This structure is
mandatory as per POSIX -- in POSIX.1-2008, it’s specified in section
10.1. Systems which do neither follow POSIX directory structure, nor
are Windows, are unsupported.

On POSIX-compliant systems other than Mac OS X, the filesystem
encoding [generally is
unspecified](https://unix.stackexchange.com/questions/2089/what-charset-encoding-is-used-for-filenames-and-paths-on-linux).
Pathnames are merely byte blobs which do not contain NUL bytes, and
components are separated by `/`. It’s up to the applications,
including utilities like a shell or the ls(1) program, to make
something of those byte streams. Therefore, it is perfectly possible
that on one system, user A uses ISO-8859-1 filenames and user B uses
UTF-8 filenames. Even the same user could use differently encoded
filenames. Programs that have to interpret the byte blobs in pathnames
on these systems look at the locale environment variables, namely
`LANG` and `LC_ALL`, see section 7 of POSIX.1-2008. As a consequence,
it may happen you want to create filenames with characters not
supported in the user’s pathname encoding. For example, if you want to
create a file with a hebrew filename and the user’s pathname encoding
is ISO-8859-1, there’s a problem, because ISO-8859-1 has no hebrew
characters in it, but in UTF-8, which is the encoding you are advised
to use and which is what Pathie’s API expects from you, they are
available. There is no sensible solution to this problem that the
Pathie library could dictate; the `iconv()` function used by pathie
just replaces characters that are unavailable in the target encoding
with a system-defined default (probably “?”). Note that on systems
which have a Unicode pathname encoding, especially modern Linuxes with
UTF-8, such a situation can’t ever arise, because the Unicode
encodings (UTF-*) cover all characters you can ever use.

Note a possible problem in Pathie::Path::expand() when you link Pathie
in statically and depend on expanding pathes like `~username/foo` on
UNIX. See Pathie::Path::expand() docs for information on this.

At least on FreeBSD, calling the POSIX `iconv()` function fails with
the cryptic error message “Service unavailable” if a program is linked
statically. I’ve reported [a bug on
this](https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=196567). This
means that you currently can’t link in pathie statically on FreeBSD
and systems which don’t allow statically linked executables to call
`iconv()`.

It is recommended to set your program’s locale to the environment’s
locale before you call any functions the Pathie library provides on a
UNIX system, because this will allow Pathie to use the correct
encoding for filenames on UNIX systems. This is relevant where the
environment’s encoding is not UTF-8, e.g. with $LANG set to
`de_DE.ISO-8859-1`. You can do this as follows (the `""` locale always
refers to the locale of the environment):

~~~~~~~~~~~~~~~~~~~~~{.cpp}
#include <locale>
std::locale::global(std::locale(""));
~~~~~~~~~~~~~~~~~~~~~

Don’t you duplicate boost::filesystem?
-------------------------------------

Yes and
no. [boost::filesystem](http://www.boost.org/doc/libs/1_56_0/libs/filesystem/doc/index.htm)
provides many methods pathie provides, but has a major problem with
Unicode path handling if you are not willing to do the UTF-8/UTF-16
conversion manually. boost::filesystem always uses UTF-8 to store the
pathes on UNIX, and, which is the problem, always uses UTF-16LE to
store the pathes on a Windows system. There is no way to override
this, although there is a [hidden documentation
page](http://www.boost.org/doc/libs/1_51_0/libs/locale/doc/html/default_encoding_under_windows.html)
that claims to solve the problem. I have wasted a great amount of time
to persuade boost::filesystem to automatically convert all
`std::string` input it receives into UTF-16LE, but failed to
succeed. Each time I wanted to create a file with a Unicode filename,
the test failed on Windows by producing garbage filenames. Finally I
found out that the neat trick shown in the documentation above indeed
does work -- but only if you use the Microsoft Visual C++ compiler
(MSVC) to compile your code. I don’t, I generally use g++ via the
[MinGW](http://www.mingw.org) toolchain. boost::filesystem fails with
g++ via MinGW with regard to Unicode filenames on Windows entirely and
completely as of this writing (September 2014).

Apart from that, pathie provides some additional methods, especially
with regard to finding out where the user’s pathes are. It is modelled
after Ruby’s popular
[Pathname](http://ruby-doc.org/stdlib-2.1.2/libdoc/pathname/rdoc/Pathname.html#method-i-rmtree)
class, but it doesn’t entirely duplicate its interface (which wouldn’t
be idiomatic C++).

Also, pathie is a small library. Adding it to your project shouldn’t
hurt too much, while boost::filesystem is quite a large dependency.

Contributing
------------

This library is free software under the GPL (see _License_ below), so
you can basically do with it as you like. If you want me to include
your changes into the main repository, I will happily do so after you
have signed a Contributor License Agreement (CLA) as outlined at
<http://www.guelkerdev.de/contributing/>. I’ll ask you to do so if you
submit something to me, so don’t worry if you forget it.

Other than that, the usual things apply. Try to make separate branches
for your new features, give a description on what you changed,
etc. The main unstable development branch is `devel`, so unless you
are working on release-specific fixes this is where you should target
your PR at.

License
-------

Pathie is a path management library.

Copyright © 2015 Marvin Gülker

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

### Commercial use ###

If you have the urgent feeling that you don’t want to help free
software to spread, feel free to contact me. We might be able to work
something out on an individual base.

### Contact information ###

My email address is <m-guelker@guelkerdev.de>. You can find my
paper mail address on [my website](http://www.guelkerdev.de).

_To not use UTF-16LE, nevermore…_
