#pragma once
#include <DXHInclude.h>

class Game : public DXH::DXHEngine
{
public:
	Game(DXH::AppProperties props);
	~Game();

	void Update(const DXH::Timer& gt) override;
};