/*
 * World VTT
 *
 * Copyright (C) 2025, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 *
 */

#include "ProtoWrapper.h"

#include "logger.h"

constexpr uint32_t ProtoAPIVersion = 1;

namespace WChat::ChatClient::server::api {
    ProtoBuffer buildRegisterSessionReq(const std::string& userName) {
        WChat::Msg msg;
        msg.set_version(ProtoAPIVersion);
        msg.set_type(WChat::MessageType::REGISTER_SESSION_REQ);
        WChat::RegisterSessionReq* req = msg.mutable_registersessionreq();
        req->set_user_name(userName);

        std::string serialized;
        msg.SerializeToString(&serialized);
        return serialized;
    }
    ProtoBuffer buildTestMsg(const uint64_t fromUserId, const uint64_t toUserId, const std::string& msgText) {
        WChat::Msg msg;
        msg.set_version(ProtoAPIVersion);
        msg.set_type(WChat::MessageType::SEND_TEXT_MSG);
        WChat::TextMessage* text = msg.mutable_textmessage();
        text->set_from_user_id(fromUserId);
        text->set_to_user_id(toUserId);
        text->set_message(msgText);

        std::string serialized;
        msg.SerializeToString(&serialized);
        return serialized;
    }

    ProtoBuffer buildContactConnectionReq(const uint64_t fromUserId, const uint64_t toUserId, const std::string& fromUserName) {  // cppcheck-suppress unusedFunction
        WChat::Msg msg;
        msg.set_version(ProtoAPIVersion);
        msg.set_type(WChat::MessageType::CONTACT_CONNECTION_REQ);
        WChat::ContactConnectionReq* req = msg.mutable_contactconnectionreq();
        req->set_from_user_id(fromUserId);
        req->set_from_user_name(fromUserName);
        req->set_to_user_id(toUserId);

        std::string serialized;
        msg.SerializeToString(&serialized);
        return serialized;
    }

    ProtoBuffer buildContactConnectionRes(const uint64_t fromUserId, const uint64_t toUserId, const Ack& ack) {  // cppcheck-suppress unusedFunction
        WChat::Msg msg;
        msg.set_version(ProtoAPIVersion);
        msg.set_type(WChat::MessageType::CONTACT_CONNECTION_REQ);
        WChat::ContactConnectionRes* res = msg.mutable_contactconnectionres();
        res->set_from_user_id(fromUserId);
        res->set_to_user_id(toUserId);
        res->set_status(WChat::Response(ack));

        std::string serialized;
        msg.SerializeToString(&serialized);
        return serialized;
    }

    ProtoBuffer buildUnregisterSession(const uint64_t userId) {  // cppcheck-suppress unusedFunction
        WChat::Msg msg;
        msg.set_version(ProtoAPIVersion);
        msg.set_type(WChat::MessageType::UNREGISTER_SESSION);
        WChat::UnregisterSessionReq* res = msg.mutable_unregistersessionreq();
        res->set_user_id(userId);

        std::string serialized;
        msg.SerializeToString(&serialized);
        return serialized;
    }

    ProtoBuffer buildListContact(const uint64_t userId) {
        WChat::Msg msg;
        msg.set_version(ProtoAPIVersion);
        msg.set_type(WChat::MessageType::LIST_CONTACT_REQ);
        WChat::ListContactReq* res = msg.mutable_listcontactreq();
        res->set_user_id(userId);

        std::string serialized;
        msg.SerializeToString(&serialized);
        return serialized;
    }

};  // namespace WChat::ChatClient::server::api
