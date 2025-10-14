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

@pytest.mark.asyncio
async def test_send_msg_to_unregistered_user(ws_client1):
    uid1 =  await register_user(ws_client=ws_client1, user_name="A1")
    await send_message_and_expect_response(ws1=ws_client1, uid1=uid1, uid2=42, msg="Lorem ipsum dolor sit ac.")


@pytest.mark.asyncio
async def test_double_registered_user(ws_client1):
    uidA1_1 =  await register_user(ws_client=ws_client1, user_name="A1")
    await unregister_user(ws_client1, uidA1_1);
    uidA1_2 =  await register_user(ws_client=ws_client1, user_name="A1")
    assert uidA1_1 == uidA1_2


# @pytest.mark.xfail(reason="Not implemented yet")
@pytest.mark.asyncio
async def test_user_get_waiting_msg(ws_client1, ws_client2):
    uid1 =  await register_user(ws_client=ws_client1, user_name="A1")
    uidA2_1 =  await register_user(ws_client=ws_client2, user_name="A2")
    await unregister_user(ws_client1, uidA2_1)
    await send_message_and_expect_response(ws1=ws_client1, uid1=uid1, uid2=uidA2_1, msg="Lorem ipsum dolor sit ac.")

    uidA2_2 =  await register_user(ws_client=ws_client2, user_name="A2")
    assert uidA2_1 == uidA2_2
    await expect_incoming_message(ws_client2, uid1, uidA2_2, "Lorem ipsum dolor sit ac.")



