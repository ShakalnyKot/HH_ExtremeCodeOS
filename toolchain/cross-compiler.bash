#!/bin/bash

export TARGET=$1
export PREFIX=$2
export PATH="$PREFIX/bin:$PATH"

install_deps_sudo() {
	if [[ -f "/usr/bin/apt" ]]; then
		sudo apt update
		sudo apt install gcc g++ wget build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo -y
	elif [[ -f "/usr/bin/pacman" ]]; then
		sudo pacman -Sy
		sudo pacman -S gcc g++ wget base-devel gmp libmpc mpfr
	else
		printf "Seems like your distribution's package manager not supported yet for autoinstallation.\nYou should install cross-compiler by guide in https://wiki.osdev.org/GCC_Cross-Compiler instead\n"
		sleep 2
	fi
}

install_deps_asroot() {
	if [[ -f "/usr/bin/apt" ]]; then
		apt update
		apt install gcc g++ wget build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo -y
	elif [[ -f "/usr/bin/pacman" ]]; then
		pacman -Sy
		pacman -S gcc g++ wget base-devel gmp libmpc mpfr
	else
		printf "Seems like your distribution's package manager not supported yet for autoinstallation.\nYou should install cross-compiler by guide in https://wiki.osdev.org/GCC_Cross-Compiler instead\n"
		sleep 2
	fi
}

install_deps() {
	if [[ -f "/usr/bin/sudo" ]]; then
		install_deps_sudo
	else
		install_deps_asroot
	fi
}

prepare_sources() {
	echo Downloading sources...
	wget https://ftp.gnu.org/gnu/binutils/binutils-$1.tar.xz
	wget https://ftp.gnu.org/gnu/gcc/gcc-$2/gcc-$2.tar.xz
	echo Unpacking sources...
	tar -xf binutils-$1.tar.xz
	tar -xf gcc-$2.tar.xz
}

compile_sources() {
	echo Compiling Binutils...
	mkdir binutils-compile
	cd binutils-compile
	../binutils-$1/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
	make -j$(nproc)
	make install -j$(nproc)
	cd ..
	
	echo Compiling GCC...
	which -- $TARGET-as || echo $TARGET-as is not in the PATH
	mkdir gcc-compile
	cd gcc-compile
	../gcc-$2/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
	make all-gcc -j$(nproc)
	make all-target-libgcc -j$(nproc)
	make install-gcc -j$(nproc) 
	make install-target-libgcc -j$(nproc)
	cd ..
}

# echo $TARGET $PREFIX

install_deps

mkdir temp_dir
cd temp_dir
prepare_sources 2.39 12.2.0
echo prefix:$PREFIX, target:$TARGET
compile_sources 2.39 12.2.0

# cleanup
echo cleaning up...
cd ..
rm -rf temp_dir

echo cross-compiler installed to $PREFIX! here are --version of binutils and gcc:
$TARGET-as --version
$TARGET-gcc --version