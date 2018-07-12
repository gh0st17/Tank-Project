#include <SFML/Graphics.hpp>
#include <Box2D.h>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <math.h>
#include <random>
#include <vector>
#include <ctime>
#include "world.hpp"
#include "Centauro120.hpp"
#include "Ural.hpp"
#include "Vaz.hpp"

using namespace sf;

int main() {
	initSprites();
	ContextSettings settings;
	settings.antialiasingLevel = 4;
	settings.depthBits = 8;
	RenderWindow window(sf::VideoMode(1200, 720, 8), "Tank Project v1.5.2a | Ghost-17", Style::Close, settings);
	Image icon; icon.loadFromFile("images/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	World m_world(&window);
	m_world.generateMap(true);

	window.setFramerateLimit(70);

	Hud hud = Hud(&window);
	MyContact *contactListener = new MyContact();
	player = shared_ptr<Tank>(new Vaz(m_world.getWorld(), &window, hud, contactListener));
	m_world.m_world->SetContactListener(contactListener);

	view.setSize(ZOOM_FACTOR * 1200.f, ZOOM_FACTOR * 720.f);

	DebugDrawer dd(m_world.getWorld(), &window);
	bool debug = 0;

	float time;
	Clock clock;

	while (window.isOpen())
	{
		window.clear();
		time = (float)clock.getElapsedTime().asMicroseconds();

		sf::Event e;
		while (window.pollEvent(e)) if (e.type == Event::Closed) window.close();
		if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();

		view.setCenter(player->getChassisOffsetView());
		window.setView(view);
		m_world.drawBackground();

		if (Keyboard::isKeyPressed(Keyboard::W)){
			player->right();
			if (Keyboard::isKeyPressed(Keyboard::LShift)) player->n20();
			else player->stop_n20();
		}
		else if (Keyboard::isKeyPressed(Keyboard::S)) player->left();
		else if (Keyboard::isKeyPressed(Keyboard::Space)) player->handBreak();
		else player->stop();

		if (Keyboard::isKeyPressed(Keyboard::PageDown) && view.getSize().x < 3300) {
			view.setSize(1.01f * view.getSize());
			player->getHud()->setScale(1.01f);
		}
		else if (Keyboard::isKeyPressed(Keyboard::PageUp) && view.getSize().x > 1500) {
			view.setSize(0.99f * view.getSize());
			player->getHud()->setScale(0.99f);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Home)) {
			view.setSize(ZOOM_FACTOR * 1200.f, ZOOM_FACTOR * 720.f);
			player->getHud()->resetScale();
		}

		if (Keyboard::isKeyPressed(Keyboard::P)) {
			player->rescue();
		}
		player->setHudSpeed();

		if (Keyboard::isKeyPressed(Keyboard::F1) ||
			Keyboard::isKeyPressed(Keyboard::F2) ||
			Keyboard::isKeyPressed(Keyboard::F3)) {
			b2Vec2 pos = player->getPosition();
			pos.y -= 3.f;
			float n20 = player->getN20();
			m_world.m_world->SetContactListener(NULL);
			if (Keyboard::isKeyPressed(Keyboard::F1))
				player = shared_ptr<Tank>(new Centauro120(m_world.getWorld(), &window, hud, contactListener, pos, n20));
			else if (Keyboard::isKeyPressed(Keyboard::F2))
				player = shared_ptr<Tank>(new Ural(m_world.getWorld(), &window, hud, contactListener, pos, n20));
			else if (Keyboard::isKeyPressed(Keyboard::F3))
				player = shared_ptr<Tank>(new Vaz(m_world.getWorld(), &window, hud, contactListener, pos, n20));
			Sleep(500);
		}

		if (Keyboard::isKeyPressed(Keyboard::D)) debug = true; else debug = false;

		dd.setDrawCircle(debug);
		dd.setDrawPolygon(debug);
		dd.setDrawEdge(debug);

		//////////Draw///////////////
		if (Mouse::isButtonPressed(Mouse::Right)){ 
			m_world.update(120.f, view.getCenter()); 
			player->setN20_cosumption(0.5f);
		}
		else{
			m_world.update(60.f, view.getCenter());
			player->setN20_cosumption(1.0f);
		}
		player->update();
		dd.update();

		time = clock.getElapsedTime().asSeconds();
		player->updateHud(time, view.getCenter());
		clock.restart().asSeconds();
		window.display();
	}
	return 0;
}
