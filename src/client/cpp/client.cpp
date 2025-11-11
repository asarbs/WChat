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

#include "logger.h"

int main() {
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    int value = 50;
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
