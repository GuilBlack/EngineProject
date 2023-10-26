#pragma once
#include <DXHInclude.h>
#include "src/ecs/Entity.h" // TODO: Remove this later

class Game : public DXH::DXHEngine
{
public:
	Game(DXH::AppProperties props);
	~Game();

	void Update(const DXH::Timer& gt) override;

private:
	DXH::Entity testEntity;
};