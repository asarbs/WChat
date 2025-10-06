# World VTT
#
# Copyright (C) 2025, Asar Miniatures
# All rights reserved.
#
# This file is part of the [Project Name] project. It may be used, modified,
# and distributed under the terms specified by the copyright holder.

import json
import asyncio


async def register_user(ws_client, user_name:str):
    assert type(user_name) == str
    msg = {"msg_type_id":2,"payload":{"user_id":user_name}}
    await ws_client.send(json.dumps(msg))
    response = json.loads(await asyncio.wait_for(ws_client.recv(), timeout=2))
    assert response['msg_type_id'] == 0
    assert response['payload']['status'] == "ok"
    return True
