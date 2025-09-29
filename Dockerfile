FROM gcc:10.2

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
      make \
      cmake \
      cppcheck \
      git \
      && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

CMD ["/bin/bash"]