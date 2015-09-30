#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Box2D\Box2D\Box2D.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 35, b2_dynamicBody);
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		App->physics->CreateBox(App->input->GetMouseX(), App->input->GetMouseY(), 30, 20, b2_dynamicBody);
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
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

		App->physics->CreateChain(point, App->input->GetMouseX(), App->input->GetMouseY(), 24, b2_dynamicBody);

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

		App->physics->CreateChain(rick_head, App->input->GetMouseX(), App->input->GetMouseY(), 72, b2_dynamicBody);
	}

	return UPDATE_CONTINUE;
}
