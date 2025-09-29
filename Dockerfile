FROM debian:stable

ENV DEBIAN_FRONTEND=noninteractive

# Instalacja GCC, G++, make, cmake, cppcheck, git
RUN apt-get update && apt-get install -y --no-install-recommends \
      gcc \
      g++ \
      make \
      cmake \
      cppcheck \
      git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

CMD ["/bin/bash"]