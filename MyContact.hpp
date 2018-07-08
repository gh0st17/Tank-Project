#include <SFML/Audio.hpp>
#include "tank.hpp"

shared_ptr<Tank> player;

class MyContact : public b2ContactListener{
	SoundBuffer sb;
	Sound sound;
public:
	MyContact(){
		sb.loadFromFile("sound/boxcrash.wav");
		sound.setBuffer(sb);
		sound.setVolume(15.f);
	}
	void platformWelcome(){
		msg.timep = clock();
		player->getHud()->p_draw = 1;
		msg.fplatform = false;
	}

	void boxWelcome(){
		msg.timeb = clock();
		player->getHud()->b_draw = 1;
		msg.fbox = false;
	}

	void groundWelcome(){
		msg.timeg = clock();
		player->getHud()->g_draw = 1;
		msg.fground = false;
	}

	void gravityWelcome(){
		msg.timegr = clock();
		player->getHud()->gr_draw = 1;
		msg.fgravity = false;
	}

	void BeginContact(b2Contact* contact) {
		b2Body *bA = contact->GetFixtureA()->GetBody();
		b2Body *bB = contact->GetFixtureB()->GetBody();

		string bodyUserDataA = static_cast<string>((char*)bA->GetUserData());
		string bodyUserDataB = static_cast<string>((char*)bB->GetUserData());

		if ((bodyUserDataA == "wheel" && bodyUserDataB == "gravityBox") ||
			(bodyUserDataB == "wheel" && bodyUserDataA == "gravityBox")){
			msg.timeGravity = clock();
			msg.gravity = true;
			if (msg.fgravity) gravityWelcome();

			if (bodyUserDataB == "gravityBox"){
				player->destroyBox(bB); sound.play();
			}
			else if (bodyUserDataA == "gravityBox"){
				player->destroyBox(bA); sound.play();
			}
		}

		if (msg.fplatform){
			if ((bodyUserDataA == "wheel" && bodyUserDataB == "platform") ||
				(bodyUserDataB == "wheel" && bodyUserDataA == "platform")){
				platformWelcome();
			}
		}
		
		if (bodyUserDataA == "wheel" || bodyUserDataB == "wheel")
			player->startContact();

		if (msg.fground){
			if ((bodyUserDataA == "wheel" && bodyUserDataB == "ground") ||
				(bodyUserDataB == "wheel" && bodyUserDataA == "ground")){
				groundWelcome();
			}
		}

		if (bodyUserDataA == "wheel" && bodyUserDataB == "box"){
			player->destroyBox(bB); sound.play();
			if (msg.fbox) boxWelcome();
		}
		else if (bodyUserDataB == "wheel" && bodyUserDataA == "box"){
			player->destroyBox(bA); sound.play();
			if (msg.fbox) boxWelcome();
		}
	}

	void EndContact(b2Contact* contact) {
		b2Body *bA = contact->GetFixtureA()->GetBody();
		b2Body *bB = contact->GetFixtureB()->GetBody();

		string bodyUserDataA = static_cast<string>((char*)bA->GetUserData());
		string bodyUserDataB = static_cast<string>((char*)bB->GetUserData());

		if (bodyUserDataA == "wheel" || bodyUserDataB == "wheel")
			player->endContact();
	}
};