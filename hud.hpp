#include <SFML/Graphics.hpp>
#include <Box2D.h>
#include "bar.hpp"

using namespace sf;

View view;

class Hud{
private:
	Bar * n20Bar = new Bar(350, 0, Color::Blue);
	RenderWindow * window;
	Font font;
	Text fps, boxesText, text;
	float factor = 1.f;
	unsigned long brokenBoxes = 0;
	void gdraw(){
		text.setOrigin(250, 150);
		text.setPosition(view.getCenter());
		sprites[11].setPosition(view.getCenter());
		if ((double)(clock() - msg.timeg) / CLOCKS_PER_SEC <= 4)
			text.setString("\t\t  Hello, player!\n\t W and S - Moving\n\t\t\t D - Debug");
		else if ((double)(clock() - msg.timeg) / CLOCKS_PER_SEC <= 8)
			text.setString("\t\t\tP - Rescue\n  F1-F3 change vehicle\n\tSpace - Handbreak");
		else if ((double)(clock() - msg.timeg) / CLOCKS_PER_SEC <= 12)
			text.setString("\t PageUp - Zoom in\nPageDown - Zoom out\n\tHome - Reset zoom");
		else if ((double)(clock() - msg.timeg) / CLOCKS_PER_SEC <= 16)
			text.setString(" LShift - Acceleration\n\tRMB - Slowmotion\n\t\t\tEsc - close");
		else {
			g_draw = false;
			text.setOrigin(250, 125);
		}
		if (g_draw){
			window->draw(sprites[11]);
			window->draw(text);
		}
	}
	void pdraw(){
		text.setString("\tPlatforms can help,\nbut can be a hindrance");
		text.setPosition(view.getCenter());
		sprites[11].setPosition(view.getCenter());
		if ((double)(clock() - msg.timep) / CLOCKS_PER_SEC <= 3){
			window->draw(sprites[11]);
			window->draw(text);
		}
		else p_draw = false;
	}
	void bdraw(){
		text.setString("Crush the boxes to get\n\textra acceleration");
		text.setPosition(view.getCenter());
		sprites[11].setPosition(view.getCenter());
		if ((double)(clock() - msg.timeb) / CLOCKS_PER_SEC <= 3){
			window->draw(sprites[11]);
			window->draw(text);
		}
		else b_draw = false;
	}
	void grdraw(){
		text.setString("  Blue box make Moon\n gravity for 30 seconds");
		text.setPosition(view.getCenter());
		sprites[11].setPosition(view.getCenter());
		if ((double)(clock() - msg.timegr) / CLOCKS_PER_SEC <= 4){
			window->draw(sprites[11]);
			window->draw(text);
		}
		else gr_draw = false;
	}

public:
	Hud(RenderWindow * window){
		this->window = window;
		font.loadFromFile("res/SF-Pro-Display-Bold.otf");
		fps.setFont(font); fps.setCharacterSize(19 * (unsigned)ZOOM_FACTOR); fps.setScale(ZOOM_FACTOR, ZOOM_FACTOR); fps.setOutlineThickness(2.0f);
		boxesText.setFont(font); boxesText.setCharacterSize((unsigned)(7.5f * ZOOM_FACTOR)); boxesText.setScale(ZOOM_FACTOR, ZOOM_FACTOR); boxesText.setFillColor(Color::Black);
		
		sprites[12].setPosition(view.getCenter());
		text.setOrigin(250, 125);
		text.setLetterSpacing(1.25);
		text.setFillColor(Color::Color(0, 0, 0, 140));
		text.setStyle(Text::Style::Bold);
		text.setFont(font); text.setCharacterSize(23 * (unsigned)ZOOM_FACTOR); text.setScale(ZOOM_FACTOR, ZOOM_FACTOR);
	}

	void update(float n20_LIMIT, float n20_count){
		n20Bar->setPercentage(100.f / n20_LIMIT * n20_count);
		window->draw(sprites[12]);
		n20Bar->update((*window));
		std::stringstream ss;
		ss << roundf(n20Bar->getPercentage() * 10) / 10 << '%';
		boxesText.setString(ss.str());
		window->draw(boxesText);
		window->draw(sprites[10]);
		if (p_draw) pdraw();
		else if (b_draw) bdraw();
		else if (g_draw) gdraw();
		else if (gr_draw) grdraw();
	}
	void updateDebug(float &time, Vector2f viewCenter, string str){
		string grav = "";
		if (msg.gravity && 30.f - (clock() - msg.timeGravity) / CLOCKS_PER_SEC > 0)
			grav = "\nGravity remain: " + to_string(30.f - (clock() - msg.timeGravity) / CLOCKS_PER_SEC);
		fps.setString("FPS: " + to_string((unsigned)(1.0f / time)) + "\nBoxes: " +
			to_string(brokenBoxes) + str + grav);
		fps.setPosition((viewCenter.x - view.getSize().x / 2) + (10 * factor), (viewCenter.y - view.getSize().y / 2) - (10 * factor));
		window->draw(fps);
	}
	void setBarPos(float32 speed){
		Vector2f center = view.getCenter();
		Vector2f offset = Vector2f(600.f, 125.f);
		n20Bar->setBarPos((center + offset).x + 145, (center + offset).y + 380);
		sprites[10].setPosition((center + offset).x + 252, (center + offset).y + 252);
		sprites[12].setPosition((center + offset).x, (center + offset).y);
		sprites[10].setRotation(1.8f * speed);
		boxesText.setPosition((center + offset).x + 205, (center + offset).y + 326);
	}
	void incBoxesBroken(){ brokenBoxes++; }

	bool p_draw = false, b_draw = false, g_draw = false, gr_draw;

	void setScale(float factor){
		fps.setScale(factor * fps.getScale());
		text.setScale(factor * text.getScale());
		/*boxesText.setScale(factor * boxesText.getScale());
		sArrow.setScale(factor * sArrow.getScale());
		sSpeedometer.setScale(factor * sSpeedometer.getScale());
		n20Bar->setScale(factor);*/
		sprites[11].setScale(factor * sprites[11].getScale());
		this->factor = factor;
	}

	void resetScale(){
		fps.setScale(2.f, 2.f);
		text.setScale(2.f, 2.f);
		/*boxesText.setScale(2.f, 2.f);
		sArrow.setScale(1.f, 1.f);
		sSpeedometer.setScale(1.f, 1.f);
		n20Bar->resetScale();*/
		sprites[11].setScale(1.f, 1.f);
	}
};