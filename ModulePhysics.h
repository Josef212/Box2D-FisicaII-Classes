#pragma once
#include "Module.h"
#include "Globals.h"

class b2World;
enum b2BodyType;

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

private:
	b2World* world;

	bool debug;

public:
	void CreateCircle(int x, int y, int radius, b2BodyType bodyType);
	void CreatePolygon();
	//void CreateEdge(int x_start, int y_start, int x_end, int y_end, b2BodyType bodyType);
	void CreateChain();
};