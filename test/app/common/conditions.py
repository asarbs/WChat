# World VTT
#
# Copyright (C) 2025, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.

import asyncio
import messeges_pb2

async def expect_response_ack(ws):
    raw_data1 = await asyncio.wait_for(ws.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data1)
    assert response.version              == 1
    assert response.type                 == messeges_pb2.MessageType.RESPONSE
    # assert response.response             == messeges_pb2.Response.ACK
    return response

async def expect_response_nack(ws1):
    raw_data1 = await asyncio.wait_for(ws1.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data1)
    assert response.version              == 1
    assert response.type                 == messeges_pb2.MessageType.RESPONSE
    assert response.response             == messeges_pb2.Response.NACK

async def wait_for_connection_request(ws_client):
    raw_data = await asyncio.wait_for(ws_client.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data)
    assert response.version                     == 1
    assert response.type                        == messeges_pb2.MessageType.CONTACT_CONNECTION_REQ
    return response.contactConnectionReq.from_user_id, response.contactConnectionReq.from_user_name, response.contactConnectionReq.to_user_id

async def wait_for_connection(ws_client):
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

async def expect_incoming_message(ws, from_uid, to_uid, msg):
    raw_data = await asyncio.wait_for(ws.recv(), timeout=2)
    response = messeges_pb2.Msg()
    response.ParseFromString(raw_data)
    assert response.version                  == 1
    assert response.type                     == messeges_pb2.MessageType.SEND_TEXT_MSG
    assert response.textMessage.from_user_id == int(from_uid)
    assert response.textMessage.to_user_id   == int(to_uid)
    assert response.textMessage.message      == msg
