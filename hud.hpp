#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "bar.hpp"

using namespace sf;

View view;

class Hud{
private:
	Texture tArrow, tSpeedometer;
	Sprite sArrow, sSpeedometer;
	Texture p; Sprite ps;
	Bar * n20Bar = new Bar(350, 0, Color::Blue);
	RenderWindow * window;
	Font font;
	Text fps, boxesText, text;
	unsigned long brokenBoxes = 0;
	void gdraw(){
		text.setOrigin(250, 150);
		text.setPosition(view.getCenter());
		ps.setPosition(view.getCenter());
		if ((double)(clock() - msg.timeg) / CLOCKS_PER_SEC <= 4){
			text.setString("\t\t  Hello, player!\n\t W and S - Moving\n\t\t\t D - Debug");
			window->draw(ps);
			window->draw(text);
		}
		else if ((double)(clock() - msg.timeg) / CLOCKS_PER_SEC <= 8){
			text.setString("\t\t\tP - Rescue\n  F1-F3 change vehicle\n\tSpace - Handbreak");
			window->draw(ps);
			window->draw(text);
		}
		else if ((double)(clock() - msg.timeg) / CLOCKS_PER_SEC <= 12){
			text.setString(" LShift - Acceleration\n\tRMB - Slowmotion\n\t\t\tEsc - close");
			window->draw(ps);
			window->draw(text);
		}
		else {
			p_draw = false;
			text.setOrigin(250, 125);
		}
	}
	void pdraw(){
		text.setString("\tPlatforms can help,\nbut can be a hindrance");
		text.setPosition(view.getCenter());
		ps.setPosition(view.getCenter());
		if ((double)(clock() - msg.timep) / CLOCKS_PER_SEC <= 3){
			window->draw(ps);
			window->draw(text);
		}
		else p_draw = false;
	}
	void bdraw(){
		text.setString("Crush the boxes to get\n\textra acceleration");
		text.setPosition(view.getCenter());
		ps.setPosition(view.getCenter());
		if ((double)(clock() - msg.timeb) / CLOCKS_PER_SEC <= 3){
			window->draw(ps);
			window->draw(text);
		}
		else p_draw = false;
	}

public:
	Hud(RenderWindow * window){
		this->window = window;
		tArrow.loadFromFile("images/hud/arrow.png");
		tSpeedometer.loadFromFile("images/hud/speedometer.png");
		tArrow.setSmooth(1);
		tSpeedometer.setSmooth(1);
		sArrow.setTexture(tArrow);
		sSpeedometer.setTexture(tSpeedometer);
		sArrow.setOrigin(252, 252);
		font.loadFromFile("res/SF-Pro-Display-Bold.otf");
		fps.setFont(font); fps.setCharacterSize(19 * (unsigned)ZOOM_FACTOR); fps.setScale(ZOOM_FACTOR, ZOOM_FACTOR); fps.setOutlineThickness(2.0f);
		boxesText.setFont(font); boxesText.setCharacterSize((unsigned)(7.5f * ZOOM_FACTOR)); boxesText.setScale(ZOOM_FACTOR, ZOOM_FACTOR); boxesText.setFillColor(Color::Black);
		
		p.loadFromFile("images/hud/message.png");
		p.setSmooth(true);
		ps.setTexture(p);
		ps.setOrigin(600, 350);
		text.setOrigin(250, 125);
		ps.setPosition(view.getCenter());
		text.setLetterSpacing(1.25);
		text.setFillColor(Color::Color(0, 0, 0, 140));
		text.setStyle(Text::Style::Bold);
		text.setFont(font); text.setCharacterSize(23 * (unsigned)ZOOM_FACTOR); text.setScale(ZOOM_FACTOR, ZOOM_FACTOR);
	}

	void update(float n20_LIMIT, float n20_count){
		n20Bar->setPercentage(100.f / n20_LIMIT * n20_count);
		window->draw(sSpeedometer);
		n20Bar->update((*window));
		std::stringstream ss;
		ss << roundf(n20Bar->getPercentage() * 10) / 10 << '%';
		boxesText.setString(ss.str());
		window->draw(boxesText);
		window->draw(sArrow);
		if (p_draw) pdraw();
		else if (b_draw) bdraw();
		else if (g_draw) gdraw();
	}
	void updateDebug(float &time, Vector2f viewCenter, string debugString){
		fps.setString("FPS: " + to_string((unsigned)(1.0f / time)) + "\nBoxes: " +
			to_string(brokenBoxes) + "\n" + debugString);
		fps.setPosition(viewCenter.x - 600 * ZOOM_FACTOR, viewCenter.y - 360 * ZOOM_FACTOR - 20);
		window->draw(fps);
	}
	void setBarPos(float x, float y, float32 speed){
		n20Bar->setBarPos(x + 145, y + 380);
		sArrow.setPosition(x + 252, y + 252);
		sSpeedometer.setPosition(x, y);
		sArrow.setRotation(1.8f * speed);
		boxesText.setPosition(x + 205, y + 326);
	}
	void incBoxesBroken(){ brokenBoxes++; }

	bool p_draw = false, b_draw = false, g_draw = false;
};