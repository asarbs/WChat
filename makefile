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
TEST_DIR = ${PROJECT_DIR}/test/unit
TEST_BIN_DIR = ${PROJECT_DIR}/build/test

BUILD_DIR = ${PROJECT_DIR}/build
BUILD_TYPE ?= Debug

FIND_CMD = find ${SOURCES_DIR} \( -iname "*.h" -o -iname "*.cpp" -o -iname "*.cppm" \) -and ! -path "*/libs/*" -and ! -path "*ranslation.h"

# TARGET_NAME := app
# TARGET_ARGS := -a 111 --test_b 2 -c test_name -d -e
# prof: TARGET_NAME := prof_app

# GTEST_DIR=./build/linux/_deps/gtest-src/googletest
# CPPFLAGS_PROD :=
# prof: CPPFLAGS_PROD += -pg
# CPPFLAGS_PROD += -g
# # CPPFLAGS_PROD += -isystem
# CPPFLAGS_PROD += -Wall
# CPPFLAGS_PROD += -pedantic
# CPPFLAGS_PROD += -pthread
# CPPFLAGS_PROD += -lpthread
# CPPFLAGS_PROD += -std=c++20
# CPPFLAGS_PROD += -fpermissive
# CPPFLAGS_PROD += -ferror-limit=1
# CPPFLAGS_PROD += -fmodules
# CPPFLAGS_PROD += -fprebuilt-module-path=.
# CPPFLAGS_PROD += -v
# CPPFLAGS_PROD += -E

# CXXFLAGS := -g -Wall -Wextra -pthread
# CPPFLAGS_TEST := -g -Wall -Wextra -pthread -isystem $(GTEST_DIR)/include

# TEST_SOURCES_C :=
# TEST_SOURCES_CPP := test/unit/test_main.cpp
# TEST_SOURCES_CPP += src/serwer/messages/MessageHandler_RegisterClient.cpp
# TEST_SOURCES_CPP += src/serwer/messages/MessageManager.cpp
# TEST_SOURCES_CPP += src/serwer/messages/MessageHandler.cpp
# TEST_SOURCES_CPP += src/serwer/messages/MessageHandler_Message.cpp
# TEST_SOURCES_CPP += src/serwer/ChatClientDatabase.cpp
# TEST_SOURCES_CPP += src/serwer/ChatClient.cpp
# TEST_SOURCES_CPP += build/linux/_deps/cpp_logger-src/logger.cpp
# TEST_SOURCES_CPP += build/linux/_deps/cpp_arguments-src/arguments.cpp

# TEST_OBJS :=
# TEST_OBJS += $(TEST_SOURCES_CPP:%.cpp=%.o)
# TEST_OBJS += $(TEST_SOURCES_C:%.c=%.o)

# GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# INCLUDES := src
# INCLUDES += build/linux/_deps/gtest-src/googletest/include
# INCLUDES += build/linux/_deps/gtest-src/googlemock/include
# INCLUDES += build/linux/_deps/nlohmann_json-src/include
# INCLUDES += build/linux/_deps/websocketpp-src
# INCLUDES += build/linux/_deps/cpp_arguments-src
# INCLUDES += build/linux/_deps/cpp_logger-src
# INCLUDES_PARAMS=$(foreach d, $(INCLUDES), -I"${PWD}/$d")
# INCLUDES_PARAMS += -I"/usr/include/clang/20/"
# INCLUDES_PARAMS += -I"/usr/include/x86_64-linux-gnu/c++/11/"
# INCLUDES_PARAMS += -I"/usr/include/c++/11/"

# GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
#                 $(GTEST_DIR)/include/gtest/internal/*.h

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

# %.o : %.cpp
# 	@echo 'Build object file: $< -> $@'
# 	$(Q)$(GCC) $(CPPFLAGS_PROD) $(INCLUDES_PARAMS) -c -o "$@" "$<"

# gtest-all.o : $(GTEST_SRCS_)
# 	@echo 'Build file: $< -> $@'
# 	$(Q)$(GCC) $(CPPFLAGS_TEST) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

# gtest_main.o : $(GTEST_SRCS_)
# 	@echo 'Build file: $< -> $@'
# 	$(Q)$(GCC) $(CPPFLAGS_TEST) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

# test-unit-conf:
# 	cmake -S . -B build/linux
# 	$(Q)mkdir -p  ${TEST_BIN_DIR}

compile-test_unit:
	@echo 'Build file: test_main'
	${Q}cmake -S . -B $(BUILD_DIR)/linux -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
	${Q}cmake --build $(BUILD_DIR)/linux -j8 --target unit_tests

test-unit: compile-test_unit
	@echo 'Exe: test_main'
	${Q} ./build/linux/bin/unit_tests

test-app:
	cd test/app && pytest -v -s

test: test-unit test-app

compile_server:
	@echo 'Build executable file: $(TARGET_NAME)'
	${Q}cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -B $(BUILD_DIR)/linux
	${Q}cmake --build ${BUILD_DIR}/linux -j8 --target WChat_SERVER

run_server: compile_server
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
		--exclude=src/core/translation.h \
		--recursive $(SOURCES_DIR)

cppcheck:
	@echo 'cppcheckaa'
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

all: clang-reformat compile_server clang-check cpplint cppcheck
