# Copyright IBM Corp. and others 2023
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] https://openjdk.org/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0

# Create the OMR build environment with Debian 11.7.

FROM debian:11.7-slim AS base

# Workaround for a hang during docker build.
ENV TZ=America/Toronto
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get install -y \
	binfmt-support \
        bison \
	build-essential \
        ccache \
        cmake \
        flex \
	gcc-riscv64-linux-gnu \
        gdb \
        git \
	g++-riscv64-linux-gnu \
        libelf-dev \
        libdwarf-dev \
	libglib2.0-dev \
	lsb-release \
	mmdebstrap \
        ninja-build \
	pkg-config \
        python3 \
	qemu-user-static \
	time \
	tzdata \
        vim \
	wget \
        && rm -rf /var/lib/apt/lists/*

# Create jenkins user and group to match that of OMR host systems
RUN groupadd -g 1001 jenkins \
&& useradd -rm -u 1001 -g jenkins jenkins

RUN wget -O /usr/local/include/riscv.h 'https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob_plain;f=include/opcode/riscv.h;hb=2f973f134d7752cbc662ec65da8ad8bbe4c6fb8f' \
&& wget -O /usr/local/include/riscv-opc.h 'https://sourceware.org/git/gitweb.cgi?p=binutils-gdb.git;a=blob_plain;f=include/opcode/riscv-opc.h;hb=2f973f134d7752cbc662ec65da8ad8bbe4c6fb8f'

# Install Debian ports GPG keys
RUN gpg --homedir /tmp \
	--no-default-keyring --primary-keyring "/tmp/debian-ports-keyring.gpg" \
        --keyserver keyserver.ubuntu.com --recv-key B523E5F3FC4E5F2C
RUN gpg --homedir /tmp \
	--no-default-keyring --primary-keyring "/tmp/debian-ports-keyring.gpg" \
        --keyserver keyserver.ubuntu.com --recv-key 8D69674688B6CB36
RUN gpg --homedir /tmp \
	--no-default-keyring --primary-keyring "/tmp/debian-ports-keyring.gpg" \
        --export 8D69674688B6CB36 > /etc/apt/trusted.gpg.d/debian-ports-archive-2023.gpg
RUN gpg --homedir /tmp \
	--no-default-keyring --primary-keyring "/tmp/debian-ports-keyring.gpg" \
        --export 8D69674688B6CB36 > /etc/apt/trusted.gpg.d/debian-ports-archive-2023.gpg

# Install RISC-V 64 build root in /opt/sysroot
RUN mmdebstrap \
	--mode=fakeroot \
	--variant=extract \
	--architectures=riscv64 \
	--include="debian-ports-archive-keyring,build-essential,libdwarf-dev,libelf-dev,libx11-dev,libxext-dev,libxrender-dev,libxrandr-dev,libxtst-dev,libxt-dev,libasound2-dev,libcups2-dev,libfontconfig1-dev" \
	unstable "/opt/sysroot" \
	"deb http://deb.debian.org/debian unstable main contrib"

ENV CROSS_SYSROOT_RISCV64=/opt/sysroot
USER jenkins
