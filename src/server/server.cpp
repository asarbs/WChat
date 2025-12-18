/*
 * World VTT
 *
 * Copyright (C) 2025, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 */

#include <format>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>
#include <stdexcept>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "arguments.h"
#include "logger.h"
#include "server/client/ChatClient.h"
#include "server/client/ChatClientDatabase.h"
#include "server/core/ServerConfig.h"
#include "server/core/storage/db/sqlite/SQLightWrapper.h"
#include "server/errors/ErrorHandlers.h"
#include "server/messages/Manager.h"
#include "server/messages/handlers/ConnectionReq.h"
#include "server/messages/handlers/ConnectionRes.h"
#include "server/messages/handlers/ListContactReq.h"
#include "server/messages/handlers/Message.h"
#include "server/messages/handlers/RegisterClientSession.h"
#include "server/messages/handlers/UnregisterClientSession.h"
#include "server/proto/messeges.pb.h"

// std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> clients;

WChat::ChatServer::messages::Manager __messageManager;

bool operator==(const websocketpp::connection_hdl& a, const websocketpp::connection_hdl& b) {
    // Porównanie connection_hdl na podstawie shared_ptr
    return a.lock() == b.lock();
}

void on_open(websocketpp::connection_hdl hdl) {
    logger::logger << logger::debug << "New connection! Currently connected clients: " << WChat::ChatServer::client::ChatClientDatabase::getInstance().size() << logger::endl;
}

void on_close(websocketpp::connection_hdl hdl) {
    logger::logger << logger::debug << "Client disconnected. Clients remaining: " << WChat::ChatServer::client::ChatClientDatabase::getInstance().size() << logger::endl;
}

void on_message(websocket_server* s, websocketpp::connection_hdl hdl, websocket_server::message_ptr msg) {
    try {
        if (msg->get_opcode() != websocketpp::frame::opcode::binary) {
            throw WChat::ChatServer::errors::ProtoculError("Received non-binary message");
        }

        const std::string& payload = msg->get_payload();

        WChat::Msg proto_msg;
        if (!proto_msg.ParseFromString(payload)) {
            throw WChat::ChatServer::errors::ProtoculError("ParseFromString failed");
        }

        if (proto_msg.type() >= WChat::MessageType::LAST) {
            std::string error_msg = std::format("Msg type[{}] out of band", uint32_t(proto_msg.type()));
            throw WChat::ChatServer::errors::ProtoculWarning(error_msg);
        }

        __messageManager.handle(s, hdl, proto_msg);

        // nlohmann::json j;
        // j["msg_type_id"]       = 0;
        // j["payload"]["status"] = "ok";

        // std::string msg_str = j.dump();
        // logger::logger << logger::debug << msg_str << logger::endl;
        // s->send(hdl, msg_str, websocketpp::frame::opcode::text);

        // if (msg_type_id == 1) {  // chat msg -> resend to all
        // std::string user_id = data.at("payload").value("user_id", "unknown");
        // std::string message = data.at("payload").value("message", "");
        // logger::logger << logger::debug << "user_id: " << user_id << ", message: `" << message << "`" << logger::endl;
        // for (ChatClient it : __chat_clients) {
        //     s->send(it.connection, msg->get_payload(), websocketpp::frame::opcode::text);
        // }
        // } else if (msg_type_id == 2) {  // register user ID
        //     for (auto it = __chat_clients.begin(); it != __chat_clients.end(); ++it) {
        //         if (it->connection == hdl) {
        //             std::string user_id = data.at("payload").value("user_id", "unknown");
        //             it->user_id         = user_id;
        //             logger::logger << logger::debug << "New user registered:" << user_id << ";" << logger::endl;
        //         }
        //     }
        // }
    } catch (nlohmann::json::parse_error& e) {
        logger::logger << logger::error << "JSON parse error: `" << e.what() << "`." << logger::endl;
    } catch (nlohmann::json_abi_v3_11_3::detail::out_of_range& e) {
        logger::logger << logger::error << "JSON parse error: `" << e.what() << "`." << logger::endl;
        WChat::ChatServer::messages::handlers::send_nack(s, hdl);
    } catch (WChat::ChatServer::errors::ProtoculWarning& e) {
        logger::logger << logger::warning << "Runtime Wargning:" << e.what() << "." << logger::endl;
        WChat::ChatServer::messages::handlers::send_nack(s, hdl);
    } catch (std::runtime_error& e) {
        logger::logger << logger::critical << "Runtime Error:" << e.what() << "." << logger::endl;
        WChat::ChatServer::messages::handlers::send_nack(s, hdl);
    } catch (...) {
        logger::logger << logger::critical << "UNKNOWN Error" << logger::endl;
        WChat::ChatServer::messages::handlers::send_nack(s, hdl);
        throw;
    }
}

int main(int argc, char* argv[]) {
    try {
        logger::logger.setLogLevel(logger::debug);
        logger::logger << logger::info << "Start WChat Server" << logger::endl;
        websocket_server ws_server;

        WChat::ChatServer::core::storage::db::sqlite::SQLightWrapper::instance();
        WChat::ChatServer::core::ServerConfig::instance().loadFromFile();

        Argument::ArgumentParser& argpars = Argument::ArgumentParser::getInstance("Chat", {0, 0, 1});
        argpars.parse(argc, argv);

        __messageManager.register_handler(WChat::MessageType::SEND_TEXT_MSG, std::make_shared<WChat::ChatServer::messages::handlers::Message>());
        __messageManager.register_handler(WChat::MessageType::REGISTER_SESSION_REQ, std::make_shared<WChat::ChatServer::messages::handlers::RegisterClientSession>());
        __messageManager.register_handler(WChat::MessageType::UNREGISTER_SESSION, std::make_shared<WChat::ChatServer::messages::handlers::UnregisterClientSession>());
        __messageManager.register_handler(WChat::MessageType::CONTACT_CONNECTION_REQ, std::make_shared<WChat::ChatServer::messages::handlers::ConnectionReq>());
        __messageManager.register_handler(WChat::MessageType::CONTACT_CONNECTION_RES, std::make_shared<WChat::ChatServer::messages::handlers::ConnectionRes>());
        __messageManager.register_handler(WChat::MessageType::LIST_CONTACT_REQ, std::make_shared<WChat::ChatServer::messages::handlers::ListContactReq>());

        ws_server.set_reuse_addr(true);
        ws_server.init_asio();

        ws_server.clear_access_channels(websocketpp::log::alevel::all);
        ws_server.clear_error_channels(websocketpp::log::elevel::all);

        ws_server.set_message_handler(bind(&on_message, &ws_server, std::placeholders::_1, std::placeholders::_2));
        ws_server.set_open_handler(&on_open);
        ws_server.set_close_handler(&on_close);
        // ws_server.set_validate_handler(&validate_handler);
        uint32_t port = WChat::ChatServer::core::ServerConfig::instance().value<uint32_t>(WChat::ChatServer::core::ParamKey::Port);
        ws_server.listen(boost::asio::ip::tcp::v4(), port);
        ws_server.start_accept();

        logger::logger << logger::debug << "server WebSocket port " << port << logger::endl;

        ws_server.run();
    } catch (websocketpp::exception const& e) {
        logger::logger << logger::error << "Error: " << e.what() << logger::endl;
    }
    logger::logger << logger::info << "Close WChat Server" << logger::endl;
    return 0;
}
