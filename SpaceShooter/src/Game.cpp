#include <DXHInclude.h>

class Game : public DXH::DXHEngine 
{
public:
	Game(DXH::AppProperties props)
		: DXH::DXHEngine(props)
	{}
	~Game() {}
};

DXH::DXHEngine* DXH::CreateDXHEngine() 
{
	DXH::AppProperties props
	{
		.WindowTitle = L"Space Shooter"
	};
	return new Game(props);
}