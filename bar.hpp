#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

class Bar {
private:
	bool visible = 1;
	float percentage = 0, x, y, w, h;
	RectangleShape backRect, colorRect;

public:
	Bar(float X, float Y, Color color){
		setWidth(200); setHeight(30);
		x = X; y = Y;
		colorRect.setFillColor(color);
		backRect.setSize(Vector2f(w + 3, h));
		colorRect.setSize(Vector2f(w - 4, h - 4));
		backRect.setPosition(x, y);
		colorRect.setPosition(x + 2, y + 2);
	}

	void setBarPos(float X, float Y){
		backRect.setPosition(X + 1, Y);
		colorRect.setPosition(X + 2, Y + 2);
	}

	void update(RenderWindow &window){
		if (percentage > 0.f){
			backRect.setFillColor(Color::Magenta);
			float offset = percentage * (w / 100.f);
			Vector2f size = colorRect.getSize();
			size.x = offset;
			colorRect.setSize(size);
		}
		else if (percentage == 0.f) backRect.setFillColor(Color::Red);
		else if (percentage < 0.f) percentage = 0.f;

		window.draw(backRect);
		window.draw(colorRect);
	}

	float getPercentage(){ return percentage; }
	void setPercentage(float p){ 
		percentage = p; }
	bool getVisible(){ return visible; }
	void setVisible(bool v){ visible = v; }
	void setX(float f){ x = f; }
	void setY(float f){ y = f; }
	void setWidth(float f){ w = f; }
	void setHeight(float f){ h = f; }
};