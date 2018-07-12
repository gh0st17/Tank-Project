#include <SFML/Graphics.hpp>
#include <Box2D.h>
#include <vector>
#include <ctime>
#include "bullet.hpp"
#include "hud.hpp"

using namespace sf;
using namespace std;
//#define debContact

class Tank {
private:
	float N20_LIMIT = 0.f;
public:
	enum ContactType {
		Wheel,
		Body,
		Gun
	};
protected:
	bool m_contacting = false;
#ifdef debContact
	string contactType, contactBody;
#endif
	b2World * world;
	RenderWindow * window;
	b2Body * body;
	float32 m_hz, m_zeta, max_speed, min_speed, max_motor_torque;
	vector<b2Body*> m_wheels = vector<b2Body*>(0);
	vector<b2WheelJoint*> m_springs = vector<b2WheelJoint*>(0); // (0)chassis (0)wheel (0)wArc (0)cab (0)gun
	b2RevoluteJointDef rjd;
	float n20_count, n20_cosumption = 1.f, n20addition;
	Hud * hud;
	void setN20limit(float limit) { N20_LIMIT = limit; }
	float getN20limit() { return N20_LIMIT; }
public:
	Tank(b2World * world, RenderWindow * window, Hud &hud, unsigned wCount);
	~Tank(){}

#ifdef debContact
	void startContact(ContactType type, string contactData);
#else
	void startContact();
#endif
	void endContact();
	void incBrokenBoxes(){ hud->incBoxesBroken(); }
	virtual void setBody(b2Vec2 pos) = 0;
	void destroyBox(b2Body * box);
	virtual void right();
	virtual void left();
	virtual void stop();
	virtual void handBreak();
	virtual void destroy();
	void stop_n20();
	virtual void rescue();
	virtual void setN20_cosumption(float n20_cosumption);
	virtual void n20();
	virtual void addN20(float amount);
	virtual void setHudSpeed() final;
	virtual void update() = 0;
	virtual void updateHud(float &time, Vector2f viewCenter) = 0;
	virtual Vector2f getChassisOffsetView() = 0;
	virtual Vector2f getChassisCenter() = 0;
	virtual b2Vec2 getPosition(){ return body->GetPosition(); }
	virtual vector<b2Vec2> getWheelsPosition();
	virtual float getSpeed() final;
	float getN20(){ return n20_count; };
	virtual Hud * getHud() final { return hud; }
	b2Body * getBody(){ return body; }
};

Tank::Tank(b2World * world, RenderWindow * window, Hud &hud, unsigned wCount){
	this->world = world;
	this->window = window;
	this->hud = &hud;
	m_wheels.resize(wCount);
	m_springs.resize(wCount);
}

#ifdef debContact
void Tank::startContact(ContactType type, string contactData) {
	m_contacting = true;
	if (type == Body) this->contactType = "body ";
	if (type == Wheel) this->contactType = "wheel ";
	if (type == Gun) this->contactType = "gun ";
	this->contactBody = contactData;
}
#else
void Tank::startContact() {	m_contacting = true; }
#endif

void Tank::endContact() { m_contacting = false; }

void Tank::destroyBox(b2Body * box){
	box->SetUserData("bodyForDel");
	addN20(20.f);
	incBrokenBoxes();
}

void Tank::right() {
	for (auto spring : m_springs){
		spring->EnableMotor(true);
		spring->SetMotorSpeed(max_speed);
	}
}

void Tank::left() {
	for (auto spring : m_springs){
		spring->EnableMotor(true);
		spring->SetMotorSpeed(min_speed);
	}
}

void Tank::stop() {
	for (auto spring : m_springs) spring->EnableMotor(false);
}

void Tank::handBreak() {
	for (auto spring : m_springs) {
		spring->EnableMotor(true);
		spring->SetMotorSpeed(0);
	}
}

void Tank::destroy(){
	world->DestroyBody(body);
	for (auto wheel : m_wheels)
		world->DestroyBody(wheel);
}

void Tank::stop_n20(){
	for (auto spring : m_springs)
		spring->SetMaxMotorTorque(max_motor_torque);
}

void Tank::rescue(){
	float angle = body->GetAngle() * DEG;
	b2Vec2 bPos = body->GetPosition();
	bPos.y -= 4.f;
	if (angle > 90 || angle < -90 || roundf(getSpeed()) < 1.f)
		body->SetTransform(bPos, 0);
}

void Tank::setN20_cosumption(float n20_cosumption){ this->n20_cosumption = n20_cosumption; }

void Tank::n20(){
	if (n20_count != 0 && m_contacting){
		n20_count -= n20_cosumption;
		for (size_t i = 0; i < m_springs.size(); i++)
			m_springs[i]->SetMaxMotorTorque(max_motor_torque + n20addition);
	}
}

void Tank::addN20(float amount){
	n20_count += amount;
	if (n20_count > N20_LIMIT) 
		n20_count = N20_LIMIT;
}

void Tank::setHudSpeed() {
	hud->setBarPos(getSpeed());
}

vector<b2Vec2> Tank::getWheelsPosition(){
	vector<b2Vec2> wPos(m_wheels.size());
	for (size_t i = 0; i < wPos.size(); i++) {
		wPos[i] = m_wheels[i]->GetPosition();
		wPos[i].y += 1.85f;
	}
	return wPos;
}

float Tank::getSpeed() { 
	return sqrt(powf(body->GetLinearVelocityFromLocalPoint(body->GetLocalCenter()).x, 2) + powf(body->GetLinearVelocityFromLocalPoint(body->GetLocalCenter()).y, 2));
}