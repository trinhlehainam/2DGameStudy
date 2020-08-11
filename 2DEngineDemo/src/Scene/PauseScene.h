#pragma once
#include "BaseScene.h"

class PauseScene:
	public BaseScene
{
	friend SceneManager;
public:
	PauseScene(SceneManager&, KeyboardInput&);
	~PauseScene();

	void ProcessInput() override;
	void Update(const float& deltaTime) override;
	void Render() override;
};

