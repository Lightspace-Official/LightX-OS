# MichalOS

A 16-bit keyboard controlled operating system based on MikeOS 4.5, aimed to be more advanced and lightweight on the inside, but simple and easy to use on the outside.

## New features

- Screensaver with customizable timeout
- Customizable user interface (custom background, window colors etc.)
- Custom font
- On-screen clock with timezone support
- AdLib synthesizer support
- Built-in graphics drawing functions

## System requirements

- Intel 80386 or higher, Pentium recommended
- At least 80 kB RAM, 256 kB recommended
- An EGA video card, VGA recommended
- A keyboard

## Screenshots

![Login screen](https://user-images.githubusercontent.com/41787099/128972820-d9d31c96-0e88-4fcb-b216-772a4f0d1568.png)
![Desktop](https://user-images.githubusercontent.com/41787099/128972823-3aae09a0-7684-4dc7-a195-92c6e2fb33d7.png)

More screenshots are available in the [gallery](https://github.com/prochazkaml/MichalOS/blob/master/misc/gallery.md).

## Building instructions

For building the OS, a Unix-based system is required (Linux, BSD, WSL, macOS) with **NASM**, **mtools** and **make** installed. **DOSBox** is required for testing MichalOS builds (QEMU, [VirtualBox](https://github.com/prochazkaml/MichalOS/blob/master/misc/VirtualBox.md), VMware etc. could also be used, but you will be met with limited functionality). **mkisofs** is needed only if you want to generate an ISO image for CDs by running ```make iso```.

On Debian GNU/Linux (and its derivates, such as Linux Mint or Ubuntu), these requirements can be met by running ```sudo apt-get install nasm mtools make dosbox```.

On macOS, first install [Homebrew](https://brew.sh/) and then run ```brew install nasm mtools dosbox``` from the Terminal.

It is also necessary to compile a ZX7 data compressor with ```cd misc/zx7 && ./build.sh```, which requires gcc/clang.

Then, you can simply build the image by running ```make```.
