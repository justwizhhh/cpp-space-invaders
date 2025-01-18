
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window),
    middle_of_screen(0, 0),
    window_size(0, 0)
{
  srand(time(NULL));

  current_game_state = game_states::TITLE;

  middle_of_screen.set((float)window.getSize().x / 2, (float)window.getSize().y / 2);
  window_size.set(window.getView().getSize().x, window.getView().getSize().y);
}

Game::~Game()
{

}

bool Game::init()
{
  // Initialising gameplay sprites
  updateLives(max_lives);

  player_ship.initialiseSprite("Data/Images/SpaceShooterRedux/PNG/playerShip3_blue.png");
  player_ship.sprite.setScale(0.75, 0.75);

  for(auto & projectile : player_projectiles)
  {
    projectile.initialiseSprite("Data/Images/SpaceShooterRedux/PNG/Lasers/laserBlue04.png");
    projectile.setVisibility(false);
  }

  for (int i = 0; i < alien_area_width; i++)
  {
    for (int j = 0; j < alien_area_height; j++)
    {
      aliens[i][j].sprite.setScale(0.5, 0.5);
      switch (j)
      {
        default:
        case 0:
          aliens[i][j].initialiseSprite("Data/Images/Alien UFO pack/PNG/shipBeige_manned.png");
          break;
        case 1:
          aliens[i][j].initialiseSprite("Data/Images/Alien UFO pack/PNG/shipBlue_manned.png");
          break;
        case 2:
          aliens[i][j].initialiseSprite("Data/Images/Alien UFO pack/PNG/shipGreen_manned.png");
          break;
        case 3:
          aliens[i][j].initialiseSprite("Data/Images/Alien UFO pack/PNG/shipPink_manned.png");
          break;
        case 4:
          aliens[i][j].initialiseSprite("Data/Images/Alien UFO pack/PNG/shipYellow_manned.png");
          break;
      }
    }
  }

  // Initialising UI elements
  background.setFillColor(sf::Color(61, 25, 29, 255));
  background.setSize(sf::Vector2f(window_size.x, window_size.y));

  if (!main_font.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout << "Font not loaded correctly! \n";
    return false;
  }
  else
  {
    // -- Start menu --
    start_logo_text.setFont(main_font);
    start_logo_text.setScale(3, 3);
    start_logo_text.setPosition(205, 190);
    start_logo_text.setFillColor(sf::Color(255, 255, 255, 255));
    start_logo_text.setString("Space Invaders");

    start_button_text.setFont(main_font);
    start_button_text.setScale(1.4, 1.4);
    start_button_text.setPosition(490, 400);
    start_button_text.setFillColor(sf::Color(255, 255, 255, 255));
    start_button_text.setString("Start");

    change_alien_logo_text.setFont(main_font);
    change_alien_logo_text.setScale(1.4, 1.4);
    change_alien_logo_text.setPosition(300, 500);
    change_alien_logo_text.setFillColor(sf::Color(152, 152, 152, 255));
    change_alien_logo_text.setString("Change Alien Difficulty");

    change_alien_button_text.setFont(main_font);
    change_alien_button_text.setScale(1.4, 1.4);
    change_alien_button_text.setFillColor(sf::Color(152, 152, 152, 255));
    change_alien_button_text.setString("< Standard >");
    change_alien_button_text.setPosition(middle_of_screen.x - (change_alien_button_text.getGlobalBounds().width / 2), 550);

    // -- Main gameplay UI --
    score_header_text.setFont(main_font);
    score_header_text.setScale(1.2, 1.2);
    score_header_text.setPosition(40, 25);
    score_header_text.setFillColor(sf::Color(255, 255, 255, 255));
    score_header_text.setString("Score");

    score_text.setFont(main_font);
    score_text.setScale(1.6, 1.6);
    score_text.setPosition(40, 65);
    score_text.setFillColor(sf::Color(255, 255, 255, 255));
    score_text.setString("0");

    lives_header_text.setFont(main_font);
    lives_header_text.setScale(1.2, 1.2);
    lives_header_text.setPosition((float)window_size.x - 120, 25);
    lives_header_text.setFillColor(sf::Color(255, 255, 255, 255));
    lives_header_text.setString("Lives");

    lives_text.setFont(main_font);
    lives_text.setScale(1.6, 1.6);
    lives_text.setPosition((float)window_size.x - 55, 65);
    lives_text.setFillColor(sf::Color(255, 255, 255, 255));
    lives_text.setString("3");

    // -- Win screen UI --
    win_logo_text.setFont(main_font);
    win_logo_text.setScale(3, 3);
    win_logo_text.setPosition(170, 250);
    win_logo_text.setFillColor(sf::Color(255, 255, 255, 255));
    win_logo_text.setString("Congratulations!");

    // -- Lose screen UI --
    gameover_logo_text.setFont(main_font);
    gameover_logo_text.setScale(3, 3);
    gameover_logo_text.setPosition(250, 240);
    gameover_logo_text.setFillColor(sf::Color(255, 255, 255, 255));
    gameover_logo_text.setString("Game Over!");

    restart_button_text.setFont(main_font);
    restart_button_text.setScale(1.4, 1.4);
    restart_button_text.setPosition(215, 425);
    restart_button_text.setFillColor(sf::Color(255, 255, 255, 255));
    restart_button_text.setString("Press enter to restart the game");

    return true;
  }
}

void Game::keyPressed(sf::Event event)
{
  switch(current_game_state)
  {
    case TITLE:
      // Start menu
      changeMenuState(event.key.code == sf::Keyboard::W ? 0 : event.key.code == sf::Keyboard::S ? 1 : current_menu_state);
      if (current_menu_state == 0)
      {
        start_button_text.setFillColor(sf::Color(255, 255, 255, 255));
        change_alien_logo_text.setFillColor(sf::Color(152, 152, 152, 255));

        if (event.key.code == sf::Keyboard::Enter)
        {
          startGame();
          changeState(GAMEPLAY);
        }
      }
      else
      {
        start_button_text.setFillColor(sf::Color(152, 152, 152, 255));
        change_alien_logo_text.setFillColor(sf::Color(255, 255, 255, 255));

        // Changing the aliens' movement patterns
        alien_menu_state += event.key.code == sf::Keyboard::A ? -1 : event.key.code == sf::Keyboard::D ? 1 : 0;
        if (alien_menu_state < 0)
        {
          alien_menu_state = 3;
        }
        else if (alien_menu_state > 3)
        {
          alien_menu_state = 0;
        }

        change_alien_button_text.setString(alien_state_names[alien_menu_state]);
        change_alien_button_text.setPosition(
          middle_of_screen.x - (change_alien_button_text.getGlobalBounds().width / 2),
          change_alien_button_text.getPosition().y);
      }
      break;

    case GAMEPLAY:
      // Main gameplay logic
      if (event.key.code == sf::Keyboard::A)
      {
        setLeftInput(true);
      }
      if (event.key.code == sf::Keyboard::D)
      {
        setRightInput(true);
      }

      // Shooting
      if (event.key.code == sf::Keyboard::Space)
      {
        spawnProjectile();
      }

      break;

    case WIN:
    case GAME_OVER:
      // Game restart
      if (event.key.code == sf::Keyboard::Enter)
        restartGame();

      break;
  }
}

void Game::keyReleased(sf::Event event)
{
  // Movement inputs are only processed during the 'GAMEPLAY' state, no need for switch statement
  if (current_game_state == GAMEPLAY)
  {
    // Main gameplay logic
    if (event.key.code == sf::Keyboard::A)
    {
      setLeftInput(false);
    }
    if (event.key.code == sf::Keyboard::D)
    {
      setRightInput(false);
    }
  }
}

void Game::changeState(Game::game_states new_state)
{
  current_game_state = new_state;
}

void Game::startGame()
{
  player_ship.position.set(
  middle_of_screen.x - (player_ship.getSprite().getGlobalBounds().width / 2),
  middle_of_screen.y + 250);

  // Positioning all of the aliens
  sf::Texture base_alien_texture;
  base_alien_texture.loadFromFile("Data/Images/Alien UFO pack/PNG/shipBeige.png");
  Vector2 start_position(
    middle_of_screen.x -
      (((float)alien_area_width / 2) * aliens[0][0].getSprite().getGlobalBounds().width),
    middle_of_screen.y -
      (((float)alien_area_height / 2) * aliens[0][0].getSprite().getGlobalBounds().height));

  for (int i = 0; i < alien_area_width; i++)
  {
    for (int j = 0; j < alien_area_height; j++)
    {
      GameObject& alien = aliens[i][j];
      Vector2& start_pos = alien_start_pos[i][j];
      alien.setVisibility(true);

      start_pos.set(
        start_position.x +
          ((float)i * (aliens[0][0].sprite.getGlobalBounds().width + (float)alien_area_margin)
           - ((float)alien_area_margin) * 2),
        start_position.y +
          ((float)j * aliens[0][0].sprite.getGlobalBounds().height)
          + ((float)j * (float)alien_area_margin)
          - (float)alien_area_offset);

      alien.setPosition(start_pos);

      alien.updatePos();
      continue;
    }
  }

  // Setting up alien movement behaviour
  defeated_aliens = 0;
  setAlienDirection(((std::rand() % 2) * 2) - 1);
  setAlienSpeed(0);
  switch (alien_menu_state)
  {
    case 0:
    default:
      current_alien_state = STATIONARY;
      break;
    case 1:
      current_alien_state = GRAVITY;
      break;
    case 2:
      current_alien_state = QUADRATIC;
      break;
    case 3:
      current_alien_state = SINE;
      break;
  }
}

void Game::updateScore(int value)
{
  score += value;
  score_text.setString(std::to_string(score));
}

void Game::updateLives(int value)
{
  lives += value;
  lives_text.setString(std::to_string(lives));
}

void Game::restartGame()
{
  setLeftInput(false);
  setRightInput(false);

  defeated_aliens = 0;
  for (auto & proj : player_projectiles)
  {
    proj.setVisibility(false);
  }

  updateScore(-getScore());
  updateLives(max_lives - getLives());

  startGame();
  changeState(TITLE);
}

void Game::movePlayer(float dt)
{
  setPlayerDirection(isLeftInput() && isRightInput() ? 0 : isLeftInput() ? -1 : isRightInput() ? 1 : 0);

  float left_side = 0;
  float right_side = (float)window_size.x - player_ship.getSprite().getGlobalBounds().width;

  player_ship.position.x += player_max_speed * (float)getPlayerDirection() * dt;
  player_ship.position.x = std::max(left_side, std::min(player_ship.position.x, right_side));
}

void Game::spawnProjectile()
{
  // Find a valid visible projectile to shoot from the array
  for (auto & proj : player_projectiles)
  {
    if (!proj.visibility)
    {
      proj.position.x = player_ship.getPosition().x
                        + (player_ship.getSprite().getGlobalBounds().width / 2)
                        - (proj.getSprite().getGlobalBounds().width / 2);
      proj.position.y = player_ship.position.y - proj.getSprite().getGlobalBounds().height;
      proj.updatePos();

      proj.setVisibility(true);

      // Get rid of some score points, as punishment for spamming the shoot button
      if (getScore() < 20)
      {
        setScore(0);
      }
      else
      {
        score -= 20;
        score_text.setString(std::to_string(getScore()));
      }
      break;
    }
  }
}

void Game::moveProjectile(GameObject& projectile, float dt)
{
  if (projectile.getPosition().y > 0)
  {
    projectile.position.y -= player_max_projectile_speed * dt;

    // Collision detection with aliens only happens when a projectile is actually moving around
    int rows = sizeof(aliens) / sizeof(aliens[0]);
    int columns = sizeof(aliens[0]) / sizeof(aliens[0][0]);
  }
  else
  {
    projectile.setVisibility(false);
  }
}

void Game::moveAliens(float dt)
{
  int left_side = alien_screen_margin;
  int right_side = window_size.x - aliens[0][0].getSprite().getGlobalBounds().width - alien_screen_margin;

  for (int i = 0; i < alien_area_width; i++)
  {
    for (int j = 0; j < alien_area_height; j++)
    {
      GameObject& alien = aliens[i][j];
      Vector2& start_pos = alien_start_pos[i][j];

      switch(current_alien_state)
      {
        case STATIONARY:
        default:
          alien.setPosition(Vector2(alien.getPosition().x + (alien_max_speed * dt) * getAlienDirection(), alien.getPosition().y));
          break;

        case GRAVITY:
          alien.setPosition(
            Vector2(
              alien.getPosition().x + (alien_max_speed * dt) * getAlienDirection(),
              alien.getPosition().y + getAlienSpeed()));
          setAlienSpeed(alien_speed + alien_gravity_multiplier);
          break;

        case QUADRATIC:
          alien.setPosition(
            Vector2(
              alien.getPosition().x + (alien_max_speed * dt) * getAlienDirection(),
              start_pos.y - (std::pow(alien.getPosition().x - middle_of_screen.x, 2) / alien_quadratic_length)));
          break;

        case SINE:
          alien.setPosition(
            Vector2(
              alien.getPosition().x + (alien_max_speed * dt) * getAlienDirection(),
              start_pos.y + (std::sin(alien.getPosition().x / alien_sine_wavelength) * alien_sine_amplitude)));
          break;
      }

      alien.updatePos();

      if (alien.getPosition().x >= right_side || alien.getPosition().x <= left_side)
      {
        alien_direction *= -1;
      }
    }
  }
}

void Game::changeMenuState(int dir)
{
  current_menu_state = dir;
}

void Game::update(float dt)
{
  if (current_game_state == GAMEPLAY)
  {
    // Moving/updating main gameplay objects
    movePlayer(dt);
    player_ship.updatePos();

    moveAliens(dt);

    for (auto & proj : player_projectiles)
    {
      if (proj.getVisibility())
      {
        moveProjectile(proj, dt);
        proj.updatePos();

        // Projectiles colliding with aliens
        for (int i = 0; i < alien_area_width; i++)
        {
          for (int j = 0; j < alien_area_height; j++)
          {
            GameObject& alien = aliens[i][j];
            if (alien.visibility)
            {
              Vector2 alienCol = proj.collision(alien);
              if (!(alienCol.x == 0 && alienCol.y == 0))
              {
                alien.setVisibility(false);
                proj.setVisibility(false);

                defeated_aliens++;
                updateScore(100);

                break;
              }
            }
          }
        }
      }
    }

    // Lose condition (colliding with an alien
    for (int i = 0; i < alien_area_width; i++)
    {
      for (int j = 0; j < alien_area_height; j++)
      {
        GameObject& alien = aliens[i][j];
        if (alien.getVisibility())
        {
          Vector2 player_col = alien.collision(player_ship);
          if (!(player_col.x == 0 && player_col.y == 0))
          {
            if (lives > 1)
            {
              updateLives(-1);
              startGame();
            }
            else
            {
              changeState(GAME_OVER);
            }
          }
        }
      }
    }

    // Win condition (all aliens have been defeated)
    if (defeated_aliens >= (alien_area_width * alien_area_height))
    {
      changeState(WIN);
    }
  }
}

void Game::render()
{
  // Rendering all UI assets
  window.draw(background);
  switch(current_game_state)
  {
    case TITLE:
      window.draw(start_logo_text);
      window.draw(start_button_text);
      window.draw(change_alien_logo_text);
      window.draw(change_alien_button_text);

      break;

    case GAMEPLAY:
      // Game elements
      window.draw(player_ship.getSprite());
      for(auto & projectile : player_projectiles)
      {
        if (projectile.getVisibility())
          window.draw(projectile.getSprite());
      }

      for (int i = 0; i < alien_area_width; i++)
      {
        for (int j = 0; j < alien_area_height; j++)
        {
          if (aliens[i][j].visibility)
            window.draw(aliens[i][j].getSprite());
        }
      }

      // UI elements
      window.draw(score_header_text);
      window.draw(score_text);

      window.draw(lives_header_text);
      window.draw(lives_text);

      break;

    case WIN:
      window.draw(win_logo_text);
      window.draw(restart_button_text);

      window.draw(score_header_text);
      window.draw(score_text);
      break;

    case GAME_OVER:
      window.draw(gameover_logo_text);
      window.draw(restart_button_text);

      window.draw(score_header_text);
      window.draw(score_text);
      break;
  }
}

void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);
}

// ------- Getters and setters -------

bool Game::isLeftInput() const
{
  return left_input;
}
void Game::setLeftInput(bool leftInput)
{
  left_input = leftInput;
}
bool Game::isRightInput() const
{
  return right_input;
}
void Game::setRightInput(bool rightInput)
{
  right_input = rightInput;
}
int Game::getScore() const
{
  return score;
}
void Game::setScore(int score)
{
  Game::score = score;
}
int Game::getLives() const
{
  return lives;
}
void Game::setLives(int lives)
{
  Game::lives = lives;
}
int Game::getPlayerSpeed() const
{
  return player_speed;
}
void Game::setPlayerSpeed(int playerSpeed)
{
  player_speed = playerSpeed;
}
int Game::getPlayerDirection() const
{
  return player_direction;
}
void Game::setPlayerDirection(int playerDirection)
{
  player_direction = playerDirection;
}
int Game::getPlayerProjectileSpeed() const
{
  return player_projectile_speed;
}
void Game::setPlayerProjectileSpeed(int playerProjectileSpeed)
{
  player_projectile_speed = playerProjectileSpeed;
}
float Game::getAlienSpeed() const
{
  return alien_speed;
}
void Game::setAlienSpeed(float alienSpeed)
{
  alien_speed = alienSpeed;
}
int Game::getAlienDirection() const
{
  return alien_direction;
}
void Game::setAlienDirection(int alienDirection)
{
  alien_direction = alienDirection;
}
