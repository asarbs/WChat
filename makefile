# World VTT
#
# Copyright (C) 2024, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.


PWD ?= $(shell pwd)

ifeq ($(V),1)
Q :=
else
Q ?= @
endif

SUDO ?= sudo

GCC = clang++-20
FORMATTER = clang-format-19
CPPCHECK = cppcheck
CPPLINT = cpplint

PROJECT_DIR := ${PWD}
SOURCES_DIR := ${PROJECT_DIR}/src
PROTO_DIR 	:= ${SOURCES_DIR}/serwer/proto
TEST_DIR = ${PROJECT_DIR}/test/unit
TEST_BIN_DIR = ${PROJECT_DIR}/build/test

BUILD_DIR = ${PROJECT_DIR}/build
BUILD_TYPE ?= Debug

FIND_CMD = find ${SOURCES_DIR} \( -iname "*.h" -o -iname "*.cpp" -o -iname "*.cppm" \) -and ! -path "*/libs/*" -and ! -path "*/proto/*"

.setup-cpplint:
	$(Q)@echo 'setup-cpplint'
	${SUDO} apt install cpplint -y

.setup-cppcheck:
	$(Q)@echo 'setup-cppcheck'
	${SUDO} apt install cppcheck -y

.setup-cmake:
	$(Q)${SUDO} apt-get update
	$(Q)${SUDO} apt-get install -y cmake
	$(Q)cmake --version

.setup-clang:
	@echo 'setup-clang'
	$(Q) wget https://apt.llvm.org/llvm.sh
	$(Q) chmod 777 llvm.sh
	$(Q) ${SUDO} ./llvm.sh
	$(Q) rm llvm.sh*
	$(Q) ${SUDO} apt-get update
	$(Q) ${SUDO} apt-get install -y clang-18 clang-tools-18 clang-format-18 lldb-18 lld-18

.setup-clang-format: .setup-clang
	${SUDO} apt-get install clang-format-19 -y

.setup-mingw:
	${SUDO} apt install mingw-w64 libx11-dev libxrandr-dev -y

.setup-boost:
	${SUDO} apt install libboost-all-dev libssl-dev -y

.setup-test-app:
	${SUDO} pip3 install websockets --break-system-packages
	${SUDO} pip3 install anyio --break-system-packages
	${SUDO} pip3 install asyncio --break-system-packages
	${SUDO} pip3 install pytest-tornasync --break-system-packages
	${SUDO} pip3 install pytest_asyncio --break-system-packages
	${SUDO} pip3 install trio --break-system-packages
	${SUDO} pip3 install twisted --break-system-packages
	${SUDO} pip3 install protobuf --break-system-packages

.setup-test: .setup-test-app

.setup: .setup-clang-format .setup-cpplint .setup-cppcheck .setup-boost .setup-cmake .setup-test
	${SUDO} apt-get install libxinerama-dev libopengl-dev libxxf86vm-dev libxi-dev libeigen3-dev libglfw3-dev -y

clean:
	@echo 'Clean'
	${Q}rm -rf $(BUILD_DIR)
	${Q}rm -rf test_exe
	${Q}rm -rf prof_app
	${Q}rm -rf $(TARGET_NAME)
	${Q}rm -rf gmon.out
	${Q}find . -name "*.o" | xargs -r rm
	${Q}find . -name "*.pcm" | xargs -r rm
	${Q}find . -name "*.pb.h" | xargs -r rm
	${Q}find . -name "*.pb.cc" | xargs -r rm

compile-test_unit: compile-proto-cpp
	@echo 'Build file: test_main'
	${Q}cmake -S . -B $(BUILD_DIR)/linux -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	${Q}cmake --build $(BUILD_DIR)/linux -j8 --target unit_tests

test-unit: compile-test_unit
	@echo 'Exe: test_main'
	${Q} ./build/linux/bin/unit_tests --gtest_output=xml:build/test-unit-results.xml

test-app: compile-proto-python
	cd test/app && pytest -v -s --junitxml=../../build/test-app-results.xml

test-app-filter: compile-proto-python
	cd test/app && pytest -v -s -k "$(K)"

test: test-unit test-app

compile-proto-cpp:
	@echo 'Compile proto-cpp: $(TARGET_NAME)'
	protoc -I=${PROTO_DIR} --cpp_out=${PROTO_DIR} ${PROTO_DIR}/messeges.proto

compile-proto-python:
	@echo 'Compile proto-cpp: $(TARGET_NAME)'
	protoc -I=${PROTO_DIR} --python_out=test/app ${PROTO_DIR}/messeges.proto

compile-proto: compile-proto-cpp compile-proto-python

compile-server: compile-proto-cpp
	@echo 'Build executable file: $(TARGET_NAME)'
	${Q}cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -B $(BUILD_DIR)/linux
	${Q}cmake --build ${BUILD_DIR}/linux -j8 --target WChat_SERVER

run_server: compile-server
	${Q} ./build/linux/bin/WChat_SERVER

run_client:
	@echo "Starting simple HTTP server at http://localhost:8080"
	@cd src/client/html && python3 -m http.server 8080

clang-check:
	${Q}$(FORMATTER) --version
	${Q}$(FIND_CMD) | xargs $(FORMATTER) --Werror --dry-run --verbose

clang-reformat:
	${Q} $(FORMATTER) --version
	${Q} $(FIND_CMD) | xargs $(FORMATTER) --Werror -i --verbose

cpplint:
	@echo 'cpplint'
	$(Q)${CPPLINT} --version
	$(Q)${CPPLINT} \
		--exclude=${SOURCES_DIR}/serwer/proto \
		--recursive $(SOURCES_DIR)

cppcheck: clean
	@echo 'cppcheck'
	$(Q)$(CPPCHECK) --version
	$(Q)$(CPPCHECK) \
		--platform=native \
		--language=c++ \
		--std=c++23 \
		--suppressions-list=tools/cppcheck.suppress \
		--library=std \
		--enable=all \
		--inline-suppr \
		--force \
		--error-exitcode=1 \
		-I ${SOURCES_DIR} \
		${SOURCES_DIR}

all: clean clang-check cppcheck cpplint cpplint compile-server test-unit test-app
