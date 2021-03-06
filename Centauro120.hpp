﻿#include <SFML/Graphics.hpp>
#include <Box2D.h>
#include <vector>
#include <math.h>
#include <iostream>

using namespace sf;

class Centauro120 : public Tank{
private:
	b2Body* b_gun;
	b2Vec2 axis2;
	void setBody(b2Vec2 pos){
		b2PolygonShape chassis;
		b2Vec2 vertices[8];
		vertices[0].Set(-12.7f, 3.2f);
		vertices[1].Set(-12.7f, 2.7f);
		vertices[2].Set(-13.2f, 2.7f);
		vertices[3].Set(-13.2f, 2.f);
		vertices[4].Set(-14.f, -1.1f);
		vertices[5].Set(1.8f, -1.1f);
		vertices[6].Set(9.8f, .4f);
		vertices[7].Set(7.f, 3.2f);
		chassis.Set(vertices, 8);

		b2PolygonShape cab;
		b2Vec2 verticesCab[7];
		verticesCab[0].Set(-9.1f, -3.3f);
		verticesCab[1].Set(-13.7f, -2.7f);
		verticesCab[2].Set(-13.7f, -1.4f);
		verticesCab[3].Set(-9.1f, -1.f);
		verticesCab[4].Set(-3.2f, -1.2f);
		verticesCab[5].Set(-1.f, -1.2f);
		verticesCab[6].Set(-1.f, -3.f);
		cab.Set(verticesCab, 7);

		b2PolygonShape gun;
		b2Vec2 verticesGun[4];
		verticesGun[0].Set(-1.8f, -2.6f);
		verticesGun[1].Set(-1.8f, -1.7f);
		verticesGun[2].Set(9.9f, -1.9f); // x = 14.f - natural size!!!
		verticesGun[3].Set(9.9f, -2.5f); // x = 14.f - natural size!!!
		gun.Set(verticesGun, 4);

		b2CircleShape circle;
		circle.m_radius = 1.8f;

		b2BodyDef bd;
		bd.type = b2_dynamicBody;

		b2FixtureDef fd;
		fd.shape = &circle;
		fd.density = 6.f;
		fd.friction = 4.0f;
		fd.restitution = 0.0f;

		float f = 5.f;
		for (unsigned i = 0; i < m_wheels.size(); i++){
			bd.position.Set(pos.x + f - 15, pos.y + 3.f);
			m_wheels[i] = world->CreateBody(&bd);
			m_wheels[i]->CreateFixture(&fd);
			m_wheels[i]->SetUserData("wheel");
			f += 4.55f;
			if (i == 2) f += 0.65f;
		}

		bd.position = pos;
		body = world->CreateBody(&bd);
		b_gun = world->CreateBody(&bd);
		fd.shape = &chassis;
		fd.density = 10.f;
		fd.friction = 0.01f;
		body->CreateFixture(&fd);
		fd.shape = &cab;
		fd.density = 5.f;
		body->CreateFixture(&fd);
		body->SetUserData("body");
		fd.shape = &gun;
		fd.density = 2.f;
		b_gun->CreateFixture(&fd);
		b_gun->SetUserData("gun");

		axis2 = b2Vec2(pos.x + 14.f - 15, pos.y + -2.3f);

		rjd.Initialize(b_gun, body, axis2);
		rjd.enableMotor = true;
		rjd.enableLimit = true;
		rjd.motorSpeed = 0.0f;
		rjd.maxMotorTorque = 10000.0f;
		rjd.lowerAngle = -5 / DEG;
		rjd.upperAngle = 60 / DEG;
		world->CreateJoint(&rjd);

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
	Vector2f mouse_pos;
	//RectangleShape rect;

public:
/*	Centauro120(b2World * world, RenderWindow * window, Hud &hud, MyContact * contactListener) : Tank(world, window, hud, 4) {
		n20_count = N20_LIMIT;
		min_speed = -8.5f, max_speed = 34.f;
		m_hz = 4.25f, m_zeta = 1.75f; max_motor_torque = 8500.f;
		//rect.setSize(Vector2f(5, 5));
		//rect.setFillColor(Color::Red);
		setBody(b2Vec2(15, -3));
		contactListener = new MyContact();
		world->SetContactListener(contactListener);
		body->SetBullet(true);
	}*/
	Centauro120(b2World * world, RenderWindow * window, Hud &hud, MyContact * contactListener, b2Vec2 pos, float n20) : Tank(world, window, hud, 4) {
		n20_count = n20;
		min_speed = -8.5f, max_speed = 34.f;
		m_hz = 4.25f, m_zeta = 1.75f; max_motor_torque = 8500.f;
		n20addition = 1250.f;
		//rect.setSize(Vector2f(5, 5));
		//rect.setFillColor(Color::Red);
		setBody(pos);
		contactListener = new MyContact();
		world->SetContactListener(contactListener);
		body->SetBullet(true);
		if (this->n20_count > getN20limit()) n20_count = getN20limit();
		setN20limit(750.f);
	}
	~Centauro120() {
		destroy(); cout << "Centauro120 deleted\n";
	}

	void destroy(){
		body->SetUserData("bodyForDel");
		b_gun->SetUserData("bodyForDel");
		for (auto wheel : m_wheels)
			wheel->SetUserData("bodyForDel");
		for (auto spring : m_springs)
			spring->SetUserData("jointForDel");
	}

	void update(){
		mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));
		setGunAngle(mouse_pos);
		b2Vec2 pos = body->GetPosition();
		float angle = body->GetAngle() * DEG;
		sf::Vector2f sfPos(pos.x*SCALE, pos.y*SCALE);
		sprites[5].setPosition(sfPos);
		sprites[5].setRotation(angle);
		sprites[7].setPosition(sfPos);
		sprites[7].setRotation(angle);
		sprites[8].setPosition(sfPos);
		sprites[8].setRotation(angle);
		window->draw(sprites[5]);

		for (unsigned i = 0; i < m_wheels.size(); i++){
			pos = m_wheels[i]->GetPosition();
			angle = m_wheels[i]->GetAngle() * DEG;
			sfPos = sf::Vector2f(pos.x*SCALE, pos.y*SCALE);
			sprites[6].setPosition(sfPos);
			sprites[6].setRotation(angle);
			window->draw(sprites[6]);
		}

		pos = b_gun->GetPosition();
		sfPos = sf::Vector2f(pos.x*SCALE, pos.y*SCALE);
		angle = b_gun->GetAngle() * DEG;
		sprites[9].setPosition(sfPos);
		sprites[9].setRotation(angle);

		window->draw(sprites[9]);
		window->draw(sprites[7]);
		window->draw(sprites[8]);
		//window->draw(rect);


		/*if (Mouse::isButtonPressed(Mouse::Left)){
			Bullet * b = new Bullet(world, getPosition().x + 11.f, getPosition().y - 1.2f);
		}*/

	}

	void updateHud(float &time, Vector2f viewCenter){
		hud->update(getN20limit(), n20_count);
		hud->updateDebug(time, viewCenter, "\nAngle: " + to_string(getAngle(mouse_pos)));
	}

	void setGunAngle(Vector2f mouseP){
		float32 targetAngle = getAngle(mouseP);
		b2JointEdge *j = b_gun->GetJointList();
		b2RevoluteJoint *r = (b2RevoluteJoint *)j->joint;
		float32 rjdAngle = r->GetJointAngle() * DEG;
		if (roundf(rjdAngle) > roundf(targetAngle)) 	
			r->SetMotorSpeed(-0.5f);
		else if (roundf(rjdAngle) < roundf(targetAngle))
			r->SetMotorSpeed( 0.5f);
		else r->SetMotorSpeed(0.f);
	}

	float getAngle(Vector2f &mouseP) { 
		float a;
		b2Vec2 vec = rjd.bodyB->GetPosition() + rjd.localAnchorA;
		a = atan2f(mouseP.x - vec.x * SCALE, mouseP.y - vec.y * SCALE) * DEG - 90;
		return a + (body->GetAngle() * DEG);
	}

	Vector2f getChassisOffsetView(){
		Vector2f center = sprites[5].getPosition();
		center.x += sprites[5].getTextureRect().width;
		center.y -= 125;
		return center;
	}

	Vector2f getChassisCenter(){
		return sprites[5].getPosition();
	}
};

