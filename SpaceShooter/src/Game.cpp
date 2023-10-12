#include <DXHInclude.h>

class Game : public DXH::DXHEngine 
{
public:
	Game() {}
	~Game() {}
};

DXH::DXHEngine* DXH::CreateDXHEngine() 
{
	return new Game();
}