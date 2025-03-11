#include <iostream>
#include <thread>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> client;

void on_message(websocketpp::connection_hdl, client::message_ptr msg) {
    std::cout << "\n📩 Otrzymano: " << msg->get_payload() << std::endl;
    std::cout << "Wpisz wiadomość do wysłania: ";
    std::cout.flush();
}

int main() {
    client ws_client;

    try {
        ws_client.init_asio();

        ws_client.clear_access_channels(websocketpp::log::alevel::all);
        ws_client.clear_error_channels(websocketpp::log::elevel::all);

        ws_client.set_message_handler(&on_message);

        websocketpp::lib::error_code ec;
        client::connection_ptr con = ws_client.get_connection("ws://localhost:9002", ec);

        if (ec) {
            std::cerr << "❌ Błąd połączenia: " << ec.message() << std::endl;
            return 1;
        }

        ws_client.connect(con);
        std::cout << "✅ Połączono z serwerem WebSocket!\n";

        // Uruchom pętlę odbiorczą w osobnym wątku
        std::thread client_thread([&ws_client]() { ws_client.run(); });

        // Pętla wysyłania wiadomości
        std::string input;
        while (true) {
            std::cout << "Wpisz wiadomość do wysłania: ";
            std::getline(std::cin, input);
            if (input == "exit") break;

            ws_client.send(con->get_handle(), input, websocketpp::frame::opcode::text);
        }

        // Zamknięcie połączenia
        ws_client.close(con->get_handle(), websocketpp::close::status::going_away, "Koniec pracy klienta");
        client_thread.join();
    } catch (websocketpp::exception const &e) {
        std::cerr << "❌ Błąd klienta: " << e.what() << std::endl;
    }

    return 0;
}
