#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <math.h>
#include <random>
#include <vector>
#include <ctime>
#include "world.hpp"

using namespace sf;

int main() {
	ContextSettings settings;
	settings.antialiasingLevel = 4;
	settings.depthBits = 8;
	RenderWindow window(sf::VideoMode(1200, 720, 8), "Tank Project v1.4a | Ghost-17", Style::Close, settings);
	Image icon; icon.loadFromFile("images/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	World m_world(&window);
	m_world.generateMap(true);

	window.setFramerateLimit(70);

	// Bridge
	/*{
		int32 N = 10;
		b2PolygonShape shape;
		shape.SetAsBox(2.0f, 0.125f);

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 15.0f;
		fd.friction = 1.f;
		fd.restitution = 0.f;

		b2RevoluteJointDef jd;

		b2Body* prevBody = ground;
		for (int32 i = 0; i < N; ++i)
		{
		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position.Set(150.0f + 4.0f * i, hs[9] + 20.f);
		b2Body* body = world.CreateBody(&bd);
		body->SetUserData("bridge");
		body->CreateFixture(&fd);

		b2Vec2 anchor(151.0f + 4.0f * i, hs[9] + 20.f);
		jd.Initialize(prevBody, body, anchor);
		world.CreateJoint(&jd);

		prevBody = body;
		}

		b2Vec2 anchor(150.0f + 4.0f * N, hs[10] + 20.f);
		jd.Initialize(prevBody, ground, anchor);
		world.CreateJoint(&jd);
		}*/

	Hud hud = Hud(&window);
	MyContact *contactListener = new MyContact();
	player = new Vaz(m_world.getWorld(), &window, hud, contactListener);
	m_world.m_world->SetContactListener(contactListener);

	view.setSize(1200.f, 720.f);
	view.zoom(ZOOM_FACTOR);

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

		view.setCenter(player->getChassisCenter());
		window.setView(view);
		window.draw(m_world.getBack());

		if (Keyboard::isKeyPressed(Keyboard::W)){
			player->right();
			if (Keyboard::isKeyPressed(Keyboard::LShift)) player->n20();
			else player->stop_n20();
		}
		else if (Keyboard::isKeyPressed(Keyboard::S)) player->left();
		else if (Keyboard::isKeyPressed(Keyboard::Space)) player->handBreak();
		else player->stop();

		if (Keyboard::isKeyPressed(Keyboard::P)) {
			player->rescue();
		}
		player->setHudPos(view.getCenter());

		if (Keyboard::isKeyPressed(Keyboard::F1) ||
			Keyboard::isKeyPressed(Keyboard::F2) ||
			Keyboard::isKeyPressed(Keyboard::F3)) {
			b2Vec2 pos = player->getPosition();
			float n20 = player->getN20();
			m_world.m_world->SetContactListener(NULL);
			player->destroy();
			delete player;
			if (Keyboard::isKeyPressed(Keyboard::F1))
				player = new Centauro120(m_world.getWorld(), &window, hud, contactListener, pos, n20);
			else if (Keyboard::isKeyPressed(Keyboard::F2))
				player = new Ural(m_world.getWorld(), &window, hud, contactListener, pos, n20);
			else if (Keyboard::isKeyPressed(Keyboard::F3))
				player = new Vaz(m_world.getWorld(), &window, hud, contactListener, pos, n20);
		}

		if (Keyboard::isKeyPressed(Keyboard::D)) debug = true; else debug = false;

		dd.setDrawCircle(debug);
		dd.setDrawPolygon(debug);
		//dd.setDrawEdge(debug);

		//////////Draw///////////////
		if (Mouse::isButtonPressed(Mouse::Right)){ 
			m_world.update(player, 120.f, view.getCenter()); 
			player->setN20_cosumption(0.5f);
		}
		else{
			m_world.update(player, 60.f, view.getCenter());
			player->setN20_cosumption(1.0f);
		}
		Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		player->setGunAngle(mouse_pos);
		player->update();
		dd.update();

		time = clock.getElapsedTime().asSeconds();
		player->updateHud(time, view.getCenter(), mouse_pos);
		clock.restart().asSeconds();
		window.display();
	}
	return 0;
}
