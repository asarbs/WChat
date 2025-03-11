#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML + ImGui");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

    bool show_demo_window = true;
    sf::Clock deltaClock;

    char input_text[256] = "";  // Tablica na tekst wprowadzony przez użytkownika

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        window.clear();

        // Tworzymy pole tekstowe
        ImGui::InputText("Input", input_text, sizeof(input_text));

        // Wyświetlamy tekst, który użytkownik wpisał
        ImGui::Text("You typed: %s", input_text);

        ImGui::SFML::Render(window);
        window.display();
    }

    // Cleanup
    ImGui::SFML::Shutdown();
    return 0;
}
