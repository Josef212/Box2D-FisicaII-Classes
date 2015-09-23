#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

// TODONE 1: Include Box 2 header and library
#include "Box2D/Box2D/Box2D.h"
#pragma comment(lib, "Box2D/libx86/Debug/Box2D.lib")

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODONE 2: Create a private variable for the world
	// - You need to send it a default gravity
	b2Vec2 gravity(GRAVITY_X, GRAVITY_Y);
	// - You need init the world in the constructor
	world = new b2World(gravity);
	// - Remember to destroy the world after using it

	// TODONE 4: Create a a big static circle as "ground"

	CreateCircle(525, 400, 225, b2_staticBody);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODONE 3: Update the simulation ("step" the world)
	world->Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATION);
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODONE 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 35, b2_dynamicBody);
	}
	
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METTOPIX(pos.x), METTOPIX(pos.y), METTOPIX(shape->m_radius), 255, 255, 255);
				}
				break;

				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType bodyType)
{
	b2BodyDef bodydef;
	bodydef.type = bodyType;
	bodydef.position.Set(PIXTOMET(x), PIXTOMET(y));
	b2Body* body = world->CreateBody(&bodydef);

	b2CircleShape cShape;
	cShape.m_radius = PIXTOMET(radius);

	b2FixtureDef fixture;
	fixture.shape = &cShape;
	body->CreateFixture(&fixture);
}
void CreateCPolygon()
{

}
void CreateEdge()
{

}
void CreateChain()
{

}