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

#include "ProtoDecoder.h"

#include "logger.h"

namespace WChat::ChatClient::ServerAPI {
    ProtoDecoder::ProtoDecoder() {
    }

    ProtoDecoder::~ProtoDecoder() {
    }

    ProtoDecoder::ProtoDecoder(const ProtoDecoder& rhs) {
    }

    ProtoDecoder::ProtoDecoder(const ProtoDecoder&& rhs) {
    }

    ProtoDecoder& ProtoDecoder::operator=(const ProtoDecoder& rhs) {
        if (this != &rhs) {
            // pass
        }
        return *this;
    }

    ProtoDecoder& ProtoDecoder::operator=(const ProtoDecoder&& rhs) {
        if (this != &rhs) {
            // pass
        }
        return *this;
    }

    void ProtoDecoder::decode(const std::string& payload) {
        WChat::Msg msg;

        if (!msg.ParseFromString(payload)) {
            throw std::runtime_error("ParseFromString failed");
        }

        WChat::MessageType message_type_id = msg.type();
        logger::logger << logger::debug << "Message_type_id = `" << message_type_id << "` handler." << logger::endl;
        switch (message_type_id) {
            case WChat::MessageType::REGISTER_SESSION_RES:
                {
                    logger::logger << logger::debug << ".user_id = `" << msg.registersessionres().user_id() << "` handler." << logger::endl;
                    logger::logger << logger::debug << ".status = `" << msg.registersessionres().status() << "` handler." << logger::endl;
                }
                break;
            default:
                logger::logger << logger::debug << "unsupported message type id = `" << message_type_id << "` handler." << logger::endl;
        }
    }
};  // namespace WChat::ChatClient::ServerAPI
