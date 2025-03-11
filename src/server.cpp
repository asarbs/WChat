#include <iostream>
#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> clients;

void on_open(websocketpp::connection_hdl hdl) {
    clients.insert(hdl);
    std::cout << "Nowe połączenie! Aktualnie podłączonych klientów: " << clients.size() << std::endl;
}

void on_close(websocketpp::connection_hdl hdl) {
    clients.erase(hdl);
    std::cout << "Rozłączono klienta. Pozostało klientów: " << clients.size() << std::endl;
}

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::cout << "Otrzymano wiadomość: " << msg->get_payload() << std::endl;

    for (auto it : clients) {
        s->send(it, msg->get_payload(), websocketpp::frame::opcode::text);
    }
}

int main() {
    server ws_server;

    try {
        ws_server.set_reuse_addr(true);
        ws_server.init_asio();

        ws_server.clear_access_channels(websocketpp::log::alevel::all);
        ws_server.clear_error_channels(websocketpp::log::elevel::all);

        ws_server.set_message_handler(bind(&on_message, &ws_server, std::placeholders::_1, std::placeholders::_2));
        ws_server.set_open_handler(&on_open);
        ws_server.set_close_handler(&on_close);

        ws_server.listen(9002);
        ws_server.start_accept();

        std::cout << "Serwer WebSocket działa na porcie 9002" << std::endl;

        ws_server.run();
    } catch (websocketpp::exception const& e) {
        std::cerr << "Wystąpił błąd: " << e.what() << std::endl;
    }

    return 0;
}
