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

async def __send_text_msg(ws1, uid1, uid2, msg):
    pmsg = messeges_pb2.Msg()
    pmsg.version    = 1
    pmsg.type       = messeges_pb2.MessageType.SEND_TEXT_MSG
    pmsg.textMessage.from_user_id = int(uid1)
    pmsg.textMessage.to_user_id   = int(uid2)
    pmsg.textMessage.message = msg
    await ws1.send(pmsg.SerializeToString())

async def __expect_response_ack(ws):
    raw_data1 = await asyncio.wait_for(ws.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data1)
    assert response.version              == 1
    assert response.type                 == messeges_pb2.MessageType.RESPONSE
    # assert response.response             == messeges_pb2.Response.ACK
    return response

async def __expect_response_nack(ws1):
    raw_data1 = await asyncio.wait_for(ws1.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data1)
    assert response.version              == 1
    assert response.type                 == messeges_pb2.MessageType.RESPONSE
    assert response.response             == messeges_pb2.Response.NACK

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

async def send_connection_request(ws_client, connection_from_user_id:int, connection_to_user_id:int, connection_from_user_name:str) -> None:
    msg = messeges_pb2.Msg()
    msg.version = 1
    msg.type    = messeges_pb2.MessageType.CONTACT_CONNECTION_REQ
    msg.contactConnectionReq.from_user_id = connection_from_user_id
    msg.contactConnectionReq.to_user_id = connection_to_user_id
    msg.contactConnectionReq.from_user_name = connection_from_user_name
    await ws_client.send(msg.SerializeToString())
    return await __expect_response_ack(ws_client)

async def wait_for_connection_request(ws_client):
    raw_data = await asyncio.wait_for(ws_client.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data)
    assert response.version                     == 1
    assert response.type                        == messeges_pb2.MessageType.CONTACT_CONNECTION_REQ
    return response.contactConnectionReq.from_user_id, response.contactConnectionReq.from_user_name, response.contactConnectionReq.to_user_id


async def send_connection_ack(ws_client, to_id:int, from_id:int) -> None:
    msg = messeges_pb2.Msg()
    msg.version = 1
    msg.type    = messeges_pb2.MessageType.CONTACT_CONNECTION_RES
    msg.contactConnectionRes.status         = messeges_pb2.Response.ACK
    msg.contactConnectionRes.from_user_id   = from_id
    msg.contactConnectionRes.to_user_id     = to_id
    await ws_client.send(msg.SerializeToString())
    await __expect_response_ack(ws_client)

async def send_connection_nack(ws_client, to_id:int, from_id:int) -> None:
    msg = messeges_pb2.Msg()
    msg.version = 1
    msg.type    = messeges_pb2.MessageType.CONTACT_CONNECTION_RES
    msg.contactConnectionRes.status         = messeges_pb2.Response.NACK
    msg.contactConnectionRes.from_user_id   = from_id
    msg.contactConnectionRes.to_user_id     = to_id
    await ws_client.send(msg.SerializeToString())
    await __expect_response_ack(ws_client)

async def wait_for_connection_ack(ws_client):
    raw_data = await asyncio.wait_for(ws_client.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data)
    assert response.version                     == 1
    assert response.type                        == messeges_pb2.MessageType.CONTACT_CONNECTION_RES
    assert response.contactConnectionRes.status == messeges_pb2.Response.ACK
    return response.contactConnectionRes.status, response.contactConnectionRes.from_user_id, response.contactConnectionRes.to_user_id

async def wait_for_connection_nack(ws_client):
    raw_data = await asyncio.wait_for(ws_client.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data)
    assert response.version                     == 1
    assert response.type                        == messeges_pb2.MessageType.CONTACT_CONNECTION_RES
    assert response.contactConnectionRes.status == messeges_pb2.Response.NACK
    return response.contactConnectionRes.status, response.contactConnectionRes.from_user_id, response.contactConnectionRes.to_user_id


async def unregister_user(ws_client, uid:int) -> int:
    msg = messeges_pb2.Msg()
    msg.version = 1
    msg.type    = messeges_pb2.MessageType.UNREGISTER_SESSION
    msg.unregisterSessionReq.user_id = uid
    await ws_client.send(msg.SerializeToString())
    await __expect_response_ack(ws=ws_client)


async def send_message_and_expect_correct_response(ws1, ws2, uid1, uid2, msg):
    await __send_text_msg(ws1, ws2, uid1, uid2, msg)

    raw_data1 = await asyncio.wait_for(ws1.recv(), timeout=2)
    raw_data2 = await asyncio.wait_for(ws2.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data1)
    assert response.version              == 1
    assert response.type                 == messeges_pb2.MessageType.SEND_TEXT_MSG
    assert response.textMessage.from_user_id     == int(uid1)
    assert response.textMessage.to_user_id       == int(uid2)
    assert response.textMessage.message  == msg

    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data2)
    assert response.version              == 1
    assert response.type                 == messeges_pb2.MessageType.SEND_TEXT_MSG
    assert response.textMessage.from_user_id     == int(uid1)
    assert response.textMessage.to_user_id       == int(uid2)
    assert response.textMessage.message  == msg

async def send_message_and_expect_nack(ws1, uid1, uid2, msg):
    await __send_text_msg(ws1, uid1, uid2, msg)
    await __expect_response_nack(ws1)

async def send_message_and_expect_response(ws1, uid1, uid2, msg):
    await __send_text_msg(ws1, uid1, uid2, msg)
    raw_data1 = await asyncio.wait_for(ws1.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data1)
    assert response.version              == 1
    assert response.type                 == messeges_pb2.MessageType.SEND_TEXT_MSG
    assert response.textMessage.from_user_id     == int(uid1)
    assert response.textMessage.to_user_id       == int(uid2)
    assert response.textMessage.message  == msg

async def expect_incoming_message(ws, from_uid, to_uid, msg):
    raw_data = await asyncio.wait_for(ws.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data)
    assert response.version                  == 1
    assert response.type                     == messeges_pb2.MessageType.SEND_TEXT_MSG
    assert response.textMessage.from_user_id == int(from_uid)
    assert response.textMessage.to_user_id   == int(to_uid)
    assert response.textMessage.message      == msg
