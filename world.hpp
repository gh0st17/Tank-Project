#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "MyContact.hpp"
#include "Centauro120.hpp"
#include "Ural.hpp"
#include "Vaz.hpp"

class World {
private:
	unsigned py_i = 0;
	float step = 0, last;
	float32 x = 0, y = 0;
	b2Body* start;
	b2PolygonShape startSh;
	b2BodyDef bdef;
	b2BodyDef bdefGr;
	b2FixtureDef fdef;
	b2EdgeShape shape;
	std::vector<VertexArray> groundArr;
	RenderWindow * window;
	Texture block, back, start_t, platform_t;
	Sprite sBox, sBack, start_s, platform_s;
	ConvexShape cs;
	b2Body* ground = NULL;
	b2Body* m_platformBody2;
	Font font;
	Text meterText;
	bool startUpd = 0;

	void addVertex(float &x, float dx, float &y, float y2, Color color){
		groundArr.push_back(VertexArray(PrimitiveType::LinesStrip, 2));
		groundArr[groundArr.size() - 1][0] = Vector2f(x * SCALE, y * SCALE);
		groundArr[groundArr.size() - 1][1] = Vector2f((x + dx) * SCALE, y2 * SCALE);
		groundArr[groundArr.size() - 1][0].color = color;
		groundArr[groundArr.size() - 1][1].color = color;
	}

public:
	vector<b2Vec2> py;
	b2World * m_world;
	World(RenderWindow * window){
		this->window = window;
		b2Vec2 gravity(0.f, 19.6f);
		m_world = new b2World(gravity);
		m_world->SetContinuousPhysics(true);
		block.loadFromFile("images/world/box.jpg");
		back.loadFromFile("images/hud/back.jpg");
		start_t.loadFromFile("images/world/start.jpg");
		platform_t.loadFromFile("images/world/platform.jpg");
		block.setSmooth(true);
		start_t.setSmooth(true);
		platform_t.setSmooth(true);
		sBox.setTexture(block);
		sBack.setTexture(back);
		start_s.setTexture(start_t);
		platform_s.setTexture(platform_t);
		sBack.setScale(ZOOM_FACTOR, ZOOM_FACTOR);
		sBox.setOrigin(30, 30);
		start_s.setOrigin(60, 750);
		platform_s.setOrigin(300, 30);
		cs.setFillColor(Color::Color(165, 89, 8));
		cs.setPointCount(4);
		bdefGr.userData = "ground";
		ground = m_world->CreateBody(&bdefGr);
		startSh.SetAsBox(2, 25);
		bdef.position.Set(2, 7.5);
		bdef.userData = "start";
		start = m_world->CreateBody(&bdef);
		b2Fixture* startf = start->CreateFixture(&startSh, 0);
		bdef.position.Set(0, 0);
		font.loadFromFile("res/SF-Pro-Display-Bold.otf");
		meterText.setFont(font); meterText.setCharacterSize(19 * (unsigned)ZOOM_FACTOR); meterText.setScale(ZOOM_FACTOR, ZOOM_FACTOR); meterText.setOutlineThickness(2.0f);

	}
	~World(){}

	Sprite getBack() { return sBack; }

	void setWall(float x, float y, float w, float h, b2BodyType type = b2_staticBody) {
		b2PolygonShape gr;
		gr.SetAsBox(w / SCALE, h / SCALE);

		bdef.type = type;
		bdef.position.Set(x / SCALE, y / SCALE);
		b2FixtureDef fd;
		fd.density = 3.33f;
		fd.friction = 0.6f;
		fd.restitution = 0.25f;
		fd.shape = &gr;

		b2Body *b_ground = m_world->CreateBody(&bdef);
		b_ground->CreateFixture(&fd);
		b_ground->SetUserData("box");
	}
	void genBlocks(unsigned count, b2Vec2 startPos) {
		const float w = 30.f, offsetX = 1.f, offsetY = 0.4f;
		for (size_t c = 0; c < count; c++)
			for (size_t i = 0; i < 3; i++) setWall((startPos.x + (i / SCALE * w)) * SCALE,
				(startPos.y - 20.f) * SCALE,
				w, w, b2_dynamicBody);
	}
	void generateMap(bool first = false){
		float32 hs[8], dx = 10.0f, y2;
		fdef.shape = &shape;
		fdef.density = 0.f;
		fdef.friction = 0.9f;
		fdef.restitution = 0;
		if (fmod(x, 400) == 0.0 || x == 0){
			meterText.setString(to_string((unsigned)x) + 'm');
			meterText.setPosition(x * SCALE, y * SCALE - 150);
		}
		if (first) last = x;
		else
		{
			last = x - 200;
		}
		for (int k = 0; k < 4; k++){
			for (int i = 0; i < 5; ++i) {
				srand((unsigned)time(0) * (k + 1) * (i + 1));
				if (i > 0) hs[i] = hs[i - 1] + 3.f - (float)(rand() % 9);
				else hs[i] = 0;
				if (i >= 2 && fabsf(hs[i] - hs[i - 2]) > 20.f) hs[i] = hs[i - 2];
				y2 = hs[i] + 5;
				shape.Set(b2Vec2(x, y), b2Vec2(x + dx, y2));
				addVertex(x, dx, y, y2, Color::Color(165, 89, 8));
				ground->CreateFixture(&fdef);
				y = y2;
				x += dx;
				if ((unsigned)x % 150 == 0) genBlocks(4, b2Vec2(x, hs[i]));
			}
			if (k == 0) py.push_back(b2Vec2(x + 10, y2 - 5));
		}

		startUpd = 1;
		//setup platform shape for reuse
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(10, 1);

		b2BodyDef bodyDef;
		bodyDef.type = b2_kinematicBody;
		bodyDef.position.Set(py.back().x, py.back().y);
		bodyDef.angle = ((-15 + rand() % 31) / DEG);
		bodyDef.userData = "platform";
		m_platformBody2 = m_world->CreateBody(&bodyDef);
		b2Fixture* platformFixture2 = m_platformBody2->CreateFixture(&polygonShape, 0);
		platformFixture2->SetUserData((void*)1);//anything non-zero
	}
	b2World * getWorld() { return m_world; }
	void update(Tank * player, float freq, Vector2f _view){
		// Platform
		float theta = 0.025f + step;
		step += 0.015f;
		if (step == 1.0f) step = 0.0f;
		for (auto _py : py){
			b2Vec2 targetPos(_py.x, _py.y - 5.55f * cosf(theta));
			m_platformBody2->SetLinearVelocity(60 * (targetPos - m_platformBody2->GetPosition()));
		}

		if (x - player->getPosition().x < 50.f) generateMap();

		/////Destroy box//////
		std::vector<b2Vec2> wPos = player->getWheelsPosition();
		for (b2Body* it = m_world->GetBodyList(); it != 0; it = it->GetNext()){
			if (player->boxDel.size() > 0) {
				for (auto boxForDel : player->boxDel){
					m_world->DestroyBody(boxForDel);
					it = m_world->GetBodyList();
				}
				player->boxDel.resize(0);
			}
			for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext()){
				for (auto _wPos : wPos){
					if ((it->GetUserData() == "box") &&
						player->getPosition().x - it->GetPosition().x > 60) {

						m_world->DestroyBody(it);
						it = m_world->GetBodyList();
						f = it->GetFixtureList();
					}
				}
				if ((it->GetUserData() == "platform") &&
					player->getPosition().x - it->GetPosition().x > 60) {

					m_world->DestroyBody(it);
					it = m_world->GetBodyList();
					f = it->GetFixtureList();
				}
				if (startUpd && it->GetUserData() == "start") {
					m_world->DestroyBody(it);
					it = m_world->GetBodyList();
					f = it->GetFixtureList();
					bdef.position.Set(last + 2, y - 7.5f);
					bdef.type = b2_staticBody;
					b2FixtureDef fstart;
					fstart.shape = &startSh;
					start = m_world->CreateBody(&bdef);
					start->CreateFixture(&fstart);
					start->SetUserData("start");
					startUpd = 0;
				}
			}
			sBack.setPosition(_view.x - 600 * ZOOM_FACTOR, _view.y - 360 * ZOOM_FACTOR);
			b2Vec2 pos = it->GetPosition();
			float angle = it->GetAngle() * DEG;
			sf::Vector2f sfPos(pos.x*SCALE, pos.y*SCALE);

			if (it->GetUserData() == "box") {
				sBox.setPosition(sfPos);
				sBox.setRotation(angle);
				window->draw(sBox);
			}

			if (it->GetUserData() == "platform") {
				platform_s.setPosition(sfPos);
				platform_s.setRotation(angle);
				window->draw(platform_s);
			}

			if (it->GetUserData() == "start") {
				start_s.setPosition(sfPos);
				start_s.setRotation(angle);
				window->draw(start_s);
			}
		}

		for (auto grArr : groundArr) {
			window->draw(grArr);
			Vector2f v1, v2;
			v1 = grArr[0].position;
			v2 = grArr[1].position;
			cs.setPoint(0, v1);
			cs.setPoint(1, v2);
			v1.y += 1500;
			v2.y += 1500;
			cs.setPoint(3, v1);
			cs.setPoint(2, v2);
			window->draw(cs);
		}
		window->draw(meterText);
		m_world->Step(1 / freq, 8, 3);
	}
};