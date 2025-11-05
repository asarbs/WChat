# World VTT
#
# Copyright (C) 2025, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.

import pytest_asyncio
import asyncio
import websockets
import subprocess
import time
import os
import signal
import sys
from constants import *

CONFIGURATION = """
# Host of server
host=localhost
# Port number
port=9002
# Storage of serwer data
Storage=volatile
"""



@pytest_asyncio.fixture(autouse=True)
def run_server():
    proc = subprocess.Popen(PROGRAM_CMD, stdout=sys.stdout, stderr=sys.stderr)

    with open("WChatServer.conf", "w") as conf_file:
        conf_file.write(CONFIGURATION)

    time.sleep(0.2)

    yield proc  # testy się odpalą

    proc.terminate()
    try:
        proc.wait(timeout=5)
    except subprocess.TimeoutExpired:
        os.kill(proc.pid, signal.SIGKILL)

@pytest_asyncio.fixture
async def ws_client():
    """
    Tworzy klienta websocketowego na czas testu.
    Po teście automatycznie się zamyka.
    """
    async with websockets.connect(SERVER_URL) as websocket:
        yield websocket

@pytest_asyncio.fixture
async def ws_client1():
    async with websockets.connect(SERVER_URL) as ws:
        yield ws

@pytest_asyncio.fixture
async def ws_client2():
    async with websockets.connect(SERVER_URL) as ws:
        yield ws
