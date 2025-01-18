
#ifndef SPACEINVADERS_GAMEOBJECT_H
#define SPACEINVADERS_GAMEOBJECT_H

#include "Vector2.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

class GameObject
{
 public:
  GameObject();
  ~GameObject();

  Vector2 position;
  const Vector2& getPosition() const;
  void setPosition(const Vector2& position);

  sf::Sprite sprite;
  const sf::Sprite& getSprite() const;
  void setSprite(const sf::Sprite& sprite);

  sf::Texture texture;

  bool initialiseSprite(const std::string& file_path);
  void updatePos();
  Vector2 collision(const GameObject& other_object) const;

  Vector2 getMidPoint() const;

  bool visibility = false;
  bool getVisibility() const;
  void setVisibility(bool toggle);
};

#endif // SPACEINVADERS_GAMEOBJECT_H
