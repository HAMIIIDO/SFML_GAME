#include "Game.h"


Game::Game(const std::string& config) { init(config); }









void Game::init(const std::string& path){
	// read in config file here
	std::ifstream fin(path);
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
	// 



	// speed 
	int diff_speed = 1 + m_enemyconfig.SMAX - 2 * m_enemyconfig.SMIN;

	std::random_device rd;
	std::mt19937 gen(rd());

	int lowerBound = -1;
	int upperBound = 1;
	std::uniform_int_distribution<int> distribution(lowerBound, upperBound);
	int randomNum = distribution(gen);
	int valid_num{1};
	(randomNum == 1 || randomNum == -1) ? (valid_num = randomNum) : NULL;

	float speed_x = m_enemyconfig.SMIN + (rand() % diff_speed );
	float speed_y = m_enemyconfig.SMIN + (rand() % diff_speed) ;
	//srand(time(NULL));

	
	//position
	int diff_x = 1 + m_window.getSize().x - 2*m_enemyconfig.SR;
	int diff_y = 1 + m_window.getSize().y - 2 * m_enemyconfig.SR;
	float ex = m_enemyconfig.SR +( rand() % diff_x);
	float ey = m_enemyconfig.SR + (rand() % diff_y);

	// veticies
	int diff_vertex = 1 + m_enemyconfig.VMAX - m_enemyconfig.VMIN;
	int vertex = m_enemyconfig.VMIN + (rand() % diff_vertex);
	// color
	int R = rand() % 256;
	int G = rand() % 256;
	int B = rand() % 256;



	auto entity = m_entities.addEntity("enemy");
	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(speed_x * valid_num, speed_y * valid_num), 0.0f);
	entity->cShape = std::make_shared<CShape>(m_enemyconfig.SR,vertex, sf::Color(R,G, B),
		sf::Color(m_enemyconfig.OR, m_enemyconfig.OG, m_enemyconfig.OB), m_enemyconfig.OT);
	m_lastEnemySpawnTime = m_currentFrame;

		/*std::cout << "---x" << entity->cTransform->pos.x << "\n";
		std::cout <<"height: " << entity->cShape->circle.getLocalBounds().height << "\n";
		std::cout << "---y" << entity->cTransform->pos.y << "\n";
		std::cout <<"width: " << entity->cShape->circle.getLocalBounds().width << "\n";
		std::cout << "top : " << entity->cShape->circle.getLocalBounds().top<< "\n";
		std::cout << "left: " << entity->cShape->circle.getLocalBounds().left << "\n";


*/

}
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos){
// todo spawning of bullet wich travels towerd target /bullet speed is scaler /velocity formila in notes
	auto bx = entity->cTransform->pos.x;
	auto by = entity->cTransform->pos.y;
	auto bullet = m_entities.addEntity("bullet");
	bullet->cTransform = std::make_shared<CTransform>(Vec2(bx, by), Vec2(1, 1), 0);
	bullet->cShape = std::make_shared<CShape>(m_bulletconfig.SR,m_bulletconfig.V, sf::Color(m_bulletconfig.FR, m_bulletconfig.FG, m_bulletconfig.FB,255),
		sf::Color(m_bulletconfig.OR, m_bulletconfig.OG, m_bulletconfig.OB,255), m_bulletconfig.OT);

	Vec2 D(mousePos.x - bx, mousePos.y - by); // vector bztween mouse and player
	float dist = std::sqrtf(D.x * D.x + D.y * D.y);
	Vec2 norm_vec(D.x / dist, D.y / dist); // normalized version
	bullet->cTransform->velocity = {norm_vec.x* m_bulletconfig.S ,norm_vec.y* m_bulletconfig.S };
}



void Game::sLifespan(){}
void Game::sEnemySpawner(){
	if (m_currentFrame - m_lastEnemySpawnTime == m_enemyconfig.SI) {
	spawnEnemy();
	}
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
void Game::setPaused(bool paused){}
void Game::sMovement() {
	for (auto& e : m_entities.getEntities("enemy")) {

		if (m_windowconfig.w_width < (e->cShape->circle.getPosition().x + m_enemyconfig.SR) 
			|| e->cShape->circle.getPosition().x - m_enemyconfig.SR <0) {
			e->cTransform->velocity.x = -e->cTransform->velocity.x;
			std::cout << e->cTransform->velocity.x + m_enemyconfig.SR;
		}
		if (m_windowconfig.w_height < (e->cShape->circle.getPosition().y + m_enemyconfig.SR)
			|| e->cShape->circle.getPosition().y - m_enemyconfig.SR < 0) {
			e->cTransform->velocity.y = -e->cTransform->velocity.y;
			std::cout << e->cTransform->velocity.x + m_enemyconfig.SR;
		}
		
	}



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
	for (auto& b : m_entities.getEntities("bullet")) {

			sf::Uint8 alpha = b->cShape->circle.getFillColor().a;
			if (static_cast<int>(alpha )> 0) {
				alpha -= static_cast<sf::Uint8>(1);
				b->cShape->circle.setFillColor(sf::Color(
					m_bulletconfig.FR, m_bulletconfig.FG, m_bulletconfig.FB, alpha - m_bulletconfig.L/30));
				b->cShape->circle.setOutlineColor(sf::Color(
					m_bulletconfig.OR, m_bulletconfig.OG, m_bulletconfig.OB, alpha - m_bulletconfig.L/30));
			if(static_cast<int>(alpha) <6) {
				b->cShape->circle.setFillColor(sf::Color(0,0,0));
				b->cShape->circle.setOutlineColor(sf::Color(0,0,0));
				b->destroy();
			}
			}else{
			}
	}
	       
	for (auto& e : m_entities.getEntities()) {
			e->cTransform->pos.x += e->cTransform->velocity.x;
			e->cTransform->pos.y += e->cTransform->velocity.y;
	}


}

void Game::sRender(){

	m_window.clear();

	for (auto& e : m_entities.getEntities()) {
		
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle += 1.0f);
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		m_window.draw(e->cShape->circle);
	 }
	m_window.display();
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