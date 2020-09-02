#include "Attack.h"
#include "../Entity.h"

Attack::Attack(GameScene& gs, const std::shared_ptr<Entity>& owner):gs_(&gs),owner_(owner)
{
}

bool Attack::IsOwnerActive() const
{
    return self_->IsActive();
}
