CC ?= clang
CXX ?= clang++

all: container-build-debug

build-release: prepare
	@cd ./build; \
	 cmake -DCMAKE_BUILD_TYPE=Release -GNinja ../; \
   ninja; \
	 cp ./flick ../;

build-debug: prepare
	@cd ./build; \
	 cmake -DCMAKE_BUILD_TYPE=Debug -GNinja ../; \
   ninja; \
	 cp ./flick ../;

container-build-debug:
	podman run -it --rm -v ./:/app:Z flick-dev-container make build-debug

container-build-release:
	podman run -it --rm -v ./:/app:Z flick-dev-container make build-release

build-container:
	podman build -f ./containers/dev.Dockerfile -t flick-dev-container .

prepare:
	mkdir -p ./build

clean:
	rm -rf ./build/
	rm -rf ./flick
