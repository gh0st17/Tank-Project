#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <ctime>
#include "global.hpp"
#include "hud.hpp"
#include "DebugDrawer.hpp"
#include "bullet.hpp"

using namespace sf;
using namespace std;
#define debContact

class Tank {
public:
	enum ContactType {
		Wheel,
		Body,
		Gun
	};
	vector<b2Body*> boxDel;
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
	vector<b2WheelJoint*> m_springs = vector<b2WheelJoint*>(0);
	Texture tChassis, tWheel, wArc, tCab, tGun;
	Sprite wheelArc, sCab, sChassis, sWheel, sGun;
	b2RevoluteJointDef rjd;
	float n20_count;
	const float N20_LIMIT = 0;
	Hud * hud;

public:
	Tank(b2World * world, RenderWindow * window, Hud &hud, unsigned wCount);
	void startContact(ContactType type, string contactData);
	void endContact();
	void incBrokenBoxes(){ hud->incBoxesBroken(); }
	virtual void setBody(b2Vec2 pos){};
	virtual void update(){};
	virtual void updateHud(float &time, Vector2f viewCenter, Vector2f mouse_pos){};
	virtual void setN20_cosumption(float n20_cosumption){};
	virtual void Tank::right() {
		for (auto spring : m_springs){
			spring->EnableMotor(true);
			spring->SetMotorSpeed(max_speed);
		}
	}
	virtual void Tank::left() {
		for (auto spring : m_springs){
			spring->EnableMotor(true);
			spring->SetMotorSpeed(min_speed);
		}
	}
	virtual void Tank::stop() {
		for (auto spring : m_springs) spring->EnableMotor(false);
	}
	virtual void Tank::handBreak() {
		for (auto spring : m_springs) {
			spring->EnableMotor(true);
			spring->SetMotorSpeed(0);
		}
	}
	virtual void n20(){};
	virtual void destroy(){
		world->DestroyBody(body);
		for (auto wheel : m_wheels){
			world->DestroyBody(wheel);
		}
	}
	void stop_n20(){
		for (auto spring : m_springs)
			spring->SetMaxMotorTorque(max_motor_torque);
	}
	virtual void rescue(){
		float angle = body->GetAngle() * DEG;
		b2Vec2 bPos = body->GetPosition();
		bPos.y -= 4.f;
		if (angle > 90 || angle < -90 || roundf(getSpeed()) < 1.f)
			body->SetTransform(bPos, 0);
	}
	virtual void setGunAngle(Vector2f mouseP){};
	virtual void addN20(float amount){};
	float getN20(){ return n20_count; };
	virtual void setHudPos(Vector2f pos){};
	virtual Vector2f getChassisCenter() = 0;
	virtual b2Vec2 getPosition() = 0;
	virtual std::vector<b2Vec2> getWheelsPosition(){
		std::vector<b2Vec2> wPos(m_wheels.size());
		for (size_t i = 0; i < wPos.size(); i++) {
			wPos[i] = m_wheels[i]->GetPosition();
			wPos[i].y += 1.85f;
		}
		return wPos;
	}
	virtual float getSpeed() = 0;
	virtual float getAngle(Vector2f &mouseP) = 0;
	void destroyBox(b2Body * box){
		boxDel.push_back(box);
		addN20(20.f);
		incBrokenBoxes();
	}
	Hud * getHud(){ return hud; }
};

Tank::Tank(b2World * world, RenderWindow * window, Hud &hud, unsigned wCount){
	this->world = world;
	this->window = window;
	this->hud = &hud;
	m_wheels.resize(wCount);
	m_springs.resize(wCount);
}
void Tank::startContact(ContactType type, string contactData) {
	m_contacting = true;
#ifdef debContact
	if (type == Body) this->contactType = "body ";
	if (type == Wheel) this->contactType = "wheel ";
	if (type == Gun) this->contactType = "gun ";
	this->contactBody = contactData;
#endif
}
void Tank::endContact() { m_contacting = false; }