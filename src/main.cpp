#include <SFML/Graphics.hpp>

int main(){
    int widthWindow = 800, heightWindow = 600;
    // Création d'une fenêtre "render"
    sf::RenderWindow window(sf::VideoMode(widthWindow, heightWindow), "AllSurvivor",
                            sf::Style::Default); // Pour un style personnalisé
    // Boucle principale avec gestion frame par frame
    while (window.isOpen())
    {
        // Gestion des évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Demande de fermeture (croix de la fenêtre, ALT+F4)
            if (event.type == sf::Event::Closed)
                window.close();
            // D'autres évènements peuvent être gérés ici
        }
        // Effacement de l'ancienne frame (framebuffer)
        window.clear(sf::Color::Black);
        // ... Ici on dessine les objets, sprites, textes, formes
        // Affiche la nouvelle frame à l'écran
        window.display();
    }
    return 0;
}