FROM ubuntu:latest AS build

RUN apt-get update && apt-get install -y build-essential

WORKDIR /build

COPY cube.cpp .

RUN g++ -o out cube.cpp

CMD ["./out"]