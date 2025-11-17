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

#ifndef PROTOWRAPPER_H
#define PROTOWRAPPER_H

#pragma once

#include <string>
#include <variant>

#include "proto/messeges.pb.h"

namespace WChat::ChatClient::ServerAPI {
    using ProtoBuffer     = std::string;
    using ProtoRxMessages = std::variant<WChat::Msg, WChat::RegisterSessionRes, WChat::TextMessage, WChat::ListContactRes>;
    using ProtoTxMessages = std::variant<WChat::Msg, WChat::RegisterSessionReq, WChat::TextMessage, WChat::ContactConnectionReq, WChat::ContactConnectionRes,
                                         WChat::UnregisterSessionReq, WChat::ListContactReq>;
    struct Ack {
            enum class Type {
                Ack,
                Nack,
            } value;

            constexpr operator bool() const {
                return value == Type::Ack;
            }
            operator WChat::Response() const {
                return (*this) ? WChat::Response::ACK : WChat::Response::NACK;
            }
    };

    ProtoBuffer buildRegisterSessionReq(const std::string& userName);
    ProtoBuffer buildTestMsg(const uint64_t fromUserId, const uint64_t toUserId, const std::string& msgText);
    ProtoBuffer buildContactConnectionReq(const uint64_t fromUserId, const uint64_t toUserId, const std::string& fromUserName);
    ProtoBuffer buildContactConnectionRes(const uint64_t fromUserId, const uint64_t toUserId, const Ack& ack);
    ProtoBuffer buildUnregisterSession(const uint64_t userId);
    ProtoBuffer buildListContact(const uint64_t userId);

};  // namespace WChat::ChatClient::ServerAPI

#endif
