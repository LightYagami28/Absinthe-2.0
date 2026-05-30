# Building Absinthe 2.0

## Prerequisites

- GCC (32-bit capable: `gcc-multilib` on Debian/Ubuntu)
- autoconf >= 2.59, automake, aclocal
- libbz2-dev (bzip2 development headers)
- Static libraries in `staticlibs/<platform>/`: libimobiledevice, libplist,
  libusbmuxd, libbz2, libssl, libcrypto, libxml2

## Quick Build (Linux)

```sh
./autogen.sh
./configure
make
```

## Debug Build

```sh
./configure --enable-debug
make
```

## macOS

```sh
./autogen.sh
./configure
make
```

## Windows (MinGW32)

```sh
windres resources/win32/res.rc -O coff gui/win32res.o
./autogen.sh
./configure
make
```

## Running Tests

```sh
make check
```

## Packaging

```sh
./build.sh
```

Output appears in `build/absinthe/`.
