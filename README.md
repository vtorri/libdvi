[![CI meson](https://github.com/vtorri/libdvi/actions/workflows/ci_meson.yml/badge.svg)](https://github.com/vtorri/libdvi/actions?query=workflow%3A%22GitHub+CI%22)

### libdvi

#### DVI rendering library written in C99

### License

This application is released under the LGPL 2.1 or later

### Requirements

The kpathsea library (provided by TeXLive or MIKTeX) is required.
Preferably install TeXLive


### Compilation and installation

#### On UNIX (linux, BSD, Mac OSX, Solaris

```
meson buildir --prefix=/foo/bar
cd builddir
ninja install
```

#### On Windows, using MSYS2, 64 bits

Install TeXLive with pacman, then

```
meson buildir --prefix=/foo/bar -Dkpathsea-name=kpathsea-6 -Dkpathsea-path=/mingw64/bin
cd builddir
ninja install
```

If MIKTeX is installed instead of TeXLive, replace the name of kpathsea
(usually something like MIKTEX211200-kpathsea) and its location in the
meson command above.
