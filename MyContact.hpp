#include <SFML/Audio.hpp>
#include "tank.hpp"

Tank * player;

class MyContact : public b2ContactListener{
	SoundBuffer sb;
	Sound * sound;
public:
	MyContact(){
		sb.loadFromFile("sound/boxcrash.wav");
	}
	~MyContact(){
		delete sound;
		sb.~SoundBuffer();
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

	void BeginContact(b2Contact* contact) {
		b2Body *bA = contact->GetFixtureA()->GetBody();
		b2Body *bB = contact->GetFixtureB()->GetBody();

		string bodyUserDataA = static_cast<string>((char*)bA->GetUserData());
		string bodyUserDataB = static_cast<string>((char*)bB->GetUserData());

		if (msg.fplatform){
			if ((bodyUserDataA == "wheel" && bodyUserDataB == "platform") ||
				(bodyUserDataB == "wheel" && bodyUserDataA == "platform")){
				platformWelcome();
			}
		}

		if (msg.fground){
			if ((bodyUserDataA == "wheel" && bodyUserDataB == "ground") ||
				(bodyUserDataB == "wheel" && bodyUserDataA == "ground")){
				groundWelcome();
			}
		}

		if (bodyUserDataA == "wheel" && bodyUserDataB == "box"){
			player->destroyBox(bB);
			sound = new Sound(sb);
			sound->play();
			if (msg.fbox) boxWelcome();
		}
		else if (bodyUserDataB == "wheel" && bodyUserDataA == "box"){
			player->destroyBox(bA);
			sound = new Sound(sb);
			sound->play();
			if (msg.fbox) boxWelcome();
		}
	}

	void EndContact(b2Contact* contact) {
		void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
		if (bodyUserData == "body" || bodyUserData == "wheel" || bodyUserData == "box")
			player->endContact();

		bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
		if (bodyUserData == "body" || bodyUserData == "wheel" || bodyUserData == "box")
			player->endContact();
	}
};