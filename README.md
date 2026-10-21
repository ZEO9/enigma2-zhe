# ZHE — Enigma2 Graphics Development for OpenBH

ZHE is a community-driven Enigma2 development effort focused on experimenting with GPU-accelerated on-screen display (OSD) rendering and higher-resolution UI support on compatible Vu+ / Broadcom-based receivers.

This repository builds on **OpenBH**, **OE-Alliance**, and graphics work from the **OpenATV Enigma2 EGL/GLES branch**. It is not an independently written replacement for those projects. ZHE-specific integration and platform work is tracked in the repository history.

## Development highlights

- **EGL/GLES integration:** Porting an Enigma2 GPU rendering backend into the OpenBH codebase, including renderer, shaders, textures and font-atlas components.
- **Broadcom platform integration:** Developing a native-window provider for the Vu+ Broadcom platform using vendor VUGLES/NXPL interfaces.
- **Configurable OSD target:** Work toward a 2560 × 1440 OSD rendering path, with configuration through `/etc/enigma2/zhe-osd.conf`. Actual output depends on device capabilities and runtime support.
- **Compatibility and fallback:** GLES2/GLES3 linking fixes and a software framebuffer fallback where the accelerated backend cannot initialize.

These points describe code and development history, **not a guarantee that every feature works on every device**.

## Project status

The maintainer reports a working build on a Vu+ receiver. Public, reproducible test results, supported-model coverage, performance measurements, and validation of the 2560 × 1440 output path have not yet been documented here.

**Before deploying:** This is experimental receiver software. Keep a recovery image and backups, and confirm compatibility with your exact hardware.

## Selected development history

- [OpenBH EGL/GLES port and initial Broadcom provider](https://github.com/ZEO9/enigma2-zhe/commit/143e9933b5ac46714207737288e5c469ec91cb14)
- [Broadcom VUGLES native-window integration](https://github.com/ZEO9/enigma2-zhe/commit/4c1f0a854335d79ebe5781edd5d3e2eeb25dc775)
- [GLES2 compatibility fix](https://github.com/ZEO9/enigma2-zhe/commit/a91ab37383edd0573d7149f39fbcceaf5d7f6938)

## Upstream projects and attribution

ZHE incorporates or adapts work from [OpenBH / BlackHole Enigma2](https://github.com/BlackHole/enigma2), [OE-Alliance](https://github.com/oe-alliance/oe-alliance-core), and the OpenATV Enigma2 EGL/GLES implementation. Credit and license notices in upstream files must be retained. The repository contains a GNU GPL v2 license file; individual imported components may carry their own notices.

## Contributing and feedback

Bug reports and reproducible test findings are welcome through GitHub Issues. When reporting an issue, include the receiver model, firmware/build details, relevant logs, and steps to reproduce. Do not share credentials, private keys, or private network information.

---

## OpenBH build environment and instructions (preserved from the previous README)

## OpenBh buildserver requirements: ##

>Ubuntu 24.04 LTS (GNU/Linux 6.8.0-39-generic x86_64) or Ubuntu 26.04 LTS (GNU/Linux 7.0.0-14-generic x86_64)

## minimum hardware requirement for image build (building feeds may require more):

> RAM:  16GB
> SWAP: 16GB (if building feeds then RAM+SWAP should be larger)>
> CPU:  Multi core\thread Model
> HDD:  for Single Build 250GB Free, for Multibuild 500GB or more

## OpenBh python3 is built using oe-alliance build-environment and several git repositories: ##

> [https://github.com/oe-alliance/oe-alliance-core/tree/5.6](https://github.com/oe-alliance/oe-alliance-core/tree/5.6 "OE-Alliance")
>
> https://github.com/BlackHole/enigma2

----------

# Building Instructions #

1 - Install packages on your buildserver

    sudo apt-get install -y autoconf automake bison bzip2 chrpath cmake coreutils cpio curl cvs debianutils default-jre default-jre-headless diffstat flex g++ gawk gcc gcc-12 gcc-multilib g++-multilib gettext git gzip help2man info iputils-ping java-common libc6-dev libc6-dev-i386 libglib2.0-dev libncurses-dev libperl4-corelibs-perl libproc-processtable-perl libsdl1.2-dev libserf-dev libtool libxml2-utils make ncurses-bin patch perl pkg-config psmisc python3 python3-git python3-html5lib python3-jinja2 python3-pexpect python3-pip python3-setuptools quilt socat sshpass subversion tar texi2html texinfo unzip wget xsltproc xterm xz-utils zip zlib1g-dev zstd fakeroot lz4 git-lfs

----------
2 - Set python3 as preferred provider for python

	sudo update-alternatives --install /usr/bin/python python /usr/bin/python2 1
	sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 2
	sudo update-alternatives --config python
	select python3

----------
3 - Set your shell to /bin/bash.

    sudo ln -sf /bin/bash /bin/sh

----------
4 - modify max_user_watches

	echo fs.inotify.max_user_watches=524288 | sudo tee -a /etc/sysctl.conf

	sudo sysctl -n -w fs.inotify.max_user_watches=524288

----------
5 - Update apparmor profile

## Workaround for ubuntu issue [Allow bitbake to create user namespace](https://bugs.launchpad.net/ubuntu/+source/apparmor/+bug/2056555) ##
## Credit to Changqing Li (sandy-lcq), Karsten S. Opdal (karsten-s-opdal) and Ferry Toth (ftoth) ##

    echo 'kernel.apparmor_restrict_unprivileged_userns=0' | sudo tee /etc/sysctl.d/60-apparmor-namespace.conf > /dev/null && sudo sysctl --system
    sudo sysctl -w kernel.apparmor_restrict_unprivileged_userns=0

----------
6 - Add user openbhbuilder

----------
7 - Add your git user and email

    git config --global user.email "you@example.com"

    git config --global user.name "Your Name"

----------
8 - Switch to user openbhbuilder

----------
9 - Switch to home of openbhbuilder

	cd ~

----------
10 - Create folder openbh

	mkdir -p ~/openbh

----------
11 - Switch to folder openbh

	cd openbh

----------
12 - Clone oe-alliance git

    git clone https://github.com/oe-alliance/build-enviroment.git -b 5.6

----------
13 - Switch to folder build-enviroment

	cd build-enviroment

----------
14 - Update build-enviroment

	make update

----------
15 - Initialise the first machine so site.conf gets created

    MACHINE=vuultimo4k DISTRO=openbh DISTRO_TYPE=release make  init

----------
16 - Update site.conf

    - BB_NUMBER_THREADS, PARALLEL_MAKE set to number of threads supported by the CPU
    - add/modify DL_DIR = " location for build sources " to point to a location where you can save
      derived build sources, this will reduce build time in fetching these sources again.
    - Avoid wasting disk space creating spdx packages: INHERIT:remove = "create-spdx"

----------
17 - Building image with feeds  e.g.:-

	MACHINE=vuultimo4k DISTRO=openbh DISTRO_TYPE=release make image

----------
18 - Building an image without feeds (Build time 1-2h)

	MACHINE=vuultimo4k DISTRO=openbh DISTRO_TYPE=release make enigma2-image

----------
19 - Building feeds only

	MACHINE=vuultimo4k DISTRO=openbh DISTRO_TYPE=release make feeds
