#include "Attack.h"
#include "../Entity.h"

Attack::Attack(GameScene& gs):gs_(&gs)
{
}

bool Attack::IsOwnerActive() const
{
    return self_->IsActive();
}
