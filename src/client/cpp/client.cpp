/*
 * World VTT
 *
 * Copyright (C) 2025, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 */

#include <ixwebsocket/IXWebSocket.h>

#include <atomic>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/util/ref.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "ServerApi/ProtoDecoder.h"
#include "ServerApi/ProtoWrapper.h"
#include "logger.h"
int main() {
    WChat::ChatClient::ServerAPI::ProtoDecoder decoder;

    ix::WebSocket webSocket;
    bool isOpen = false;
    webSocket.setUrl("ws://localhost:9002/ws");
    webSocket.setPingInterval(15);
    // webSocket.enablePerMessageDeflate();
    webSocket.disablePerMessageDeflate();
    webSocket.setOnMessageCallback([&isOpen, &decoder](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            logger::logger << logger::debug << "Open Connection" << logger::endl;
            isOpen = true;
            // Headers can be inspected (pairs of string/string)
        } else if (msg->type == ix::WebSocketMessageType::Message) {
            decoder.decode(msg->str);
        } else if (msg->type == ix::WebSocketMessageType::Close) {
            logger::logger << logger::debug << "Close Connection" << logger::endl;

            // The server can send an explicit code and reason for closing.
            // This data can be accessed through the closeInfo object.
            logger::logger << logger::debug << msg->closeInfo.code << logger::endl;
            logger::logger << logger::debug << msg->closeInfo.reason << logger::endl;
        } else if (msg->type == ix::WebSocketMessageType::Error) {
            std::stringstream ss;
            ss << "Error: " << msg->errorInfo.reason << logger::endl;
            ss << "#retries: " << msg->errorInfo.retries << logger::endl;
            ss << "Wait time(ms): " << msg->errorInfo.wait_time << logger::endl;
            ss << "HTTP Status: " << msg->errorInfo.http_status << logger::endl;
            logger::logger << logger::debug << ss.str() << logger::endl;
        } else if (msg->type == ix::WebSocketMessageType::Ping || msg->type == ix::WebSocketMessageType::Pong) {
            logger::logger << logger::debug << "pong data: " << msg->str << logger::endl;
        }
    });

    webSocket.start();
    while (!isOpen) {
        logger::logger << logger::debug << "wait to open connection" << logger::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    ix::WebSocketSendInfo info = webSocket.sendBinary(WChat::ChatClient::ServerAPI::buildRegisterSessionReq("asar"));
    logger::logger << logger::debug << "send result:\n\r" <<         //
        "\tsucess=" << info.success << "\n\r" <<                     //
        "\tcompressionError:" << info.compressionError << "\n\r" <<  //
        "\tpayloadSize:" << info.payloadSize << "\n\r" <<            //
        "\twireSize:" << info.wireSize << "\n\r" <<                  //
        logger::endl;

    std::this_thread::sleep_for(std::chrono::seconds(30));
    webSocket.stop();

    return 0;

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    int value   = 50;
    std::string first_name;
    std::string time_str;
    std::atomic<bool> running         = true;
    ftxui::Component input_first_name = ftxui::Input(&first_name, "first name");

    auto help = ftxui::Button("[F1] Help", [&]() {  //
        value++;                                    //
        screen.PostEvent(ftxui::Event::Custom);     //
    });                                             //

    auto connect = ftxui::Button("[F2] Connect", [&]() {  //
        value = 50;                                       //
        screen.PostEvent(ftxui::Event::Custom);           //
    });                                                   //

    auto down = ftxui::Button("[F3] Down", [&]() {  //
        value--;                                    //
        screen.PostEvent(ftxui::Event::Custom);     //
    });                                             //

    auto exit = ftxui::Button("[F10] Exit", screen.ExitLoopClosure());

    auto container = ftxui::Container::Horizontal({
        help,
        connect,
        down,
        exit,
    });

    std::thread clock_thread([&running, &screen, &time_str]() {
        while (running) {
            auto now             = std::chrono::system_clock::now();
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&now_time), "%H:%M:%S");
            time_str = ss.str();
            screen.PostEvent(ftxui::Event::Custom);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    auto toolbarRenderer = ftxui::Renderer(container, [&] {
        return ftxui::hbox({
                   // lewa część: przyciski
                   ftxui::vbox({
                       ftxui::hbox({
                           help->Render(),
                           connect->Render(),
                           down->Render(),
                           exit->Render(),
                       }),
                   }) | ftxui::flex,

                   // prawa część: zegarek wyśrodkowany pionowo
                   ftxui::vbox({
                       ftxui::filler(),
                       ftxui::text(time_str) | ftxui::bold,
                       ftxui::filler(),
                   }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12),
               }) |
               ftxui::bgcolor(ftxui::Color::DarkBlue);
    });

    auto component = ftxui::Container::Vertical({
        input_first_name,  //
        toolbarRenderer    //
    });
    component->SetActiveChild(input_first_name);

    auto renderer = ftxui::Renderer(component, [&]() {  //
        auto elem = ftxui::vbox({
                        input_first_name->Render(),                       //
                        ftxui::separator(),                               //
                        ftxui::text("Value : " + std::to_string(value)),  //
                        ftxui::separator(),                               //
                        ftxui::text("Input : " + first_name)              //
                    }) |
                    ftxui::flex;

        return ftxui::vbox({
                   elem,                      //
                   toolbarRenderer->Render()  //
               }) |
               ftxui::border;
    });

    auto with_hotkeys = ftxui::CatchEvent(renderer, [&](ftxui::Event event) {
        if (event == ftxui::Event::F1) {
            value++;
            screen.PostEvent(ftxui::Event::Custom);
            return true;
        }
        if (event == ftxui::Event::F2) {
            value = 50;
            screen.PostEvent(ftxui::Event::Custom);
            return true;
        }
        if (event == ftxui::Event::F3) {
            value--;
            screen.PostEvent(ftxui::Event::Custom);
            return true;
        }
        if (event == ftxui::Event::F10) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
    });

    screen.Loop(with_hotkeys);
    running = false;
    clock_thread.join();
    return 0;
}
