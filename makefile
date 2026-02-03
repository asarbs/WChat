# World VTT
#
# Copyright (C) 2025, Asar Miniatures
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

PYTHON = python3.13

PROJECT_DIR := ${PWD}
SOURCES_DIR := ${PROJECT_DIR}/src
PROTO_DIR 	:= ${SOURCES_DIR}/server/proto

sync-github:
	git remote remove github 2>/dev/null || true
	git remote add github git@github.com:asarbs/WChat.git
	if [ -f .git/shallow ]; then git fetch --unshallow; fi
	git push --force github master --tags

compile-proto-cpp:
	@echo 'Compile proto-cpp'
	${Q}protoc -I=${PROTO_DIR} --cpp_out=${PROTO_DIR} ${PROTO_DIR}/messages.proto
	${Q}mkdir -p src/client/cpp/proto
	${Q}cp -r src/server/proto/* src/client/cpp/proto

compile-proto-python:
	@echo 'Compile proto-python'
	${Q}protoc -I=${PROTO_DIR} --python_out=test/app ${PROTO_DIR}/messages.proto

compile-proto: compile-proto-cpp compile-proto-python

run-server:
	${Q} ./build/linux/bin/WChat_SERVER

run-client01:
	${Q} ./build/linux/bin/WChat_CLIENT -c WChatClient01.conf

run-client02:
	${Q} ./build/linux/bin/WChat_CLIENT -c WChatClient02.conf

run-pyclinet:
	${Q} ${PYTHON} src/client/python/src/main.py

run-client_web:
	@echo "Starting simple HTTP server at http://localhost:8080"
	@cd src/client/html && python3 -m http.server 8080
