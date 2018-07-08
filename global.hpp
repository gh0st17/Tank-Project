struct Messages{ // A struct for global actions
	bool fbox = true, fplatform = true, 
		fground = true, gravity = false,
		fgravity = true;
	double timep, timeb, timeg, timegr, timeGravity;
};

Messages msg;

const float SCALE = 30.f;
const float DEG = (180.0f / b2_pi);
#define PIXELS_PER_METER SCALE
#define UNRATIO (1.0F/RATIO)
#define METERS_PER_PIXEL UNRATIO
#define ZOOM_FACTOR 2.0f

#include "SpriteManager.hpp"
#include "DebugDrawer.hpp"
#include "MyContact.hpp"