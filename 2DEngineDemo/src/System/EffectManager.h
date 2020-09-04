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
	void EnergyBallEffect(const unsigned int& playTime, const float& posX, const float& posY, const float& scale = 1.0f);
	void BloodExplosionEffect(const float& posX, const float& posY, const float& scale = 1.0f);
	void EliminateEnergyBulletEffect(const float& posX, const float& posY, const float& scale = 1.0f);
	void Render();
};

