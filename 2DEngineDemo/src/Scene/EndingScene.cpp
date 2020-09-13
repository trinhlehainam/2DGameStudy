#include <DxLib.h>
#include "../Constant.h"
#include "EndingScene.h"
#include "TitleScene.h"
#include "../System/AssetManager.h"
#include "../System/EntityManager.h"
#include "../System/CombatManager.h"
#include "../System/SceneManager.h"
namespace
{
	constexpr int blink_interval_fast = 5;
	constexpr int blink_interval_normal = 45;

	constexpr float wait_blink_time = 0.3f;
	constexpr float wait_fade_time = 0.6f;
	float waitTimer_ = 0.0f;

	unsigned int blinkTimer_ = 0;
	int blinkInterval = blink_interval_normal;

	int currentItemNo_ = 0;
	int itemSize = 0;
	int frame_ = 0;
}



void EndingScene::StartInput()
{
}

void EndingScene::SleepInput()
{
}

void EndingScene::Update(const float& deltaTime)
{
	(this->*updateFunc_)(deltaTime);
}

void EndingScene::FadeInUpdate(const float& deltaTime)
{
	if (waitTimer_ <= 0.0f)
	{
		sceneMng_.ChangeScene(std::move(active_scene(new TitleScene(sceneMng_, sceneInput_))));
	}
	waitTimer_ -= deltaTime;
}

void EndingScene::FadeOutUpdate(const float& deltaTime)
{
}

void EndingScene::EndingUpdate(const float& deltaTime)
{
}

void EndingScene::Render()
{
	(this->*renderFunc_)();
}

void EndingScene::NormalRender()
{
}

void EndingScene::FadeRender()
{
	NormalRender();
	auto blendpara = 255 * (wait_fade_time - waitTimer_) / wait_fade_time;
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendpara);
	DxLib::DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DxLib::DrawFormatString(0, 0, 0xffffff, L"EndingScene");
}

void EndingScene::Initialize()
{
	updateFunc_ = &EndingScene::FadeInUpdate;
	renderFunc_ = &EndingScene::FadeRender;
	inputFunc_ = &EndingScene::SleepInput;
}

void EndingScene::ProcessInput()
{
}

EndingScene::EndingScene(SceneManager& sceneMng, KeyboardInput& sceneInput):BaseScene(sceneMng, sceneInput)
{
	Initialize();

}

EndingScene::~EndingScene()
{
}
