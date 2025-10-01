# World VTT
#
# Copyright (C) 2025, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.


import pytest
import asyncio
import json


@pytest.mark.asyncio
async def test_ping(ws_client):
    msg = {"msg_type_id":2,"payload":{"user_id":"Asar"}}
    await ws_client.send(json.dumps(msg))
    response = json.loads(await asyncio.wait_for(ws_client.recv(), timeout=2))

# @pytest.mark.asyncio
# async def test_ping(ws_client):
#     """Test sprawdzający, czy serwer odpowiada na ping"""
#     await ws_client.send(json.dumps({"action": "ping"}))
#     response = json.loads(await asyncio.wait_for(ws_client.recv(), timeout=2))

#     assert response["status"] == "ok"


# @pytest.mark.asyncio
# async def test_echo(ws_client):
#     """Test sprawdzający funkcję echo"""
#     message = {"action": "echo", "data": "hello world"}
#     await ws_client.send(json.dumps(message))

#     response = json.loads(await asyncio.wait_for(ws_client.recv(), timeout=2))

#     assert response["data"] == "hello world"


# @pytest.mark.asyncio
# async def test_invalid_request(ws_client):
#     """Test sprawdzający, czy serwer poprawnie reaguje na zły request"""
#     await ws_client.send(json.dumps({"foo": "bar"}))
#     response = json.loads(await asyncio.wait_for(ws_client.recv(), timeout=2))

#     assert response.get("error") is not None
