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


SERVER_URL = "ws://localhost:9002/ws"
PROGRAM_CMD = ["../../build/linux/bin/WChat_SERVER"]


@pytest_asyncio.fixture(autouse=True)
def run_server():
    """
    Uruchamia serwer jako subprocess na czas testów.
    Zamyka go po zakończeniu testów.
    """
    # Start serwera
    proc = subprocess.Popen(PROGRAM_CMD, stdout=sys.stdout, stderr=sys.stderr)

    # Poczekaj chwilę aż serwer się podniesie
    time.sleep(2)

    yield proc  # testy się odpalą

    # Po testach -> zakończ serwer
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
