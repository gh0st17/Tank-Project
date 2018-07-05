#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

using namespace sf;

class Bullet{
private:
	b2Body * body;
	RectangleShape rect;
	b2FixtureDef fd;
	b2BodyDef bd;

	void setBody(){

	}

public:

	Bullet(b2World * m_world, float x, float y){
		//setBody();
		b2PolygonShape bullet;
		b2Vec2 verticesGun[4];
		float cabOffsetY = 4.3f;
		verticesGun[0].Set(-1.8f, 1.8f - cabOffsetY);
		verticesGun[1].Set(-1.8f, 1.9f - cabOffsetY);
		verticesGun[2].Set(1.f, 1.8f - cabOffsetY);
		verticesGun[3].Set(1.f, 1.9f - cabOffsetY);
		bullet.Set(verticesGun, 4);

		fd.shape = &bullet;
		fd.density = 50.0f;
		fd.friction = 10.0f;
		fd.restitution = 0.0f;
		bd.type = b2_dynamicBody;
		bd.position.Set(x, y);
		body = m_world->CreateBody(&bd);
		body->CreateFixture(&fd);
		body->ApplyForceToCenter(b2Vec2(100000, 0), 1);
	}
};