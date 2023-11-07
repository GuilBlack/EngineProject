#include "Render.h"
#include "src/renderer/Geometry.h"

namespace DXH
{

void NumberUI::OnDetach()
{
    if (Geo)
        DELETE_PTR(Geo);
}

void NumberUI::InitGeometry(uint32_t numCharacters)
{
    NumCharacters = numCharacters;
    Geo = new NumberGeometry(numCharacters);
}
}