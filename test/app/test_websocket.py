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
import messeges_pb2
from google.protobuf.json_format import MessageToJson

from server_procedures import *

UINT32_MAX = (1 << 32) - 1

@pytest.mark.asyncio
async def test_invalid_msg(ws_client):
    registration_msg = messeges_pb2.Msg();
    registration_msg.version = 1
    registration_msg.type    = messeges_pb2.MessageType.LAST
    msg_string = registration_msg.SerializeToString()
    await ws_client.send(msg_string)

    data = await asyncio.wait_for(ws_client.recv(), timeout=2)
    response_msg = messeges_pb2.Msg()
    response_msg.ParseFromString(data)
    assert response_msg.version  == 1
    assert response_msg.type     == messeges_pb2.MessageType.RESPONSE
    assert response_msg.response == messeges_pb2.Response.NACK
