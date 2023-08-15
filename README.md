[![Github Build status](https://github.com/vtorri/libdvi/actions/workflows/ci_linux.yml/badge.svg)](https://github.com/vtorri/libdvi/actions?query=workflow%3A%22GitHub+CI%22)
[![Github Build status](https://github.com/vtorri/libdvi/actions/workflows/ci_msys2.yml/badge.svg)](https://github.com/vtorri/libdvi/actions?query=workflow%3A%22GitHub+CI%22)
[![CodeFactor](https://www.codefactor.io/repository/github/vtorri/libdvi/badge)](https://www.codefactor.io/repository/github/vtorri/libdvi)

### libdvi

DVI rendering library written in C99

### License

This application is released under the LGPL 2.1 or later

### Requirements

The kpathsea library (provided by TeXLive or MIKTeX) is required.
Preferably install TeXLive.

To compile libdvi, you will need `meson` and `ninja` and a C99 compiler.
Refer to your distribution to install them.

### Compilation and installation

#### On UNIX (linux, BSD, Mac OSX, Solaris

Normally, if TeXLive is installed, the kpathsea library is also installed
and should be detected without any problem. Or you can also install just
the `kpathsea` library. Also update meson. For example, on Ubuntu :

```
sudo apt instal libkpathsea-dev texlive-binaries
sudo pip3 install meson==0.55.3
```

And to build `libdvi`:

```
meson setup builddir --prefix=/foo/bar
ninja -C builddir
```

#### On Windows, using MSYS2, 64 bits

Install TeXLive with `pacman -S mingw-w64-x86_64-texlive`. You can also
install just the kpathsea package with `pacman -S mingw-w64-x86_64-texlive-bin`.
The name of the DLL is (with latest TeXLive version) `libkpathsea-6.dll`.
So pass the `-Dkpathsea-name` meson option the name of the DLL without
the `lib` prefix and without the extension. Pass also the full path to
the binary directory where the DLL is located. For example :

```
meson setup builddir --prefix=/foo/bar -Dkpathsea-name=kpathsea-6 -Dkpathsea-path=/mingw64/bin
ninja -C builddir
```

If TeXLive is installed outside MSYS2, replace the name of kpathsea
if it is different from `kpathsea-6` (unlikely though) and its location
in the `meson` command above.

If MIKTeX is installed instead of TeXLive, replace the name of kpathsea
(usually something like `MIKTEX211200-kpathsea`) and its location in the
`meson` command above.
