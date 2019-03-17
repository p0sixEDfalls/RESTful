FROM gcc:latest as build

WORKDIR /gtest_build

RUN apt-get update && \
    apt-get install -y \
    libgtest-dev \
    cmake \
    && \
    cmake -DCMAKE_BUILD_TYPE=Release /usr/src/gtest && \
    cmake --build . && \
    mv lib*.a /usr/lib && \
    apt-get install -y libevent-dev && apt install rapidjson-dev


ADD . /app/src

WORKDIR /app/src

WORKDIR /app/src/build

ENTRYPOINT ["./server"]