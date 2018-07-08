#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <math.h>
#include <iostream>

using namespace sf;

class Vaz : public Tank{
private:
	const float N20_LIMIT = 800.f;
	void setBody(b2Vec2 pos){
		b2PolygonShape chassis1;
		b2PolygonShape chassis2;
		b2PolygonShape chassis3;
		b2PolygonShape chassis4;
		b2PolygonShape chassis5;
		b2PolygonShape chassis6;
		b2PolygonShape chassis7;
		b2PolygonShape chassis8; // Front bumper
		b2PolygonShape chassis9; // Rear bumper
		b2Vec2 vertices1[4];
		vertices1[0].Set(6.84f, 2.2f);
		vertices1[1].Set(7.5f, 1.37f);
		vertices1[2].Set(4.0f, -0.2f);
		vertices1[3].Set(4.2f, 2.55f);
		chassis1.Set(vertices1, 4);

		b2Vec2 vertices8[4];
		vertices8[0].Set(6.84f, 2.2f);
		vertices8[1].Set(7.5f, 1.37f);
		vertices8[2].Set(8.0f, 1.5f); 
		vertices8[3].Set(8.0f, 2.1f); 
		chassis8.Set(vertices8, 4);

		b2Vec2 vertices2[8];
		vertices2[0].Set(7.5f, 1.37f);
		vertices2[1].Set(7.95f, 0.8f);
		vertices2[2].Set(7.9f, 0.3f);
		vertices2[3].Set(7.5f, 0.1f);
		vertices2[4].Set(6.6f, -0.04f);
		vertices2[5].Set(5.85f, -0.1f);
		vertices2[6].Set(5.0f, -0.19f);
		vertices2[7].Set(4.0f, -0.2f);
		chassis2.Set(vertices2, 8);

		b2Vec2 vertices3[7];
		vertices3[0].Set(4.2f, 2.55f);
		vertices3[1].Set(4.0f, -0.2f);
		vertices3[2].Set(3.65f, -0.68f);
		vertices3[3].Set(3.55f, -0.6f);
		vertices3[4].Set(0.8f, 2.8f);
		vertices3[5].Set(1.5f, 2.77f);
		vertices3[6].Set(1.83f, 2.65f);
		chassis3.Set(vertices3, 7);

		b2Vec2 vertices4[8];
		vertices4[0].Set(3.55f, -0.6f);
		vertices4[1].Set(0.8f, 2.8f);
		vertices4[2].Set(0.55f, 2.7f);
		vertices4[3].Set(0.4f, 2.7f);
		vertices4[4].Set(0.4f, 2.65f);
		vertices4[5].Set(-0.9f, 2.6f);
		vertices4[6].Set(2.3f, -1.85f);
		vertices4[7].Set(2.5f, -1.6f);
		chassis4.Set(vertices4, 8);

		b2Vec2 vertices5[8];
		vertices5[0].Set(-0.9f, 2.6f);
		vertices5[1].Set(2.3f, -1.85f);
		vertices5[2].Set(1.9f, -2.1f);
		vertices5[3].Set(-1.5f, -2.1f);
		vertices5[4].Set(-2.38f, -2.08f);
		vertices5[5].Set(-3.13f, -2.03f);

		vertices5[6].Set(-2.42f, 2.6f);
		vertices5[7].Set(-1.35f, 2.5f);
		chassis5.Set(vertices5, 8);

		b2Vec2 vertices6[5];
		vertices6[0].Set(-3.13f, -2.03f);
		vertices6[1].Set(-2.42f, 2.6f);

		vertices6[2].Set(-5.78f, 2.6f); // important
		vertices6[3].Set(-5.8f, 0.0f);
		vertices6[4].Set(-4.2f, -1.8f);
		chassis6.Set(vertices6, 5);

		b2Vec2 vertices7[7];
		vertices7[0].Set(-5.78f, 2.6f);
		vertices7[1].Set(-5.8f, -0.1f);
		vertices7[2].Set(-6.4f, -0.1f);
		vertices7[3].Set(-8.15f, 0.15f);
		vertices7[4].Set(-8.5f, 0.53f);
		vertices7[5].Set(-8.45f, 1.5f);
		vertices7[6].Set(-7.6f, 2.6f);
		chassis7.Set(vertices7, 7);

		b2Vec2 vertices9[4];
		vertices9[0].Set(-7.45f, 1.7f);
		vertices9[1].Set(-7.45f, 2.3f);
		vertices9[2].Set(-8.55f, 2.3f);
		vertices9[3].Set(-8.65f, 1.7f);
		chassis9.Set(vertices9, 4);

		b2CircleShape circle;
		circle.m_radius = 1.2f;

		b2BodyDef bd;
		bd.type = b2_dynamicBody;

		b2FixtureDef fd;
		fd.shape = &circle;
		fd.density = 4.0f;
		fd.friction = 5.f;
		fd.restitution = 0.0f;

		float f[2] = { -4.5f, 5.5f };
		for (unsigned i = 0; i < m_wheels.size(); i++){
			bd.position.Set(pos.x + f[i], pos.y + 2.6f);
			bd.userData = "wheel";
			m_wheels[i] = world->CreateBody(&bd);
			m_wheels[i]->CreateFixture(&fd);
		}

		bd.position = pos;
		bd.userData = "body";
		body = world->CreateBody(&bd);
		fd.shape = &chassis1;
		fd.density = 5.f;
		fd.friction = 0.1f;
		body->CreateFixture(&fd);
		fd.shape = &chassis2;
		body->CreateFixture(&fd);
		fd.shape = &chassis3;
		body->CreateFixture(&fd);
		fd.shape = &chassis4;
		body->CreateFixture(&fd);
		fd.shape = &chassis5;
		fd.density = 8.f;
		body->CreateFixture(&fd);
		fd.shape = &chassis6;
		body->CreateFixture(&fd);
		fd.density = 2.5f;
		fd.shape = &chassis7;
		body->CreateFixture(&fd);
		fd.density = 1.0f;
		fd.shape = &chassis8;
		body->CreateFixture(&fd);
		fd.shape = &chassis9;
		body->CreateFixture(&fd);
		b2WheelJointDef jd;
		b2Vec2 axis(0.0f, 1.0f);

		for (unsigned i = 0; i < m_wheels.size(); i++){
			jd.Initialize(body, m_wheels[i], m_wheels[i]->GetPosition(), axis);
			jd.motorSpeed = 0.f;
			jd.maxMotorTorque = max_motor_torque;
			jd.frequencyHz = m_hz;
			jd.dampingRatio = m_zeta;
			m_springs[i] = (b2WheelJoint*)world->CreateJoint(&jd);
		}
		body->SetBullet(true);
	}

	float n20_cosumption = 1.f;

public:
	Vaz(b2World * world, RenderWindow * window, Hud &hud, MyContact * contactListener) : Tank(world, window, hud, 2) {
		n20_count = N20_LIMIT;
		min_speed = -10.5f, max_speed = 50.f;
		m_hz = 5.0f, m_zeta = 1.f; max_motor_torque = 5200.f;
		setBody(b2Vec2(15, -10));
		contactListener = new MyContact();
		world->SetContactListener(contactListener);
	}
	Vaz(b2World * world, RenderWindow * window, Hud &hud, MyContact * contactListener, b2Vec2 pos, float n20_count) : Tank(world, window, hud, 2) {
		min_speed = -10.5f, max_speed = 50.f;
		m_hz = 5.0f, m_zeta = 1.f; max_motor_torque = 5200.f;
		m_wheels.resize(2);
		m_springs.resize(2);
		setBody(pos);
		contactListener = new MyContact();
		world->SetContactListener(contactListener);
		this->n20_count = n20_count;
	}
	~Vaz() {
		destroy(); cout << "Vaz deleted\n";
	}

	void destroy(){
		body->SetUserData("bodyForDel");
		for (auto wheel : m_wheels)
			wheel->SetUserData("bodyForDel");
		for (auto spring : m_springs)
			spring->SetUserData("jointForDel");
	}

	void update(){
#ifdef debContact
		if (m_contacting){
			cout << contactType << "contact with " << contactBody << "\n";
		}
#endif

		b2Vec2 pos = body->GetPosition();
		float angle = body->GetAngle() * DEG;
		sf::Vector2f sfPos(pos.x*SCALE, pos.y*SCALE);
		sprites[0].setPosition(sfPos);
		sprites[0].setRotation(angle);
		sprites[2].setPosition(sfPos);
		sprites[2].setRotation(angle);
		window->draw(sprites[2]);

		for (unsigned i = 0; i < m_wheels.size(); i++){
			pos = m_wheels[i]->GetPosition();
			angle = m_wheels[i]->GetAngle() * DEG;
			sfPos = sf::Vector2f(pos.x*SCALE, pos.y*SCALE);
			sprites[1].setPosition(sfPos);
			sprites[1].setRotation(angle);
			window->draw(sprites[1]);
		}
		window->draw(sprites[0]);
	}

	void updateHud(float &time, Vector2f viewCenter, Vector2f mouse_pos){
		hud->update(N20_LIMIT, n20_count);
		hud->updateDebug(time, viewCenter, "");
	}

	void setN20_cosumption(float n20_cosumption){ this->n20_cosumption = n20_cosumption; }

	void n20(){
		if (n20_count != 0 && m_contacting){
			n20_count -= n20_cosumption;
			for (size_t i = 0; i < m_springs.size(); i++)
				m_springs[i]->SetMaxMotorTorque(max_motor_torque + 2000.f);
		}
	}

	void addN20(float amount){
		n20_count += amount;
		if (n20_count > N20_LIMIT) n20_count = N20_LIMIT;
	}

	Vector2f getChassisCenter(){
		Vector2f center = sprites[0].getPosition();
		center.x += sprites[0].getTextureRect().width + 200;
		center.y -= 125;
		return center;
	}

	b2Vec2 getPosition(){ return body->GetPosition(); }

	float getAngle(Vector2f &mouseP){ return 0.f; }
};

