#include <SFML/Graphics.hpp>
#include <Box2D.h>
#include "global.hpp"


class World {
private:
	const unsigned w = 1200, h = 720;
	const string shaderData = \
		"uniform vec2 storm_position;"\
		"uniform float storm_total_radius;"\
		"uniform float storm_inner_radius;"\
		"void main(){"\
			"vec4 vertex = gl_ModelViewMatrix * gl_Vertex;"\
			"vec2 offset = vertex.xy - storm_position;"\
			"float len = length(offset);"\
			"if (len < storm_total_radius) {"\
				"float push_distance = storm_inner_radius + len / storm_total_radius * (storm_total_radius - storm_inner_radius);"\
				"vertex.xy = storm_position + normalize(offset) * push_distance;"\
			"}"\
			"gl_Position = gl_ProjectionMatrix * vertex;"\
			"gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"\
			"gl_FrontColor = gl_Color;"\
		"}";
	Shader shader;
	RenderStates states;
	VertexArray m_points;
	Vector2f backPos;
	vector<Vector2f> m_xy;

	unsigned py_i = 0, s_i = 1, draw_pressed = 50;
	float step = 0, last, prevMoveBack = 0, nextMoveBack = 0;
	float32 x = 0, y = 1;
	b2Body* start;
	b2PolygonShape startSh;
	b2BodyDef bdef;
	b2BodyDef bdefGr;
	b2FixtureDef fdef, boxfdef;
	b2EdgeShape shape;
	std::vector<VertexArray> groundArr;
	RenderWindow * window;
	ConvexShape cs;
	vector<b2Body*> ground = vector<b2Body*>(1, NULL);
	b2Body* m_platformBody2;
	Font font;
	Text meterText;
	bool startUpd = 0, waitForDelete = false, moveBack = true;
	sf::Clock clok;

	void addVertex(float &x, float dx, float &y, float y2, Color color); // Vertexes for drawing
	float Noise(float x, float y); // Perlin Noise algorithm
	float SmoothNoise_2D(float x, float y);
	void drawVertex(Vector2f * center);
	void initBoxfdef();
	void initBackground();
	void drawBodies(b2Body * it);
	void drawGround();

public:
	vector<b2Vec2> py; // Position coords for platform
	b2World * m_world;
	World(RenderWindow * window){
		this->window = window;
		b2Vec2 gravity(0.f, 19.6f);
		m_world = new b2World(gravity);
		m_world->SetContinuousPhysics(true);
		cs.setFillColor(Color::Color(165, 89, 8));
		cs.setPointCount(4);
		bdefGr.userData = "ground";
		startSh.SetAsBox(2, 25);
		bdef.position.Set(2, 7.5);
		bdef.userData = "start";
		bdef.bullet = true;
		start = m_world->CreateBody(&bdef);
		b2Fixture* startf = start->CreateFixture(&startSh, 0);
		bdef.position.Set(0, 0);
		font.loadFromFile("res/SF-Pro-Display-Bold.otf");
		meterText.setFont(font);
		meterText.setCharacterSize(19 * (unsigned)ZOOM_FACTOR);
		meterText.setScale(ZOOM_FACTOR, ZOOM_FACTOR);
		meterText.setOutlineThickness(2.0f);

		initBoxfdef();
		initBackground();
	}
	~World(){}

	void genBoxes(bool &grGen, float x, float y, float w, float h, b2BodyType type = b2_staticBody) {
		srand(time(NULL));
		b2PolygonShape boxShape;
		boxShape.SetAsBox(w / SCALE, h / SCALE);
		bdef.type = type;
		bdef.position.Set(x / SCALE, y / SCALE);
		boxfdef.shape = &boxShape;

		b2Body *b_ground = m_world->CreateBody(&bdef);
		b_ground->CreateFixture(&boxfdef);
		b_ground->SetUserData("box");
		if (rand() % 101 >= 80 && !grGen){
			b_ground->SetUserData("gravityBox");
			grGen = true;
		}
	}
	void genBlocks(bool &grGen, unsigned count, b2Vec2 startPos) {
		const float w = 30.f, offsetX = 1.f, offsetY = 0.4f;
		for (size_t c = 0; c < count; c++)
			for (size_t i = 0; i < count; i++) genBoxes(grGen, (startPos.x + (i / SCALE * w)) * SCALE,
				startPos.y * SCALE,
				w, w, b2_dynamicBody);
	}
	void generateMap(bool first = false){
		float32 dx = 10.f, y2;
		bool grGen = false;
		srand(time(NULL)); // Set random seed
		fdef.shape = &shape; // Params of ground
		fdef.density = 0.f;
		fdef.friction = 0.9f;
		fdef.restitution = 0;

		if (fmod(x, 400) == 0.0 || x == 0){ // Moving meter text
			meterText.setString(to_string((unsigned)x) + 'm');
			meterText.setPosition(x * SCALE, y * SCALE - 150);
		}

		if (first) last = x; // last for start limiter
		else last = x - 200;

		if (ground.size() > 2) { // if patrs of ground > 2 then delete first
			m_world->DestroyBody(ground.front());
			ground.erase(ground.begin());
			for (unsigned i = 0; i < 20; i++) groundArr.erase(groundArr.begin());
		}

		ground.back() = m_world->CreateBody(&bdefGr); // Add part of new ground to vector
		for (int k = 0; k < 4; k++) { // Generate logic
			for (int i = 0; i < 5; i++) {
				y2 = SmoothNoise_2D(rand() % 57, rand() % 15731) * 25.f;

				if (fabs(y2 - y) > 3.5f) { // if differents between last and next y > 3.5f
					if (y2 > y)		 while (fabs(y2 - y) > 3.5f) y2 -= .5f;
					else if (y2 < y) while (fabs(y2 - y) > 3.5f) y2 += .5f;
				}

				if		(y2 >  50) while (y2 >  50) y2 -= 2.5f - (rand() % 6);
				else if (y2 < -50) while (y2 < -50) y2 += 2.5f - (rand() % 6);

				shape.Set(b2Vec2(x, y), b2Vec2(x + dx, y2)); // Set a new fixture
				addVertex(x, dx, y, y2, Color::Color(165, 89, 8));
				ground.back()->CreateFixture(&fdef);
				y = y2; // Set last x y
				x += dx;
				if ((unsigned)x % 150 == 0) genBlocks(grGen, 3, b2Vec2(x, y - 3.f));
			}
			if (k == 0) py.push_back(b2Vec2(x + 10, y - 5));
		}
		//setup platform shape
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(10, 1);

		b2BodyDef bodyDef;
		bodyDef.type = b2_kinematicBody;
		bodyDef.bullet = true;
		bodyDef.position.Set(py.back().x, py.back().y);
		bodyDef.angle = ((-15 + rand() % 31) / DEG);
		bodyDef.userData = "platform";
		m_platformBody2 = m_world->CreateBody(&bodyDef);
		b2Fixture* platformFixture2 = m_platformBody2->CreateFixture(&polygonShape, 0);

		startUpd = 1; // it allows move start limiter
		ground.push_back(NULL); // Add elem for next ground
	}
	b2World * getWorld() { return m_world; }
	void drawBackground(){
		bool draw = false;
		if (Keyboard::isKeyPressed(Keyboard::W)){
			if (Keyboard::isKeyPressed(Keyboard::LShift)) draw = true;
			else draw = false;
		}
		float radius;
		Vector2f centerp = player->getChassisCenter();

		if (draw && draw_pressed < 120) draw_pressed++;
		if (!draw && draw_pressed > 50) draw_pressed--;

		radius = 400 + cos(clok.getElapsedTime().asSeconds()) * (draw_pressed + player->getSpeed());
		shader.setUniform("storm_inner_radius", radius / 5);
		shader.setUniform("storm_position", centerp);
		shader.setUniform("storm_total_radius", radius);

		Vector2f center = view.getCenter();
		Vector2f center2 = center;
		center.x += ((w << 1) * s_i) - centerp.x / 5;
		center.y += h * 2.75f / 2;
		drawVertex(new Vector2f(center.x - (w << 1), center.y));
		drawVertex(&center);
		center.x += w << 1;
		drawVertex(&center);
		if (centerp.x + (center2.x + w - centerp.x) > center.x) s_i++;
		center.x += w << 1;
		drawVertex(&center);
	}

	void update(float freq, Vector2f _view){
		m_world->Step(1 / freq, 8, 3);
		// Platform
		float theta = 0.025f + step;
		step += 0.015f;
		if (step == 1.0f) step = 0.0f;
		for (auto _py : py){
			b2Vec2 targetPos(_py.x, _py.y - 5.55f * cosf(theta));
			m_platformBody2->SetLinearVelocity(60 * (targetPos - m_platformBody2->GetPosition()));
		}

		if (x - player->getPosition().x < 100.f) generateMap();

		for (b2Body* it = m_world->GetBodyList(); it != 0; it = it->GetNext()){
			for (b2Fixture* f = it->GetFixtureList(); f != 0; f = f->GetNext()){ // Destuction bodies
				if ((it->GetUserData() == "box" ||
					it->GetUserData() == "platform" ||
					it->GetUserData() == "gravityBox") &&
					player->getPosition().x - it->GetPosition().x > 60) {
					m_world->DestroyBody(it);
					it = m_world->GetBodyList();
					f = it->GetFixtureList();
				}

				if (it->GetUserData() == "bodyForDel") {
					m_world->DestroyBody(it);
					it = m_world->GetBodyList();
					f = it->GetFixtureList();
				}

				if (startUpd && it->GetUserData() == "start") { // Recreate start limiter at new position
					m_world->DestroyBody(it);
					it = m_world->GetBodyList();
					f = it->GetFixtureList();
					bdef.position.Set(last + 2, y - 7.5f);
					bdef.type = b2_staticBody;
					b2FixtureDef fstart;
					fstart.shape = &startSh;
					start = m_world->CreateBody(&bdef);
					start->CreateFixture(&fstart);
					start->SetUserData("start");
					startUpd = 0;
				}
			}
			sprites[11].setPosition(_view.x - 850 * ZOOM_FACTOR, _view.y - 531 * ZOOM_FACTOR);
			drawBodies(it);
		}

		drawGround();

		if (msg.gravity){
			m_world->SetGravity(b2Vec2(0.f, 4.6f));
			if ((double)(clock() - msg.timeGravity) / CLOCKS_PER_SEC > 30){
				msg.gravity = false;
				m_world->SetGravity(b2Vec2(0.f, 19.6f));
			}
		}
		if (player->getPosition().y - y > 25.f){
			b2Vec2 p_pos = player->getPosition();
			p_pos.y = y - 25.f;
			player->getBody()->SetTransform(p_pos, 0);
		}
		for (b2Joint * j = m_world->GetJointList(); j != 0; j = j->GetNext())
			if (j->GetUserData() == "jointForDel") m_world->DestroyJoint(j);
	}
};

void World::addVertex(float &x, float dx, float &y, float y2, Color color){
	groundArr.push_back(VertexArray(PrimitiveType::LinesStrip, 2));
	groundArr[groundArr.size() - 1][0] = Vector2f(x * SCALE, y * SCALE);
	groundArr[groundArr.size() - 1][1] = Vector2f((x + dx) * SCALE, y2 * SCALE);
	groundArr[groundArr.size() - 1][0].color = color;
	groundArr[groundArr.size() - 1][1].color = color;
}

float World::Noise(float x, float y){
	unsigned __int64 n = x + y * 57;
	n = (n << 13) ^ n;
	return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float World::SmoothNoise_2D(float x, float y){
	float corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
	float sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
	float center = Noise(x, y) / 2;
	return corners + sides + center;
}

void World::drawVertex(Vector2f * center){
	for (unsigned i = 0; i < m_points.getVertexCount(); i++){
		m_points[i].position = *center - m_xy[i];
	}
	window->draw(m_points, states);
}

void World::initBoxfdef(){
	boxfdef.density		= 3.f;
	boxfdef.friction	= .5f;
	boxfdef.restitution = 0.f;
	boxfdef.userData = "boxFixture";
}

void World::initBackground(){
	m_points.setPrimitiveType(Points);
	srand(time(NULL));
	for (int i = 0; i < 1750; ++i){
		m_xy.push_back(Vector2f(rand() % ((unsigned)(w * 2.f)), rand() % ((unsigned)(h * 2.75f))));
		vector<Color> colors;
		colors.push_back(Color(255, 0, 0));
		colors.push_back(Color(0, 255, 0));
		colors.push_back(Color(0, 0, 255));
		colors.push_back(Color(255, 255, 255));
		m_points.append(Vertex(Vector2f(view.getCenter().x, view.getCenter().y), colors[rand() % 4]));
	}
	shader.loadFromMemory(shaderData, Shader::Vertex);
	states.shader = &shader;
}

void World::drawBodies(b2Body * it){
	b2Vec2 pos = it->GetPosition();
	float angle = it->GetAngle() * DEG;
	sf::Vector2f sfPos(pos.x*SCALE, pos.y*SCALE);

	/* Drawing */
	if (it->GetUserData() == "box") {
		sprites[13].setPosition(sfPos);
		sprites[13].setRotation(angle);
		window->draw(sprites[13]);
	}

	if (it->GetUserData() == "gravityBox") {
		sprites[14].setPosition(sfPos);
		sprites[14].setRotation(angle);
		window->draw(sprites[14]);
	}

	if (it->GetUserData() == "platform") {
		sprites[15].setPosition(sfPos);
		sprites[15].setRotation(angle);
		window->draw(sprites[15]);
	}

	if (it->GetUserData() == "start") {
		sprites[16].setPosition(sfPos);
		sprites[16].setRotation(angle);
		window->draw(sprites[16]);
	}
}

void World::drawGround(){
	for (auto grArr : groundArr) {
		window->draw(grArr);
		Vector2f v1, v2;
		v1 = grArr[0].position;
		v2 = grArr[1].position;
		cs.setPoint(0, v1);
		cs.setPoint(1, v2);
		v1.y += 1500;
		v2.y += 1500;
		cs.setPoint(3, v1);
		cs.setPoint(2, v2);
		window->draw(cs);
	}
	window->draw(meterText);
}