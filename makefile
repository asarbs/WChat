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

GCC = clang++-18
FORMATTER = clang-format-18
CPPCHECK = cppcheck
CPPLINT = cpplint

PROJECT_DIR := ${PWD}
SOURCES_DIR := ${PROJECT_DIR}/src
TEST_DIR = ${PROJECT_DIR}/test_src

BUILD_DIR = ${PROJECT_DIR}/build
BUILD_TYPE ?= Debug


FIND_CMD = find ${SOURCES_DIR} \( -iname "*.h" -o -iname "*.cpp" -o -iname "*.cppm" \) -and ! -path "*/libs/*" -and ! -path "*ranslation.h"

TARGET_NAME := app
TARGET_ARGS := -a 111 --test_b 2 -c test_name -d -e
prof: TARGET_NAME := prof_app

GTEST_DIR=./libs/googletest/googletest
CPPFLAGS_PROD :=
prof: CPPFLAGS_PROD += -pg
CPPFLAGS_PROD += -g
# CPPFLAGS_PROD += -isystem
CPPFLAGS_PROD += -Wall
CPPFLAGS_PROD += -pedantic
# CPPFLAGS_PROD += -pthread
# CPPFLAGS_PROD += -lpthread
CPPFLAGS_PROD += -std=c++20
CPPFLAGS_PROD += -fpermissive
CPPFLAGS_PROD += -ferror-limit=1
CPPFLAGS_PROD += -fmodules
CPPFLAGS_PROD += -fprebuilt-module-path=.
# CPPFLAGS_PROD += -v
# CPPFLAGS_PROD += -E

CXXFLAGS := -g -Wall -Wextra -pthread
CPPFLAGS_TEST := -g -Wall -Wextra -pthread -isystem $(GTEST_DIR)/include

SOURCES_C :=

SOURCES_CPP :=
SOURCES_CPP += libs/logger/logger.cpp
SOURCES_CPP += arguments.cpp
SOURCES_CPP += main.cpp

OBJS :=
OBJS += $(SOURCES_C:%.c=%.o)
OBJS += $(SOURCES_CPP:%.cpp=%.o)

TEST_SOURCES_C :=
TEST_SOURCES_CPP := test_main.cpp
TEST_SOURCES_CPP += arguments.cpp
TEST_SOURCES_CPP += libs/logger/logger.cpp

TEST_OBJS :=
TEST_OBJS += $(TEST_SOURCES_CPP:%.cpp=%.o)
TEST_OBJS += $(TEST_SOURCES_C:%.c=%.o)

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

INCLUDES :=
INCLUDES += libs/logger/.

test: INCLUDES += libs/googletest/googletest/include/gtest/internal/
test: INCLUDES += libs/googletest/googletest/include/

INCLUDES += .
INCLUDES_PARAMS=$(foreach d, $(INCLUDES), -I"${PWD}/$d")

INCLUDES_PARAMS += -I"/usr/include/clang/18/"
INCLUDES_PARAMS += -I"/usr/include/x86_64-linux-gnu/c++/11/"
INCLUDES_PARAMS += -I"/usr/include/c++/11/"


GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

.setup-cpplint:
	$(Q)@echo 'setup-cpplint'
	${SUDO} apt install cpplint -y

.setup-cppcheck:
	$(Q)@echo 'setup-cppcheck'
	${SUDO} apt install cppcheck -y

.setup-cmake:
	$(Q)@echo 'setup-cmake'
	$(Q)wget https://github.com/Kitware/CMake/releases/download/v3.31.0-rc3/cmake-3.31.0-rc3.tar.gz
	$(Q)tar -xzf cmake-3.31.0-rc3.tar.gz
	$(Q)${SUDO} apt install build-essential checkinstall zlib1g-dev libssl-dev -y
	$(Q)cd cmake-3.31.0-rc3 && ./bootstrap && make -j12
	$(Q)cd cmake-3.31.0-rc3 && ${SUDO} make install
	$(Q)cmake --version
	$(Q)rm cmake-3.31.0-rc3.tar.*
	$(Q)rm -rf cmake-3.31.0-rc3


.setup-clang:
	@echo 'setup-clang'
	$(Q) wget https://apt.llvm.org/llvm.sh
	$(Q) chmod 777 llvm.sh
	$(Q) ${SUDO} ./llvm.sh
	$(Q) rm llvm.sh*

.setup-clang-format: .setup-clang
	${SUDO} apt-get install clang-format-18 -y

.setup-mingw:
	${SUDO} apt install mingw-w64 libx11-dev libxrandr-dev -y

.setup-boost:
	${SUDO} apt install libboost-all-dev libssl-dev -y

.setup: .setup-clang-format .setup-cpplint .setup-cppcheck .setup-boost .setup-cmake
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

gtest-all.o : $(GTEST_SRCS_)
	@echo 'Build file: $< -> $@'
	$(Q)$(GCC) $(CPPFLAGS_TEST) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	@echo 'Build file: $< -> $@'
	$(Q)$(GCC) $(CPPFLAGS_TEST) -I$(GTEST_DIR) $(CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc

test: $(TEST_OBJS) gtest-all.o gtest_main.o
	@echo 'Build file: test_main'
	$(Q)$(GCC) $(CPPFLAGS_PROD) $(INCLUDES_PARAMS) $^ -o test_exe
	./test_exe --gtest_catch_exceptions=0

compile:
	@echo 'Build executable file: $(TARGET_NAME)'
	${Q}cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -B $(BUILD_DIR)/linux
	${Q}cmake --build ${BUILD_DIR}/linux -j8

run: compile
	${Q} ./build/linux/bin/VoidInvaders --level assets/testLevel.yaml

run-full: compile
	${Q} ./build/linux/bin/VoidInvaders --level assets/test.yaml

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
	@echo 'cppcheck'
	$(Q)$(CPPCHECK) --version
	$(Q)$(CPPCHECK) \
		--platform=native \
		--language=c++ \
		--std=c++23 \
		--suppressions-list=tools/cppcheck.suppress \
		--enable=all \
		--inline-suppr \
		--error-exitcode=1 \
		--suppress=unmatchedSuppression \
		--suppress=unusedFunction \
		-I ${SOURCES_DIR} \
		${SOURCES_DIR}

all: clang-reformat clang-check cpplint cppcheck compile
