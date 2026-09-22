FROM ubuntu:24.04

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

# Download Linux support tools
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    sudo \
    bzip2 \
    build-essential \
    ca-certificates \
    wget \
    cmake \
    git \
    ninja-build && \
    rm -rf /var/lib/apt/lists/*

# Using toolchain version
ARG ARM_TOOLCHAIN_VERSION="10.3-2021.10"
ARG ARM_TOOLCHAIN_DIR="/opt/gcc-arm-none-eabi"

# Download toolchain from official site
# Extract all files to ARM_TOOLCHAIN_DIR
RUN mkdir ${ARM_TOOLCHAIN_DIR} && \
    wget -qO- "https://developer.arm.com/-/media/Files/downloads/gnu-rm/${ARM_TOOLCHAIN_VERSION}/gcc-arm-none-eabi-${ARM_TOOLCHAIN_VERSION}-x86_64-linux.tar.bz2" | tar -xvj -C ${ARM_TOOLCHAIN_DIR} --strip-components=1

# Append binary folder to PATH env
ENV PATH=$PATH:${ARM_TOOLCHAIN_DIR}/bin

# Setup for user
ARG USERNAME=ubuntu

# Configurate user (add to sudoers, add to plugdev group)
RUN echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME \
    && usermod -a -G plugdev $USERNAME

# Switch to new user (not root)
USER $USERNAME
