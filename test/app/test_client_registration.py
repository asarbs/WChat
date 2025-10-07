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

from server_procedures import *

@pytest.mark.asyncio
async def test_register_user(ws_client):
    msg = {"msg_type_id":2,"payload":{"user_name":"Asar"}}
    await ws_client.send(json.dumps(msg))
    response = json.loads(await asyncio.wait_for(ws_client.recv(), timeout=2))
    assert response['msg_type_id'] == 0
    assert response['payload']['status'] == "ok"
    assert response['payload']['user_name'] == "Asar"
    assert response['payload']['user_id'] == 0


@pytest.mark.asyncio
async def test_register_multiple_users(ws_client1, ws_client2):
    msg = {"msg_type_id":2,"payload":{"user_name":"Asar1"}}
    await ws_client1.send(json.dumps(msg))
    response = json.loads(await asyncio.wait_for(ws_client1.recv(), timeout=2))
    assert response['msg_type_id'] == 0
    assert response['payload']['status'] == "ok"
    assert response['payload']['user_name'] == "Asar1"
    assert response['payload']['user_id'] == 0

    msg = {"msg_type_id":2,"payload":{"user_name":"Asar2"}}
    await ws_client2.send(json.dumps(msg))
    response = json.loads(await asyncio.wait_for(ws_client2.recv(), timeout=2))
    assert response['msg_type_id'] == 0
    assert response['payload']['status'] == "ok"
    assert response['payload']['user_name'] == "Asar2"
    assert response['payload']['user_id'] == 1
