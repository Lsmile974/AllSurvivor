#pragma once

#include "menu_export.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

namespace Menu
{

struct MENU_EXPORT Button
{
    sf::RectangleShape rectangle;
    sf::Text text;
    Button(std::string initText, sf::Font& font, int fontSize);
    Button(std::string initText, sf::Font& font, int fontSize, int width, int height);
    Button(std::string initText, sf::Font& font, int fontSize, int width, int height, sf::Color fillColor,
           sf::Color outlineColor, sf::Color fontColor);
    void setPosition(float x, float y);
    float getWidth();
    float getHeight();
    void changeFillColor(sf::Color newColor);
    void changeText(std::string newText);
    void draw(sf::RenderWindow& window);
};

void center_horizontally(const auto& parent, auto& child)
{
    const auto parentBoundingBox = parent.getLocalBounds();
    const sf::Vector2f parentLocalCenter{parentBoundingBox.left + parentBoundingBox.width * 0.5f,
                                         parentBoundingBox.top + parentBoundingBox.height * 0.5f};

    const sf::Vector2f parentGlobalCenter = parent.getTransform().transformPoint(parentLocalCenter);

    const auto childBoundingBox = child.getLocalBounds();
    const auto childOrigin = child.getOrigin();
    child.setOrigin(childBoundingBox.left + childBoundingBox.width * 0.5f, childOrigin.y);

    const auto childPosition = child.getPosition();
    child.setPosition(parentGlobalCenter.x, childPosition.y);
};

void center_vertically(const auto& parent, auto& child)
{
    const auto parentBoundingBox = parent.getLocalBounds();
    const sf::Vector2f parentLocalCenter{parentBoundingBox.left + parentBoundingBox.width * 0.5f,
                                         parentBoundingBox.top + parentBoundingBox.height * 0.5f};

    const sf::Vector2f parentGlobalCenter = parent.getTransform().transformPoint(parentLocalCenter);

    const auto childBoundingBox = child.getLocalBounds();
    const auto childOrigin = child.getOrigin();
    child.setOrigin(childOrigin.x, childBoundingBox.top + childBoundingBox.height * 0.5f);

    const auto childPosition = child.getPosition();
    child.setPosition(childPosition.x, parentGlobalCenter.y);
};
void center(const auto& parent, auto& child)
{
    center_horizontally(parent, child);
    center_vertically(parent, child);
};
MENU_EXPORT int run();

}

