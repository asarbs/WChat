FROM debian:trixie

USER root
ENV DEBIAN_FRONTEND=noninteractive

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

COPY .devcontainer/library-scripts/*.sh /tmp/library-scripts/

# ---- SYSTEM ----
RUN apt-get update \
 && apt-get upgrade -y \
 && bash /tmp/library-scripts/common-debian.sh \
 && apt-get install -y \
    bash \
    bash-completion \
    python3 \
    python3-pip \
    protobuf-compiler \
    git \
    openssh-client \
    ca-certificates \
    wget \
    lsb-release \
    gdb \
    gnupg \
    make \
    docker.io \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    rclone \
 && rm -rf /var/lib/apt/lists/*

# ---- PIP CONFIG (żeby nie marudził w Debianie) ----
ENV PIP_BREAK_SYSTEM_PACKAGES=1
ENV PIP_NO_CACHE_DIR=1

# ---- PYTHON DEPS ----
WORKDIR /app

COPY src/client/python/requirements.txt ./requirements.txt

RUN pip install --upgrade setuptools \
 && pip install pytest

# ---- SHELL / USER ----
SHELL ["/bin/bash", "-c"]

EXPOSE 3000
EXPOSE 9002

USER ${USERNAME}

ENTRYPOINT ["/usr/local/share/docker-init.sh"]
CMD ["sleep", "infinity"]
