# World VTT
#
# Copyright (C) 2025, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.


import pytest
import asyncio
import messeges_pb2

from common.fixtures import *
import common.actions as act
import common.conditions as con

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
    assert await act.register_user(ws_client1, "Asar1") == 0
    assert await act.register_user(ws_client2, "Asar2") == 1

@pytest.mark.asyncio
async def test_register_unregister_users(ws_client1):
    uid = await act.register_user(ws_client1, "Asar1")
    await act.unregister_user(ws_client1, uid)

@pytest.mark.asyncio
async def test_register_reregister_users(ws_client1):
    uid = await act.register_user(ws_client1, "Asar1")
    assert uid == 0
    await act.unregister_user(ws_client1, uid)
    assert await act.register_user(ws_client1, "Asar1") == 0

@pytest.mark.asyncio
async def test_add_contact_positive_scenario(ws_client1, ws_client2):
    from_u1_name = "Asar1"
    from_uid1 = await act.register_user(ws_client1, from_u1_name)
    to_uid2 = await act.register_user(ws_client2, "Asar2")
    await act.send_connection_request(ws_client1, from_uid1, to_uid2, from_u1_name)
    from_id, from_name, to_id = await con.wait_for_connection_request(ws_client2)
    assert from_id == from_uid1
    assert from_name == from_u1_name
    assert to_id == to_uid2
    await act.send_connection(ws_client2, to_uid2, from_uid1)
    ack, from_ack_id, to_ack_id =await con.wait_for_connection(ws_client1)
    assert ack == messeges_pb2.Response.ACK
    assert from_ack_id == from_uid1
    assert to_ack_id == to_uid2

@pytest.mark.asyncio
async def test_add_contact_negative_scenario(ws_client1, ws_client2):
    from_u1_name = "Asar1"
    from_uid1 = await act.register_user(ws_client1, from_u1_name)
    to_uid2 = await act.register_user(ws_client2, "Asar2")
    await act.send_connection_request(ws_client1, from_uid1, to_uid2, from_u1_name)
    from_id, from_name, to_id = await con.wait_for_connection_request(ws_client2)
    assert from_id == from_uid1
    assert from_name == from_u1_name
    assert to_id == to_uid2
    await act.send_connection_nack(ws_client2, to_uid2, from_uid1)
    ack, from_ack_id, to_ack_id = await con.wait_for_connection_nack(ws_client1)
    assert ack == messeges_pb2.Response.NACK
    assert from_ack_id == from_uid1
    assert to_ack_id == to_uid2

pytest.mark.asyncio
async def test_get_contact_list(ws_client1, ws_client2):
    from_u1_name = "Asar1"
    from_uid1 = await act.register_user(ws_client1, from_u1_name)
    to_uid1 = await act.register_user(ws_client2, "Rasa1")
    to_uid2 = await act.register_user(ws_client2, "Rasa2")
    to_uid3 = await act.register_user(ws_client2, "Rasa3")
    await act.add_contact_for_user(ws_client1, ws_client2, from_uid1, to_uid1, from_u1_name)
    await act.add_contact_for_user(ws_client1, ws_client2, from_uid1, to_uid2, from_u1_name)
    await act.add_contact_for_user(ws_client1, ws_client2, from_uid1, to_uid3, from_u1_name)
    contacts = await act.get_contect_list(ws_client1, from_uid1)
    assert contacts[0]['name'] == "Rasa1" , (f"{contacts[0]['name']} == `Rasa1`")
    assert contacts[1]['name'] == "Rasa2" , (f"{contacts[1]['name']} == `Rasa2`")
    assert contacts[2]['name'] == "Rasa3" , (f"{contacts[2]['name']} == `Rasa3`")
    assert contacts[0]['id'] == 1 , (f"{contacts[0]['id']} == {1}")
    assert contacts[1]['id'] == 2 , (f"{contacts[1]['id']} == {2}")
    assert contacts[2]['id'] == 3 , (f"{contacts[2]['id']} == {3}")
