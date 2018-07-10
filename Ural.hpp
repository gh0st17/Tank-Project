#include <SFML/Graphics.hpp>
#include <Box2D.h>
#include <vector>
#include <math.h>
#include <iostream>

using namespace sf;

class Ural : public Tank{
private:
	const float N20_LIMIT = 900.f;
	void setBody(b2Vec2 pos){
		b2PolygonShape chassis1;
		b2PolygonShape chassis2;
		b2PolygonShape chassis3;
		b2PolygonShape chassis4;
		b2PolygonShape chassis5;
		b2PolygonShape chassis6;
		b2PolygonShape chassis7;
		b2PolygonShape chassis8;
		b2PolygonShape chassis9;
		b2Vec2 vertices1[7];
		vertices1[0].Set(11.4f, 1.8f);
		vertices1[1].Set(12.2f, 1.7f);
		vertices1[2].Set(13.0f, 1.0f);
		vertices1[3].Set(13.0f, -1.9f);
		vertices1[4].Set(11.2f, -2.5f);
		vertices1[5].Set(9.2f, -2.8f);
		vertices1[6].Set(7.7f, -2.8f);
		chassis1.Set(vertices1, 7);

		b2Vec2 vertices2[8];
		vertices2[0].Set(7.7f, -2.8f);
		vertices2[1].Set(7.4f, -4.8f);
		vertices2[2].Set(5.1f, -5.0f);
		vertices2[3].Set(4.0f, -5.0f);
		vertices2[4].Set(3.4f, -4.8f);
		vertices2[5].Set(3.2f, -1.9f); // cross
		vertices2[6].Set(3.2f, 2.2f);
		vertices2[7].Set(6.6f, 2.2f); // cross
		chassis2.Set(vertices2, 8);

		b2Vec2 vertices3[3];
		vertices3[0].Set(11.4f, 1.8f);
		vertices3[1].Set(7.7f, -2.8f);
		vertices3[2].Set(6.6f, 2.2f);
		chassis3.Set(vertices3, 3);

		b2Vec2 vertices4[4];
		vertices4[0].Set(3.2f, 2.2f);
		vertices4[1].Set(3.2f, 0.0f);
		vertices4[2].Set(1.3f, 0.0f);
		vertices4[3].Set(1.3f, 2.2f);
		chassis4.Set(vertices4, 4);

		b2Vec2 vertices5[7];
		vertices5[0].Set(3.2f, -1.9f);;
		vertices5[1].Set(2.8f, -4.0f);
		vertices5[2].Set(1.8f, -4.0f);
		vertices5[3].Set(1.5f, -2.0f);
		vertices5[4].Set(1.5f, -2.5f);
		vertices5[5].Set(1.8f, 0.0f);
		vertices5[6].Set(3.2f, 0.0f);
		chassis5.Set(vertices5, 7);

		b2Vec2 vertices6[4];
		vertices6[0].Set(-13.7f, -4.0f);
		vertices6[1].Set(-13.7f, 2.2f);
		vertices6[2].Set(-13.5f, 2.2f);
		vertices6[3].Set(-13.5f, -4.0f);
		chassis6.Set(vertices6, 4);

		b2Vec2 vertices7[4];
		vertices7[0].Set(1.1f, 2.2f);
		vertices7[1].Set(-13.5f, 2.2f);
		vertices7[2].Set(1.1f, -.15f);
		vertices7[3].Set(-13.5f, -.15f);
		chassis7.Set(vertices7, 4);

		b2Vec2 vertices8[4];
		vertices8[0].Set(1.3f, -4.0f);
		vertices8[1].Set(1.3f, 2.2f);
		vertices8[2].Set(1.1f, -4.0f);
		vertices8[3].Set(1.1f, 2.2f);
		chassis8.Set(vertices8, 4);

		b2Vec2 vertices9[7];
		vertices9[2].Set(13.0f, 1.0f);
		vertices9[0].Set(13.2f, 1.0f);
		vertices9[1].Set(13.3f, 1.35f);
		vertices9[3].Set(13.8f, 1.6f);
		vertices9[4].Set(13.8f, 0.55f);
		vertices9[5].Set(13.5f, -0.05f);
		vertices9[6].Set(13.0f, -0.05f);
		chassis9.Set(vertices9, 7);

		b2CircleShape circle;
		circle.m_radius = 2.2f;

		b2BodyDef bd;
		bd.type = b2_dynamicBody;

		b2FixtureDef fd;
		fd.shape = &circle;
		fd.density = 7.33f;
		fd.friction = 5.f;
		fd.restitution = 0.0f;

		float f[3] = { 24.2f, 11.25f, 6.f };
		for (unsigned i = 0; i < m_wheels.size(); i++){
			bd.position.Set(pos.x + f[i] - 15, pos.y + 3);
			m_wheels[i] = world->CreateBody(&bd);
			m_wheels[i]->CreateFixture(&fd);
			m_wheels[i]->SetUserData("wheel");
		}

		bd.position = pos;
		body = world->CreateBody(&bd);
		fd.shape = &chassis1;
		fd.density = 9.5f;
		fd.friction = 0.1f;
		body->CreateFixture(&fd);
		fd.shape = &chassis2;
		body->CreateFixture(&fd);
		fd.shape = &chassis3;
		fd.density = 2.5f;
		body->CreateFixture(&fd);
		fd.shape = &chassis4;
		body->CreateFixture(&fd);
		fd.shape = &chassis5;
		body->CreateFixture(&fd);
		fd.shape = &chassis6;
		fd.density = 2.2f;
		body->CreateFixture(&fd);
		fd.shape = &chassis7;
		body->CreateFixture(&fd);
		fd.shape = &chassis8;
		body->CreateFixture(&fd);


		fd.shape = &chassis9;
		fd.density = 0.25f;
		body->CreateFixture(&fd);
		b2WheelJointDef jd;
		b2Vec2 axis(0.0f, 1.0f);

		for (unsigned i = 0; i < m_wheels.size(); i++){
			jd.Initialize(body, m_wheels[i], m_wheels[i]->GetPosition(), axis);
			jd.motorSpeed = 0.f;
			jd.maxMotorTorque = max_motor_torque;
			jd.enableMotor = true;
			jd.frequencyHz = m_hz;
			jd.dampingRatio = m_zeta;
			m_springs[i] = (b2WheelJoint*)world->CreateJoint(&jd);
		}
	}

	float n20_cosumption = 1.f;

public:
/*	Ural(b2World * world, RenderWindow * window, Hud &hud, MyContact * contactListener) : Tank(world, window, hud, 3) {
		n20_count = N20_LIMIT;
		min_speed = -10.5f, max_speed = 55.f;
		m_hz = 3.75f, m_zeta = 1.f; max_motor_torque = 10000.f;
		setBody(b2Vec2(15, -3));
		contactListener = new MyContact();
		world->SetContactListener(contactListener);
		body->SetBullet(true);
		body->SetUserData("body");
	}*/
	Ural(b2World * world, RenderWindow * window, Hud &hud, MyContact * contactListener, b2Vec2 pos, float n20_count) : Tank(world, window, hud, 3) {
		min_speed = -10.5f, max_speed = 50.f;
		m_hz = 3.75f, m_zeta = 1.f; max_motor_torque = 7200.f;
		setBody(pos);
		contactListener = new MyContact();
		world->SetContactListener(contactListener);
		body->SetBullet(true);
		body->SetUserData("body");
		this->n20_count = n20_count;
	}
	~Ural() {
		destroy();
		cout << "Ural deleted\n";
	}

	void destroy(){
		body->SetUserData("bodyForDel");
		for (auto wheel : m_wheels)
			wheel->SetUserData("bodyForDel");
		for (auto spring : m_springs)
			spring->SetUserData("jointForDel");
	}

	void update(){
		b2Vec2 pos = body->GetPosition();
		float angle = body->GetAngle() * DEG;
		sf::Vector2f sfPos(pos.x*SCALE, pos.y*SCALE);
		sprites[3].setPosition(sfPos);
		sprites[3].setRotation(angle);
		window->draw(sprites[3]);

		for (unsigned i = 0; i < m_wheels.size(); i++){
			pos = m_wheels[i]->GetPosition();
			angle = m_wheels[i]->GetAngle() * DEG;
			sfPos = sf::Vector2f(pos.x*SCALE, pos.y*SCALE);
			sprites[4].setPosition(sfPos);
			sprites[4].setRotation(angle);
			window->draw(sprites[4]);
		}
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
				m_springs[i]->SetMaxMotorTorque(max_motor_torque + 2500.f);
		}
	}

	void addN20(float amount){
		n20_count += amount;
		if (n20_count > N20_LIMIT) n20_count = N20_LIMIT;
	}

	Vector2f getChassisOffsetView(){
		Vector2f center = sprites[3].getPosition();
		center.x += sprites[3].getTextureRect().width - 200;
		center.y -= 125;
		return center;
	}

	Vector2f getChassisCenter(){
		return sprites[3].getPosition();
	}

	b2Vec2 getPosition(){ return body->GetPosition(); }

	float getAngle(Vector2f &mouseP){ return 0.f; }
};

