#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "math.h"

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

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

	world = new b2World(b2Vec2(GRAVITY_X, GRAVITY_Y));

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXTOMET(x), PIXTOMET(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXTOMET(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	b->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// On space bar press, create a circle on mouse position
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 35, b2_dynamicBody);
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		CreateBox(App->input->GetMouseX(), App->input->GetMouseY(), 30, 20, b2_dynamicBody);
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

		CreatePolygon(point, App->input->GetMouseX(), App->input->GetMouseY(), 24, b2_dynamicBody);
		
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		int rick_head[72] = {
			112, 36,
			95, 40,
			93, 19,
			88, 5,
			83, 18,
			75, 31,
			41, 0,
			42, 38,
			13, 36,
			29, 62,
			0, 76,
			30, 91,
			10, 103,
			31, 114,
			25, 125,
			40, 126,
			35, 137,
			46, 133,
			53, 141,
			64, 147,
			76, 149,
			86, 147,
			94, 140,
			99, 127,
			106, 125,
			105, 121,
			100, 116,
			102, 105,
			110, 100,
			105, 93,
			109, 88,
			110, 80,
			109, 73,
			117, 66,
			107, 63,
			105, 58
		};

		CreatePolygon(rick_head, App->input->GetMouseX(), App->input->GetMouseY(), 72, b2_dynamicBody);
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug)
		return UPDATE_CONTINUE;

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

void ModulePhysics::CreateBox(int x_pos, int y_pos, int x_size, int y_size, b2BodyType bodyType)
{
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXTOMET(x_pos), PIXTOMET(y_pos));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	shape.SetAsBox(PIXTOMET(x_size), PIXTOMET(y_size));
	b2FixtureDef fixture;
	fixture.shape = &shape;

	// TODO 2: To have the box behave normally, set fixture's density to 1.0f
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);
}

void ModulePhysics::CreatePolygon(int* points, int x, int y, int pointSize, b2BodyType bodyType)
{
	int vecSize = pointSize / 2;
	b2Vec2* vec = new b2Vec2[vecSize];
	for (int i = 0; i < vecSize; i++)
	{
		vec[i].Set(PIXTOMET(points[2 * i]), PIXTOMET(points[2 * i + 1]));
	}

	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXTOMET(x), PIXTOMET(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	shape.CreateLoop(vec, vecSize);
	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);
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

/*
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
*/

/*
if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
{
CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 35, b2_dynamicBody);
}
*/