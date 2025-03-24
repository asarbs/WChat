#include <iostream>
#include <set>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>

#include "logger.h"
#include "arguments.h"

#include "ChatClient.h"
#include "serwer/messages/MessageManager.h"

typedef websocketpp::server<websocketpp::config::asio> server;

// std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> clients;
std::vector<ChatClient> __chat_clients;
MessageManager __messageManager;

bool operator==(const websocketpp::connection_hdl& a, const websocketpp::connection_hdl& b) {
    // Porównanie connection_hdl na podstawie shared_ptr
    return a.lock() == b.lock();
}

void on_open(websocketpp::connection_hdl hdl) {
    ChatClient cc;
    cc.connection = hdl;
    __chat_clients.push_back(cc);
    logger::logger << logger::debug << "Nowe połączenie! Aktualnie podłączonych klientów: " << __chat_clients.size() << logger::endl;
}

void on_close(websocketpp::connection_hdl hdl) {
    // __chat_clients.erase(hdl);
    logger::logger << logger::debug << "Rozłączono klienta. Pozostało klientów: " << __chat_clients.size() << logger::endl;
}

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    logger::logger << logger::debug << "Raw msg: `" << msg->get_payload() << "`;" << logger::endl;

    try {
        nlohmann::json data = nlohmann::json::parse(msg->get_payload());

        int msg_type_id = data.at("msg_type_id");
        logger::logger << logger::debug << "msg_type_id=" << msg_type_id << logger::endl;


        __messageManager.handle(msg_type_id, data.at("payload"));

        if(msg_type_id == 1) { // chat msg -> resend to all
            std::string user_id = data.at("payload").value("user_id", "unknown");
            std::string message = data.at("payload").value("message", "");
            logger::logger << logger::debug << "user_id: " << user_id << ", message: `" << message << "`" << logger::endl;
            for (ChatClient it : __chat_clients) {
                s->send(it.connection, msg->get_payload(), websocketpp::frame::opcode::text);
            }
        } else if (msg_type_id == 2) { // register user ID
            for (auto it = __chat_clients.begin(); it != __chat_clients.end(); ++it) {
                if(it->connection == hdl){
                    std::string user_id = data.at("payload").value("user_id", "unknown");
                    it->user_id = user_id;
                    logger::logger << logger::debug << "New user registered:" << user_id  << ";" << logger::endl;
                }
            }
        }
    } catch(nlohmann::json::parse_error& e) {
        logger::logger << logger::error << "JSON parse error: `" << e.what() << "`." << logger::endl;
    }


}

int main(int argc, char *argv[]) {
    logger::logger.setLogLevel(logger::debug);
    server ws_server;

    Argument::ArgumentParser &argpars = Argument::ArgumentParser::getInstance("Chat", {0, 0, 1});
    // argpars.addArgument("--level", Argument::Action::Store, "-l", "Path to level file.", "assets/test.yaml");
    argpars.parse(argc, argv);



    try {
        ws_server.set_reuse_addr(true);
        ws_server.init_asio();

        ws_server.clear_access_channels(websocketpp::log::alevel::all);
        ws_server.clear_error_channels(websocketpp::log::elevel::all);

        ws_server.set_message_handler(bind(&on_message, &ws_server, std::placeholders::_1, std::placeholders::_2));
        ws_server.set_open_handler(&on_open);
        ws_server.set_close_handler(&on_close);
        // ws_server.set_validate_handler(&validate_handler);

        ws_server.listen(boost::asio::ip::tcp::v4(),9002);
        ws_server.start_accept();

        logger::logger << logger::debug << "Serwer WebSocket działa na porcie 9002" << logger::endl;

        ws_server.run();
    } catch (websocketpp::exception const& e) {
        logger::logger << logger::error << "Wystąpił błąd: " << e.what() << logger::endl;
    }

    return 0;
}
