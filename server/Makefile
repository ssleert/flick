include .env

PWD	= $(shell pwd)
DATA_DIR = $(PWD)/data
POSTGRES_DATA_DIR = $(DATA_DIR)/postgres
MIGRATIONS_DIR = $(PWD)/migrations

MIGRATE_CONTAINER	 = migrate/migrate:v4.17.0
POSTGRES_CONTAINER = postgres:16.2-alpine3.19
POSTGRES_URL	= postgres://$(POSTGRES_USER):$(POSTGRES_PASSWORD)@127.0.0.1:5432/postgres?sslmode=disable

CC ?= clang
CXX ?= clang++

CMAKE_FLAGS = -GNinja \
	   -DSKIP_BUILD_TEST=on -DBUILD_DOC=off \
		 -DCMAKE_CXX_COMPILER_WORKS=1 -DCMAKE_C_COMPILER_WORKS=1

all: container-build-debug

# ===== POSTGRESQL MIGRATIONS =====
up-local-postgres:
	@if [ ! -d $(POSTGRES_DATA_DIR) ]; then \
		mkdir -p $(POSTGRES_DATA_DIR); \
	fi

	docker run \
		-d \
		--rm \
		--name postgres-local-dev \
		-v "$(POSTGRES_DATA_DIR)":"/var/lib/postgresql/data":Z \
		-p 5432:5432 \
		-e POSTGRES_PASSWORD=$(POSTGRES_PASSWORD) \
		-e POSTGRES_USER=$(POSTGRES_USER) \
		$(POSTGRES_CONTAINER)
	@sleep 2

down-local-postgres: 
	docker stop postgres-local-dev

# we cant make migrations without postgres container
# so we dont need make container task
make-migrations: up-local-postgres
	@docker run \
		--rm \
		--name migrate-util-dev \
		--network host \
		-v "$(MIGRATIONS_DIR)":"/mnt":Z \
		-it \
		$(MIGRATE_CONTAINER) \
		-path /mnt -database "$(POSTGRES_URL)" up; \
	exit 0;


migrations: make-migrations down-local-postgres

# ===== CPP BUILD ======

build-release: build/release
	@cd ./build/release; \
    ninja; \
	  cp ./flick ../../;

build/release:
	mkdir -p build/release
	@cd ./build/release; \
	  cmake -DCMAKE_BUILD_TYPE=Release \
	    $(CMAKE_FLAGS) ../../;

build-debug: build/debug
	@cd ./build/debug; \
    ninja; \
	  cp ./flick ../../;

build/debug:
	mkdir -p build/debug
	@cd ./build/debug; \
	 cmake -DCMAKE_BUILD_TYPE=Debug \
	   $(CMAKE_FLAGS) ../../;


container-build-debug:
	podman run -it --rm -v ./:/app:Z flick-dev-container make build-debug

container-build-release:
	podman run -it --rm -v ./:/app:Z flick-dev-container make build-release

build-container:
	podman build -f ./containers/dev.Dockerfile -t flick-dev-container .

clean:
	rm -rf ./build
	rm -rf ./flick
