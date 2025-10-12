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
    msg = messeges_pb2.Msg()
    msg.version = 1
    msg.type    = messeges_pb2.MessageType.REGISTER_SESSION_REQ
    msg.registerSessionReq.user_name = "Asar"
    await ws_client.send(msg.SerializeToString())

    raw_data = await asyncio.wait_for(ws_client.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data)
    assert response.version  == 1
    assert response.type     == messeges_pb2.MessageType.REGISTER_SESSION_RES
    assert response.registerSessionRes.status  == messeges_pb2.Response.ACK
    assert response.registerSessionRes.user_id == 0



@pytest.mark.asyncio
async def test_register_multiple_users(ws_client1, ws_client2):
    assert await register_user(ws_client1, "Asar1") == 0
    assert await register_user(ws_client2, "Asar2") == 1

@pytest.mark.asyncio
async def test_register_unregister_users(ws_client1):
    uid = await register_user(ws_client1, "Asar1")
    await unregister_user(ws_client1, uid)

