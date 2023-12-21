#pragma once
#include "Entity.h"
#include "EntityManager.h"
#include <SFML/Graphics.hpp>
struct WindowConfig { long long w_width, w_height;int w_frames, screen_mode; };
struct FontConfig { std::string font;int size, R, G, B; };
struct PlayerConfig  { int SR, CR, FR, FG, FB, OR, OG,OB, OT, V;float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI ;float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG,OB, OT, V ,L ;float S; };

class Game
{
	sf::RenderWindow       m_window;						// the window we will draw to;
	EntityManager          m_entities;			   		   // vector of entities to maintain
	sf::Font               m_font;						  // the font we will use to draw
	sf::Text               m_text;                       // the score text to be drawn to the screen
	WindowConfig           m_windowconfig;
	FontConfig             m_fontconfig;
	PlayerConfig           m_playerconfig;
	EnemyConfig            m_enemyconfig;
	BulletConfig           m_bulletconfig;
	int                    m_score = 0;
	int                    m_currentFrame = 0;
	int                    m_lastEnemySpawnTime = 0;
	bool                   m_pused = false; // whether we update game logic
	bool                   m_running = true; // whether bthe game is running 


	std::shared_ptr<Entity> m_player;

	void init(const std::string& config);         // initialize the gameState with a config file
	void setPaused(bool paused);                 // puse the game
	void sMovement();                           // system : entity position / movement update
	void sUserInput();                         // system : user input
	void sLifespan();                         // system : lifespan
	void sRender();                          // system : Render / Drawing
	void sEnemySpawner();                   // system : spawns enmeies
	void sCollision();                     // system : collisions

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void spawnSpetialWeapon(std::shared_ptr<Entity> entity);

public:
	Game(const std::string& config); // takes game config
	void run();
	
};




/*



####

Player specification:
shape radius, collision radius, spped, fill color,outline color,
outline thickness,shape verticies

enemy specification:
shape radius, collision radius,min/max speed, fill color,outline color,
outline thickness, min/max vertices/ small lifespan,spawn interval,

bullet Specification:
shape radius, collision radius, spped, fill color,outline color,
outline thickness,shape verticies,lifespan








##
*/