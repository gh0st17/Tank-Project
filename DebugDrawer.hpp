#include <SFML/Graphics.hpp>
#include <Box2D.h>

using namespace sf;

class DebugDrawer{
private:
	b2World * world;
	RenderWindow * window;
	bool drawEdge = 0, drawPolygon = 0, drawCircle = 0;

	b2Vec2* getPoints(b2Body* body, b2Vec2* vert, int c) {
		b2Vec2* v = new b2Vec2[c];
		for (int i = 0; i < c; i++) v[i] = body->GetWorldPoint(vert[i]);
		return v;
	}

	b2Vec2 getPoints(b2Body* body, b2Vec2 vert) {
		b2Vec2 v = body->GetWorldPoint(vert);
		return v;
	}

public:
	DebugDrawer(b2World * world, RenderWindow * window){
		this->world = world;
		this->window = window;
	}

	void setDrawEdge(bool drawEdge){ this->drawEdge = drawEdge; }
	void setDrawPolygon(bool drawPolygon){ this->drawPolygon = drawPolygon; }
	void setDrawCircle(bool drawCircle){ this->drawCircle = drawCircle; }

	void update(){
		if (drawEdge || drawPolygon || drawCircle)
			for (b2Body* bi = world->GetBodyList(); bi != 0; bi = bi->GetNext())
				for (b2Fixture* f = bi->GetFixtureList(); f; f = f->GetNext()) {
					b2Shape::Type shapeType = f->GetType();
					if (shapeType == b2Shape::e_circle && drawCircle) {
						drawingCircle(bi, f);
					}
					else if (shapeType == b2Shape::e_polygon && drawPolygon) {
						drawingPolygon(bi, f);
					}
					else if (shapeType == b2Shape::e_edge && drawEdge) {
						drawingEdge(bi, f);
					}
				}
	}

	void drawingEdge(b2Body * bi, b2Fixture * f){
		b2EdgeShape* poly = (b2EdgeShape*)f->GetShape();
		b2Vec2 pos[4];
		pos[0] = getPoints(bi, poly->m_vertex0);
		pos[1] = getPoints(bi, poly->m_vertex1);
		pos[2] = getPoints(bi, poly->m_vertex2);
		pos[3] = getPoints(bi, poly->m_vertex3);
		for (int i = 0; i < 4; i++){
			VertexArray lines(sf::LinesStrip, 2);
			if (i != 3) {
				lines[0] = sf::Vector2f(pos[i].x * SCALE, pos[i].y * SCALE);
				lines[1] = sf::Vector2f(pos[i + 1].x * SCALE, pos[i + 1].y * SCALE);
			}
			else {
				lines[0] = sf::Vector2f(pos[i].x * SCALE, pos[i].y * SCALE);
				lines[1] = sf::Vector2f(pos[0].x * SCALE, pos[0].y * SCALE);
			}
			lines[0].color = sf::Color::Magenta;
			lines[1].color = sf::Color::Magenta;
			sf::RectangleShape rectangle;
			rectangle.setSize(sf::Vector2f(5, 5));
			rectangle.setFillColor(sf::Color::Magenta);
			rectangle.setPosition(pos[i].x * SCALE - 2.5f, pos[i].y * SCALE - 2.5f);
			window->draw(rectangle);
			window->draw(lines);
		}
	}

	void drawingPolygon(b2Body * bi, b2Fixture * f){
		b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
		b2Vec2* pos = getPoints(bi, poly->m_vertices, poly->m_count);
		for (int i = 0; i < poly->m_count; i++){
			VertexArray lines(sf::LinesStrip, 2);
			if (i != poly->m_count - 1) {
				lines[0] = sf::Vector2f(pos[i].x * SCALE, pos[i].y * SCALE);
				lines[1] = sf::Vector2f(pos[i + 1].x * SCALE, pos[i + 1].y * SCALE);
			}
			else {
				lines[0] = sf::Vector2f(pos[i].x * SCALE, pos[i].y * SCALE);
				lines[1] = sf::Vector2f(pos[0].x * SCALE, pos[0].y * SCALE);
			}
			lines[0].color = sf::Color::Red;
			lines[1].color = sf::Color::Red;
			sf::RectangleShape rectangle;
			rectangle.setSize(sf::Vector2f(5, 5));
			rectangle.setFillColor(sf::Color::Red);
			rectangle.setPosition(pos[i].x * SCALE - 2.5f, pos[i].y * SCALE - 2.5f);
			window->draw(rectangle);
			window->draw(lines);
		}
	}

	void drawingCircle(b2Body * bi, b2Fixture * f){
		b2CircleShape* cir = (b2CircleShape*)f->GetShape();
		b2Vec2 pos = bi->GetWorldPoint(cir->m_p);
		sf::RectangleShape rectangle;
		rectangle.setSize(sf::Vector2f(5, 5));
		rectangle.setFillColor(sf::Color::Blue);
		rectangle.setPosition(pos.x * SCALE - 2.5f, pos.y * SCALE - 2.5f);
		sf::CircleShape cirS;
		float r = (float)cir->m_radius * SCALE;
		cirS.setRadius(r);
		cirS.setPosition(pos.x * SCALE - r, pos.y * SCALE - r);
		cirS.setOutlineColor(sf::Color::Blue);
		cirS.setOutlineThickness(2.f);
		cirS.setFillColor(sf::Color::Transparent);
		window->draw(rectangle);
		window->draw(cirS);
	}
};