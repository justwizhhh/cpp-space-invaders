
#ifndef SPACEINVADERS_GAME_H
#define SPACEINVADERS_GAME_H

#include <SFML/Graphics.hpp>
#include "Vector2.h"
#include "GameObject.h"

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();

  // Gameplay toggles
  int max_lives = 3;

  int player_max_speed = 475;
  int player_max_projectile_speed = 600;

  int alien_area_width = 6;
  int alien_area_height = 4;
  int alien_area_offset = 120;
  int alien_area_margin = 10;

  int alien_max_speed = 90;
  int alien_screen_margin = 40;

  float alien_gravity_multiplier = 0.0001;

  int alien_quadratic_length = 1500;

  int alien_sine_amplitude = 30;
  int alien_sine_wavelength = 50;

  // Gameplay objects
  Vector2 middle_of_screen;
  Vector2 window_size;
  sf::Clock clock;

  GameObject player_ship;
  GameObject player_projectiles[8];

  GameObject aliens[16][16];
  enum alien_states
  {
    STATIONARY,
    GRAVITY,
    QUADRATIC,
    SINE
  };
  alien_states current_alien_state;
  int alien_menu_state;

  bool init();

  void startGame();
  void updateScore(int value);
  void updateLives(int value);
  void restartGame();

  void movePlayer(float dt);
  void spawnProjectile();
  void moveProjectile(GameObject& projectile, float dt);
  void moveAliens(float dt);

  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);

  enum game_states
  {
    TITLE,
    GAMEPLAY,
    WIN,
    GAME_OVER
  };
  game_states current_game_state;
  void changeState(game_states new_state);

  void changeMenuState(int dir);

  bool isLeftInput() const;
  void setLeftInput(bool leftInput);

  bool isRightInput() const;
  void setRightInput(bool rightInput);

  int getScore() const;
  void setScore(int score);

  int getLives() const;
  void setLives(int lives);

  int getPlayerSpeed() const;
  void setPlayerSpeed(int playerSpeed);

  int getPlayerDirection() const;
  void setPlayerDirection(int playerDirection);

  int getPlayerProjectileSpeed() const;
  void setPlayerProjectileSpeed(int playerProjectileSpeed);

  int getAlienDirection() const;
  void setAlienDirection(int alienDirection);

  float getAlienSpeed() const;
  void setAlienSpeed(float alienSpeed);


 private:
  sf::RenderWindow& window;

  // Inputs
  bool left_input = 0;
  bool right_input = 0;

  // Gameplay variables
  int score = 0;
  int lives = 0;

  int player_speed = 0;
  int player_direction = 0;
  int player_projectile_speed = 0;

  int alien_direction = 0;
  Vector2 alien_start_pos[16][16];

  float alien_speed = 0;

  int defeated_aliens = 0;

  // UI elements
  int current_menu_state = 0;

  sf::RectangleShape background;

  sf::Font main_font;

  sf::Text start_logo_text;
  sf::Text start_button_text;
  sf::Text change_alien_logo_text;
  sf::Text change_alien_button_text;
  std::string alien_state_names[4] = {
    "< Standard >",
    "< Gravity >",
    "< Quadratic Curve >",
    "< Sine Wave >"
  };

  sf::Text score_header_text;
  sf::Text score_text;
  sf::Text lives_header_text;
  sf::Text lives_text;

  sf::Text win_logo_text;
  sf::Text gameover_logo_text;
  sf::Text restart_button_text;
};

#endif // SPACEINVADERS_GAME_H
