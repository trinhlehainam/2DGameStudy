#pragma once

class GameScene;

// Manage effects of game
class EffectManager
{
private:
	GameScene& gs_;
public:
	EffectManager(GameScene& gs);
	~EffectManager() = default;
	void Update(const float& deltaTime);
	void EmitBloodEffect(const float& posX, const float& posY, bool flipFlag);
	void BombExplosionEffect(const float& posX, const float& posY);
	void Render();
};

