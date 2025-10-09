# World VTT
#
# Copyright (C) 2025, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.

import json
import asyncio

import messeges_pb2

async def register_user(ws_client, user_name:str) -> int:
    msg = messeges_pb2.Msg()
    msg.version = 1
    msg.type    = messeges_pb2.MessageType.REGISTER_SESSION_REQ
    msg.registerSessionReq.user_name = user_name
    await ws_client.send(msg.SerializeToString())

    raw_data = await asyncio.wait_for(ws_client.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data)
    assert response.version                     == 1
    assert response.type                        == messeges_pb2.MessageType.REGISTER_SESSION_RES
    assert response.registerSessionRes.status   == messeges_pb2.Response.ACK
    return response.registerSessionRes.user_id
