#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;
using namespace sf;

std::string imgFolder = "images/";
vector<Texture> textures = vector<Texture>(17, Texture());
vector<Sprite> sprites = vector<Sprite>(17, Sprite());

void initSprites(){
	for (unsigned i = 0; i < 17; i++) textures[i] = Texture();
	textures[0].loadFromFile(imgFolder + "tanks/VAZ2107/chassis.png");
	textures[1].loadFromFile(imgFolder + "tanks/VAZ2107/wheel.png");
	textures[2].loadFromFile(imgFolder + "tanks/VAZ2107/wheelArc.png");
	textures[3].loadFromFile(imgFolder + "tanks/Ural/chassis.png");
	textures[4].loadFromFile(imgFolder + "tanks/Ural/wheel.png");
	textures[5].loadFromFile(imgFolder + "tanks/Cent120/chassis.png");
	textures[6].loadFromFile(imgFolder + "tanks/Cent120/wheel.png");
	textures[7].loadFromFile(imgFolder + "tanks/Cent120/wheelArc.png");
	textures[8].loadFromFile(imgFolder + "tanks/Cent120/cab.png");
	textures[9].loadFromFile(imgFolder + "tanks/Cent120/gun.png");
	textures[10].loadFromFile(imgFolder + "hud/arrow.png");
	textures[11].loadFromFile(imgFolder + "hud/message.png");
	textures[12].loadFromFile(imgFolder + "hud/speedometer.png");
	textures[13].loadFromFile(imgFolder + "world/box.jpg");
	textures[14].loadFromFile(imgFolder + "world/box2.jpg");
	textures[15].loadFromFile(imgFolder + "world/platform.jpg");
	textures[16].loadFromFile(imgFolder + "world/start.jpg");
	for (unsigned i = 0; i < 17; i++) {
		textures[i].setSmooth(true);
		sprites[i].setTexture(textures[i]);
	}
	
	sprites[0].setOrigin(263, 65);
	sprites[1].setOrigin(36, 36);
	sprites[2].setOrigin(263, 65);

	sprites[3].setOrigin(423, 111 + 40);
	sprites[4].setOrigin(66, 66);

	sprites[5].setOrigin(720 / 2 + 60, 141 / 2 - 28);
	sprites[6].setOrigin(54, 54);
	sprites[7].setOrigin(720 / 2 + 2, 141 / 2 - 50);
	sprites[8].setOrigin(720 / 2 + 60, 141 / 2 + 75);
	sprites[9].setOrigin(20, 80);

	sprites[10].setOrigin(252, 252);
	sprites[11].setOrigin(600, 350);

	sprites[13].setOrigin(30, 30);
	sprites[14].setOrigin(30, 30);
	sprites[15].setOrigin(300, 30);
	sprites[16].setOrigin(60, 750);
}