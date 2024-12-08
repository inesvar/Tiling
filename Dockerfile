FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
cmake build-essential libgl1-mesa-dev libglfw3-dev libglm-dev

WORKDIR /Tiling

COPY src/ ./src
COPY CMakeLists.txt .
COPY release/main .

RUN mkdir release && cd release && \
    cmake .. -D CMAKE_BUILD_TYPE=Release && \
    cmake --build .

# docker build -t Tiling .
# docker run -it Tiling