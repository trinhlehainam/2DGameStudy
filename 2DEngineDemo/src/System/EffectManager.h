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
	void EmitBloodEffect(const float& posX, const float& posY, bool flipFlag, const float& scale = 1.0f);
	void BombExplosionEffect(const float& posX, const float& posY, const float& scale = 1.0f);
	void Render();
};

