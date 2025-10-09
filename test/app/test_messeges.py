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
async def test_register_user_and_send_msg(ws_client1, ws_client2):
    user_A1_id =  await register_user(ws_client=ws_client1, user_name="A1")
    user_A2_id =  await register_user(ws_client=ws_client2, user_name="A2")

    msg = messeges_pb2.Msg()
    msg.version    = 1
    msg.type       = messeges_pb2.MessageType.SEND_TEXT_MSG
    msg.textMessage.from_user_id = int(user_A1_id)
    msg.textMessage.to_user_id   = int(user_A2_id)
    msg.textMessage.message = "Lorem ipsum dolor sit ac."
    await ws_client1.send(msg.SerializeToString())

    raw_data1 = await asyncio.wait_for(ws_client1.recv(), timeout=2)
    raw_data2 = await asyncio.wait_for(ws_client2.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data1)
    assert response.version              == 1
    assert response.type                 == messeges_pb2.MessageType.SEND_TEXT_MSG
    assert response.textMessage.from_user_id     == int(user_A1_id)
    assert response.textMessage.to_user_id       == int(user_A2_id)
    assert response.textMessage.message  == "Lorem ipsum dolor sit ac."

    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data2)
    assert response.version              == 1
    assert response.type                 == messeges_pb2.MessageType.SEND_TEXT_MSG
    assert response.textMessage.from_user_id     == int(user_A1_id)
    assert response.textMessage.to_user_id       == int(user_A2_id)
    assert response.textMessage.message  == "Lorem ipsum dolor sit ac."
