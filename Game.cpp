#include "Game.h"
#include <iostream>
#include <fstream>
#include<sstream>


Game::Game(const std::string& config) { init(config); }









void Game::init(const std::string& path){
	// read in config file here
	std::ifstream fin("config.txt");
	std::string line;
	while (std::getline(fin, line)) {
		std::istringstream iss(line);
		std::string type;
		iss >> type;
		if (type == "Window") {
			iss >> m_windowconfig.w_width >> m_windowconfig.w_height >> m_windowconfig.w_frames >> m_windowconfig.screen_mode;
		}
		else if (type == "Font") {
			iss >> m_fontconfig.font >> m_fontconfig.size >> m_fontconfig.R >> m_fontconfig.G >> m_fontconfig.B;
		}
		else if (type == "Player" ) {
			iss >> m_playerconfig.SR >> m_playerconfig.CR >> m_playerconfig.S >> m_playerconfig.FR >> m_playerconfig.FG >> m_playerconfig.FB >> m_playerconfig.OR
				>> m_playerconfig.OG >> m_playerconfig.OB >> m_playerconfig.OT >> m_playerconfig.V ;
		} 
	
		else if (type == "Enemy") {
			iss >> m_enemyconfig.SR >> m_enemyconfig.CR >> m_enemyconfig.SMIN >> m_enemyconfig.SMAX >> m_enemyconfig.OR >> m_enemyconfig.OG >> m_enemyconfig.OB >> m_enemyconfig.OT
				>> m_enemyconfig.VMIN >> m_enemyconfig.VMAX >> m_enemyconfig.L >> m_enemyconfig.SI ;

		}
		else if(type == "Bullet"){
			iss >> m_bulletconfig.SR >> m_bulletconfig.CR >> m_bulletconfig.S >> m_bulletconfig.FR >> m_bulletconfig.FG >> m_bulletconfig.FB >> m_bulletconfig.OR >> m_bulletconfig.OG
				>> m_bulletconfig.OB >> m_bulletconfig.OT >> m_bulletconfig.V >> m_bulletconfig.L;
		}
	}

	fin.close();
	m_window.create(sf::VideoMode(m_windowconfig.w_width, m_windowconfig.w_height), "WE'RE HERE!");
	m_window.setFramerateLimit(m_windowconfig.w_frames);

	spawnPlayer();
}


void Game::run() {

// todo: add pause functionality in here 
while (m_running) {
	m_entities.update();
		////demo fro pausing
		//if(!m_pused){
		//
		//}
		
	sUserInput();
	sEnemySpawner();
	sMovement();
	sCollision();
	sRender();

	// incement thecurrent frame
	// may need to be moved when pause implemented

	m_currentFrame++;
}
	
}
void Game::spawnPlayer(){
	// todo: finish adding all properties of the player with the currect values from config

	auto entity = m_entities.addEntity("player");


	// give this entity a transofrm so it spawns at (200,200) and velocity (1,1) and angle 0
	float mx = m_window.getSize().x / 2.0f;
	float my = m_window.getSize().y / 2.0f;

	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(1.0f, 1.0f), 0.0f);

	// the entity's shape will have radius 32 ,8sides dark gry fill , red outline of thik 4
	entity->cShape = std::make_shared<CShape>(m_playerconfig.SR, m_playerconfig.V,sf::Color(m_playerconfig.FR,
		m_playerconfig.FG, m_playerconfig.FB), sf::Color(m_playerconfig.OR, m_playerconfig.OG, m_playerconfig.OB), m_playerconfig.OT);

	//Add an input compenent to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	// since we want this entity to be our player, set our game's player variable to be this entity
	// this goes slightly against the entitymanager paragigm, but we use the player so much it's worth it

	m_player = entity;

}

void Game::spawnEnemy(){
	//todo make sure the enemy is spawned properly with the m_enemyconfig variables
	/// the enemy must be spawned compelitly within the bounds of the window 
	 

	auto entity = m_entities.addEntity("enemy");


	// give this entity a transofrm so it spawns at (200,200) and velocity (1,1) and angle 0
	float ex = rand() % m_window.getSize().x +0.0f;
	float ey =rand()% m_window.getSize().y +0.0f;

	entity->cTransform = std::make_shared<CTransform>(Vec2(200, 200), Vec2(1.0f, 1.0f), 0.0f);

	// the entity's shape will have radius 32 ,8sides dark gry fill , red outline of thik 4
	entity->cShape = std::make_shared<CShape>(m_enemyconfig.SR,4, sf::Color(60, 255, 10), sf::Color(m_enemyconfig.OR, m_enemyconfig.OG, m_enemyconfig.OB), m_enemyconfig.OT);

	//Add an input compenent to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();
	//record when the most recent enemy was spowned
	m_lastEnemySpawnTime = m_currentFrame;


}
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos){
// todo spawning of bullet wich travels towerd target /bullet speed is scaler /velocity formila in notes
	auto bullet = m_entities.addEntity("bullet");

	bullet->cTransform = std::make_shared<CTransform>(mousePos, Vec2(0, 0), 0);
	bullet->cShape = std::make_shared<CShape>(m_bulletconfig.SR,m_bulletconfig.V, sf::Color(m_bulletconfig.FR, m_bulletconfig.FG, m_bulletconfig.FB),
		sf::Color(m_bulletconfig.OR, m_bulletconfig.OG, m_bulletconfig.OB), m_bulletconfig.OT);
}



void Game::sLifespan(){}
void Game::sEnemySpawner(){
	// todo enemy spawning using currentframe - m_lastenemyspawntime ts determin
	// how long has been since the last enemy spawwn
	spawnEnemy();
}
void Game::sCollision(){
	//for (auto b : m_entities.getEntities("bullet")) {
	//	for (auto e : m_entities.getEntities("enemy")) {
	//		// needsto check for collision
	//		if (false) {
	//			b->destroy();e->destroy();
	//		}
	//	}
//}
}

//respawn the player in the middle of the screen


void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity){


}
void Game::spawnSpetialWeapon(std::shared_ptr<Entity> entity){}

void Game::sRender(){

	m_window.clear();

	for (auto& e : m_entities.getEntities()) {
		
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle += 1.0f);
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

		e->cShape->circle.setRotation(e->cTransform->angle);
		m_window.draw(e->cShape->circle);
	 }
	//std::cout << m_player->cShape->circle.getPosition().x << "---"<< m_player->cShape->circle.getPosition().x<<std::endl;
	m_window.display();
}
void Game::setPaused(bool paused){}
void Game::sMovement(){
	// implement player mvt
	m_player->cTransform->velocity = { 0,0 };

	if (m_player->cInput->up) {
		m_player->cTransform->velocity.y = -5;
	}
	if (m_player->cInput->down) {
		m_player->cTransform->velocity.y = 5;
	}
	if (m_player->cInput->right) {
		m_player->cTransform->velocity.x = 5;
	}
	if (m_player->cInput->left) {
		m_player->cTransform->velocity.x = -5;
	}

	// sample mvt speed update 

	m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
}

void Game::sUserInput(){
// todo handle user input here / only player's input component variables here /no player's mvt logic here/mvt system will read vars

	sf::Event event;
	while (m_window.pollEvent(event)){
		// this event triggers when the window is closed
		if (event.type == sf::Event::Closed) {
			m_running = false;
		}
		//this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::Up:
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::Down:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::Left:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::Right:
				m_player->cInput->right = true;
				break;  
			
			default: break;
			}
		}
		// this event is triggered when a key is released
		if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code) {
			case sf::Keyboard::Up:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::Down:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::Left:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::Right:
				m_player->cInput->right = false;
				break;

			default: break;
			}
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				std::cout << "right mouse button clicked at (" << event.mouseButton.x << " ," << event.mouseButton.y << " )\n";
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
				// cll spawn bullet here

			}
		}
		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Right) {
				std::cout << "Left mouse button clicked at (" << event.mouseButton.x << " ," << event.mouseButton.y << " )\n";
				// cll spawnspetialweapon here

			}
		}
	}
}