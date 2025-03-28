# Copyright IBM Corp. and others 2022
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

# Create the OMR build environment with Ubuntu 20.04.

FROM nvcr.io/nvidia/cuda:12.2.0-devel-ubi8 as cuda

FROM public.ecr.aws/lts/ubuntu:22.04_stable AS base

# Workaround for a hang during docker build.
ENV TZ=America/Toronto
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get install -y \
        tzdata \
        python3 \
        git \
        cmake \
        bison \
        flex \
        libelf-dev \
        libdwarf-dev \
        gdb \
        vim \
        gcc-11-multilib \
        g++-11-multilib \
        gcc-11 \
        g++-11 \
        ninja-build \
        ccache \
        && rm -rf /var/lib/apt/lists/*

# Add user home and copy authorized_keys and known_hosts.
RUN groupadd -r jenkins \
&& useradd -rm -u 1000 -g jenkins jenkins \
&& mkdir -p /home/jenkins/.ssh \
&& echo "Host git.eclipse.org*\n\tStrictHostKeyChecking no\n" > /home/jenkins/.ssh/config \
&& chown -R jenkins:jenkins /home/jenkins

# Copy header files necessary to build with CUDA support.
RUN mkdir -p /usr/local/cuda/nvvm
COPY --from=cuda /usr/local/cuda/include /usr/local/cuda/include
COPY --from=cuda /usr/local/cuda/nvvm/include /usr/local/cuda/nvvm/include

USER jenkins
ENV CUDA_HOME=/usr/local/cuda

ENV CC=gcc-11 CXX=g++-11
