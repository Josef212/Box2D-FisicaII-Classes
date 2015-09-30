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

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 35, b2_dynamicBody);
	}
	// TODO 1: When pressing 2, create a box on the mouse position
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		b2BodyDef body;
		body.type = b2_dynamicBody;
		float radius = PIXTOMET(25);
		body.position.Set(METTOPIX(App->input->GetMouseX()), METTOPIX(App->input->GetMouseY()));
		
		b2Body* b = world->CreateBody(&body);

		b2PolygonShape shape;
		shape.SetAsBox(METTOPIX(20), METTOPIX(10));
		b2FixtureDef fixture;
		fixture.shape = &shape;

		// TODO 2: To have the box behave normally, set fixture's density to 1.0f
		fixture.density = 1.0f;

		b->CreateFixture(&fixture);
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// TODO 3: Create a chain shape using those vertices
		// remember to convert them from pixels to meters!

		int point[24] =
		{
			-38, 80,
			-44, -54,
			-16, -60,
			-16, -17,
			19, -19,
			19, -79,
			61, -77,
			57, 73,
			17, 78,
			20, 16,
			-25, 13,
			-9, 72
		};

		b2Vec2 vec[12];
		for (int i = 0; i < 12; i++)
		{
			vec[i].Set(METTOPIX(point[2 * i]), METTOPIX(point[2 * i + 1]));
		}

		b2BodyDef body;
		body.type = b2_dynamicBody;
		float radius = METTOPIX(25);
		body.position.Set(METTOPIX(App->input->GetMouseX()), METTOPIX(App->input->GetMouseY()));

		b2Body* b = world->CreateBody(&body);

		b2ChainShape shape;
		shape.CreateLoop(vec, 12);
		b2FixtureDef fixture;
		fixture.shape = &shape;

		b->CreateFixture(&fixture);

	}
	
	
	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
									  b2CircleShape* shape = (b2CircleShape*)f->GetShape();
									  b2Vec2 pos = f->GetBody()->GetPosition();
									  App->renderer->DrawCircle(METTOPIX(pos.x), METTOPIX(pos.y), METTOPIX(shape->m_radius), 255, 255, 255);
			}
				break;

				// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
									   b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
									   int32 count = polygonShape->GetVertexCount();
									   b2Vec2 prev, v;

									   for (int32 i = 0; i < count; ++i)
									   {
										   v = b->GetWorldPoint(polygonShape->GetVertex(i));
										   if (i > 0)
											   App->renderer->DrawLine(METTOPIX(prev.x), METTOPIX(prev.y), METTOPIX(v.x), METTOPIX(v.y), 255, 100, 100);

										   prev = v;
									   }

									   v = b->GetWorldPoint(polygonShape->GetVertex(0));
									   App->renderer->DrawLine(METTOPIX(prev.x), METTOPIX(prev.y), METTOPIX(v.x), METTOPIX(v.y), 255, 100, 100);
			}
				break;

				// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
									 b2ChainShape* shape = (b2ChainShape*)f->GetShape();
									 b2Vec2 prev, v;

									 for (int32 i = 0; i < shape->m_count; ++i)
									 {
										 v = b->GetWorldPoint(shape->m_vertices[i]);
										 if (i > 0)
											 App->renderer->DrawLine(METTOPIX(prev.x), METTOPIX(prev.y), METTOPIX(v.x), METTOPIX(v.y), 100, 255, 100);
										 prev = v;
									 }

									 v = b->GetWorldPoint(shape->m_vertices[0]);
									 App->renderer->DrawLine(METTOPIX(prev.x), METTOPIX(prev.y), METTOPIX(v.x), METTOPIX(v.y), 100, 255, 100);
			}
				break;

				// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
									b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
									b2Vec2 v1, v2;

									v1 = b->GetWorldPoint(shape->m_vertex0);
									v1 = b->GetWorldPoint(shape->m_vertex1);
									App->renderer->DrawLine(METTOPIX(v1.x), METTOPIX(v1.y), METTOPIX(v2.x), METTOPIX(v2.y), 100, 100, 255);
			}
				break;
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
void CreatePolygon()
{

}
/*void CreateEdge(int x_start, int y_start, int x_end, int y_end, b2BodyType bodyType)
{
	b2BodyDef bodydef;
	bodydef.type = bodyType;
	bodydef.position.Set(PIXTOMET(x_start), PIXTOMET(y_start));
	b2Body* body = world->CreateBody(&bodydef);

	b2Vec2 v1(x_start, y_start);
	b2Vec2 v2(x_end, y_end);

	b2EdgeShape edge;
	edge.Set(v1, v2);

	b2FixtureDef fixture;
	fixture.shape = &edge;
	body->CreateFixture(&fixture);
}*/
void CreateChain()
{
	
}