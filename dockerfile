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


ADD ./src /app/src
ADD ./test /app/test
ADD ./CMakeLists.txt /app/CMakeLists.txt
ADD ./run.sh /app/run.sh

WORKDIR /app

RUN cmake CMakeLists.txt
RUN cmake --build . --target server
RUN cmake --build . --target client
RUN cmake --build . --target service

CMD ./run.sh