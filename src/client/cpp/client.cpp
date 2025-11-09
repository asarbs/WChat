/*
 * World VTT
 *
 * Copyright (C) 2025, Asar Miniatures
 * All rights reserved.
 *
 * This file is part of the [Project Name] project. It may be used, modified,
 * and distributed under the terms specified by the copyright holder.
 */

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
#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string
#include <thread>
#include <vector>

#include "logger.h"

int main() {
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    int value = 50;
    std::string first_name;
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

    auto toolbarRenderer = ftxui::Renderer(container, [&] {
        return ftxui::hbox({
                   help->Render(),
                   connect->Render(),
                   down->Render(),
                   exit->Render(),
               }) |
               ftxui::hcenter | ftxui::bgcolor(ftxui::Color::DarkBlue);
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
    screen.Loop(renderer);
}

/*
int main_test_tui() {  // cppcheck-suppress unusedFunction
    using namespace ftxui;

    std::string first_name;
    std::string last_name;
    std::string password;
    std::string phoneNumber;
    std::string timer;

    std::vector<std::string> tab_values{"tab_1", "tab_2", "tab_3"};
    int tab_selected = 0;
    auto tab_toggle  = Toggle(&tab_values, &tab_selected);
    std::vector<std::string> tab_1_entries{"Forest", "Water", "I don't know"};
    int tab_1_selected = 0;

    std::vector<std::string> tab_2_entries{"Hello", "Hi", "Hay"};
    int tab_2_selected = 0;

    std::vector<std::string> tab_3_entries{"Table", "Nothing", "Is", "Empty"};
    int tab_3_selected = 0;

    auto tab_1 = Renderer(Radiobox(&tab_1_entries, &tab_1_selected), [&] {
        return vbox({
                   text("=== TAB 1 ==="),
                   text("Timer: " + timer),  // 👈 zegar tylko w tab 1
               }) |
               border;
    });

    auto tab_2 = Renderer(Radiobox(&tab_2_entries, &tab_2_selected), [&] {
        return vbox({
                   text("=== TAB 2 ==="),
               }) |
               border;
    });

    auto tab_3 = Renderer(Radiobox(&tab_3_entries, &tab_3_selected), [&] {
        return vbox({
                   text("=== TAB 3 ==="),
               }) |
               border;
    });

    auto tab_container = Container::Tab({tab_1, tab_2, tab_3}, &tab_selected);

    auto container = Container::Vertical({
        tab_toggle,
        tab_container,
    });

    // The data:

    // The basic input components:
    Component input_first_name = Input(&first_name, "first name");
    Component input_last_name  = Input(&last_name, "last name");

    // The password input component:
    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "password", password_option);

    // The phone number input component:
    // We are using `CatchEvent` to filter out non-digit characters.
    Component input_phone_number = Input(&phoneNumber, "phone number");
    input_phone_number |= CatchEvent([&](Event event) { return event.is_character() && !std::isdigit(event.character()[0]); });
    input_phone_number |= CatchEvent([&](Event event) { return event.is_character() && phoneNumber.size() > 10; });

    // The component tree:
    auto component = Container::Vertical({
        input_first_name,
        input_last_name,
        input_password,
        input_phone_number,
        tab_toggle,
        tab_container,
    });

    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
        return vbox({
                   hbox(text(" First name : "), input_first_name->Render()),       //
                   hbox(text(" Last name  : "), input_last_name->Render()),        //
                   hbox(text(" Password   : "), input_password->Render()),         //
                   hbox(text(" Phone num  : "), input_phone_number->Render()),     //
                   separator(),                                                    //
                   text("Hello               : " + first_name + " " + last_name),  //
                   text("Your password is    : " + password),                      //
                   text("Your phone number is: " + phoneNumber),                   //
                   text("Time                : " + timer),                         //
                   separator(),                                                    //
                   tab_toggle->Render(),                                           //
                   separator(),                                                    //
                   tab_container->Render(),                                        //
                   separator()                                                     //
               }) |
               border;
    });

    std::atomic<bool> running = true;
    auto screen               = ScreenInteractive::TerminalOutput();

    std::thread clock_thread([&running, &screen, &timer]() {
        while (running) {
            auto now             = std::chrono::system_clock::now();
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&now_time), "%H:%M:%S");
            timer = ss.str();
            screen.PostEvent(Event::Custom);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    screen.Loop(renderer);
    running = false;
    clock_thread.join();
    return 0;
}
*/
