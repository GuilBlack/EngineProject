#include "Score.h"

void Score::Update(const DXH::Timer& gt)
{
    m_DeltaTime += gt.DeltaTime();
    if (m_DeltaTime >= 1.f)
    {
        m_DeltaTime -= 1.f;
        m_Score++;
    }
    if (pGameObject->Has<DXH::NumberUI>())
    {
        DXH::NumberUI& num = pGameObject->Get<DXH::NumberUI>();
        num.Number = std::to_string(m_Score);
        for (uint64_t i = num.Number.size(); i < num.NumCharacters; i++)
        {
            num.Number = "0" + num.Number;
        }
    }
}
