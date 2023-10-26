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
	const size_t _length = 10;
	std::vector<DXH::Entity> _entities;
};