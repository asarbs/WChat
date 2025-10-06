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
async def test_register_user_and_send_msg(ws_client):
    user_A1_id =  await register_user(ws_client=ws_client, user_name="A1")
    user_A2_id =  await register_user(ws_client=ws_client, user_name="A2")
    msg= {"msg_type_id":1,"payload":{
          "from": int(user_A1_id),
          "to":   int(user_A2_id),
          "message": "Lorem ipsum dolor sit ac."
    }}
    await ws_client.send(json.dumps(msg))
    response = json.loads(await asyncio.wait_for(ws_client.recv(), timeout=2))
    assert response['msg_type_id'] == 3
    assert response['payload']['from'] == user_A1_id
    assert response['payload']['to'] == user_A2_id
    assert response['payload']['message'] == "Lorem ipsum dolor sit ac."
