/*
 * World VTT
 *
 * Copyright (C) 2024, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 */

#include <iostream>
#include <nlohmann/json.hpp>
#include <set>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "ChatClient.h"
#include "ChatClientDatabase.h"
#include "arguments.h"
#include "logger.h"
#include "serwer/messages/MessageHandler_Message.h"
#include "serwer/messages/MessageHandler_RegisterClient.h"
#include "serwer/messages/MessageManager.h"

// std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> clients;

MessageManager __messageManager;

bool operator==(const websocketpp::connection_hdl& a, const websocketpp::connection_hdl& b) {
    // Porównanie connection_hdl na podstawie shared_ptr
    return a.lock() == b.lock();
}

void on_open(websocketpp::connection_hdl hdl) {
    // ChatClientDatabase::getInstance().regiserClinet(hdl);
    logger::logger << logger::debug << "New connection! Currently connected clients: " << ChatClientDatabase::getInstance().size() << logger::endl;
}

void on_close(websocketpp::connection_hdl hdl) {
    logger::logger << logger::debug << "Client disconnected. Clients remaining: " << ChatClientDatabase::getInstance().size() << logger::endl;
}

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    // logger::logger << logger::debug << "Raw msg: `" << msg->get_payload() << "`;" << logger::endl;

    try {
        nlohmann::json data = nlohmann::json::parse(msg->get_payload());

        int msg_type_id = data.at("msg_type_id");
        logger::logger << logger::debug << "msg_type_id=" << msg_type_id << logger::endl;

        __messageManager.handle(s, hdl, msg_type_id, data.at("payload"));

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
        send_nack(s, hdl);
    }
}

int main(int argc, char* argv[]) {
    logger::logger.setLogLevel(logger::debug);
    logger::logger << logger::info << "Start WChat Server" << logger::endl;
    server ws_server;

    Argument::ArgumentParser& argpars = Argument::ArgumentParser::getInstance("Chat", {0, 0, 1});
    // argpars.addArgument("--level", Argument::Action::Store, "-l", "Path to level file.", "assets/test.yaml");
    argpars.parse(argc, argv);

    __messageManager.register_handler(1, std::make_shared<MessageHandler_Message>());
    __messageManager.register_handler(2, std::make_shared<MessageHandler_RegisterClient>());

    try {
        ws_server.set_reuse_addr(true);
        ws_server.init_asio();

        ws_server.clear_access_channels(websocketpp::log::alevel::all);
        ws_server.clear_error_channels(websocketpp::log::elevel::all);

        ws_server.set_message_handler(bind(&on_message, &ws_server, std::placeholders::_1, std::placeholders::_2));
        ws_server.set_open_handler(&on_open);
        ws_server.set_close_handler(&on_close);
        // ws_server.set_validate_handler(&validate_handler);

        ws_server.listen(boost::asio::ip::tcp::v4(), 9002);
        ws_server.start_accept();

        logger::logger << logger::debug << "Serwer WebSocket działa na porcie 9002" << logger::endl;

        ws_server.run();
    } catch (websocketpp::exception const& e) {
        logger::logger << logger::error << "Wystąpił błąd: " << e.what() << logger::endl;
    }
    logger::logger << logger::info << "Close WChat Server" << logger::endl;
    return 0;
}
