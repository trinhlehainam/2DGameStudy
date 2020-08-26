#include "GameScene.h"

#include <DxLib.h>

#include "../Constant.h"

#include "PauseScene.h"

#include "../System/AssetManager.h"
#include "../System/EnemyManager.h"
#include "../System/EntityManager.h"
#include "../System/SceneManager.h"
#include "../System/CollisionManager.h"
#include "../System/Map.h"
#include "../System/Camera.h"
#include "../System/EffectManager.h"

#include "../Game Object/Entity.h"
#include "../Game Object/Player/Player.h"
#include "../Game Object/Enemy/Slasher.h"
#include "../Game Object/Enemy/SideSpawner.h"
#include "../Game Object/Environment.h"
#include "../Game Object/Enemy/BossSpawner.h"
#include "../Game Object/Enemy/Asura.h"

#include "../Component/TransformComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/AABBColliderComponent.h"
#include "../Component/CircleColliderComponent.h"
#include "../Component/RigidBody2D.h"

#include "../Input/KeyboardInput.h"

namespace
{
	float waitTimer_ = 0.0f;
	constexpr float wait_fade_time = 0.6f;
	const Vector2 slasher_start_pos = Vector2(100.0f, 100.0f);
	constexpr float gravity_force_y = 1000.f;
	constexpr float side_spawn_offset_x = 100.0f;
	constexpr float side_spawn_offset_y = WINDOW_HEIGHT/2.0f;

	Rect windowBox_;

}

int GameScene::GetTexture(std::string textureID)
{
	return assetMng_->GetTexture(textureID);
}

GameScene::GameScene(SceneManager& sceneMng, KeyboardInput& sceneInput):BaseScene(sceneMng, sceneInput)
{
	waitTimer_ = wait_fade_time;
	windowBox_ = Rect(Vector2(0.0f, 0.0f), WINDOW_WIDTH, WINDOW_HEIGHT);
	updateFunc_ = &GameScene::FadeInUpdate;
	renderFunc_ = &GameScene::FadeInRender;

	assetMng_ = std::make_unique<AssetManager>();
	entityMng_ = std::make_unique<EntityManager>();
	collisionMng_ = std::make_unique<CollisionManager>(*this);
	effectMng_ = std::make_unique<EffectManager>(*this);
	
	LoadLevel(1);

	environment_ = std::make_unique<Environment>(*this);
}

void GameScene::LoadLevel(const int& level)
{
	// Load assets
	assetMng_->AddTexture("bg", L"assets/Title.png");
	assetMng_->AddTexture("map", L"assets/Image/Tilemap/Assets.png");

	assetMng_->AddTexture("player-run", L"assets/Image/Character/Player/adventurer-run-sheet.png");
	assetMng_->AddTexture("player-idle", L"assets/Image/Character/Player/adventurer-idle-00-sheet.png");
	assetMng_->AddTexture("player-jump", L"assets/Image/Character/Player/adventurer-roll-sheet.png");
	assetMng_->AddTexture("player-fall", L"assets/Image/Character/Player/adventurer-fall-sheet.png");

	assetMng_->AddTexture("bomb-equip", L"assets/Image/Character/Player/Equipment/bombshot.png");
	assetMng_->AddTexture("shuriken-equip", L"assets/Image/Character/Player/Equipment/shuriken.png");
	assetMng_->AddTexture("chain-equip", L"assets/Image/Character/Player/Equipment/chainsickle.png");
	assetMng_->AddTexture("shuriken-icon", L"assets/Image/UI/shuriken.png");
	assetMng_->AddTexture("chain-icon", L"assets/Image/UI/chain.png");
	assetMng_->AddTexture("bomb-icon", L"assets/Image/UI/bomb.png");

	assetMng_->AddTexture("slasher-run", L"assets/Image/Character/Enemy/slasher/slasher-run-Sheet.png");
	assetMng_->AddTexture("slasher-slash", L"assets/Image/Character/Enemy/slasher/slasher-slash-Sheet.png");
	assetMng_->AddTexture("boss-asura", L"assets/Image/Character/Enemy/asura/ashura.png");
	
	assetMng_->AddTexture("environment-1", L"assets/Image/Environment/environment_1.png");
	assetMng_->AddTexture("environment-2", L"assets/Image/Environment/environment_2.png");

	assetMng_->AddTexture("blood", L"assets/Image/Effect/blood.png");
	assetMng_->AddTexture("bomb-explosion", L"assets/Image/Effect/bomb_exp.png");
	
	// Create Title Map
	map_ = std::make_unique<Map>(*entityMng_,*collisionMng_,16,2);
	map_->LoadMapLayer("background",assetMng_->GetTexture("map"),"assets/Image/Tilemap/background.map",
		MAP_SIZE_X, MAP_SIZE_Y, MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y);
	map_->LoadMapLayer("foreground", assetMng_->GetTexture("map"), "assets/Image/Tilemap/foreground.map",
		MAP_SIZE_X, MAP_SIZE_Y, MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y);

	// Load Collision Map
	map_->LoadCollisionLayer("GROUND", "assets/Image/Tilemap/ground.map", MAP_SIZE_X, MAP_SIZE_Y);
	map_->LoadCollisionLayer("BRICK", "assets/Image/Tilemap/brick.map", MAP_SIZE_X, MAP_SIZE_Y);
	map_->LoadCollisionLayer("ASURA", "assets/Image/Tilemap/boss-position.map", MAP_SIZE_X, MAP_SIZE_Y);

	// Initialize Camera ( Track Camera to Player )
	Camera::Instance().viewport.w = WINDOW_WIDTH;
	Camera::Instance().viewport.h = WINDOW_HEIGHT;

	// Create player Entity
	player_ = std::make_unique<Player>(*this);
	player_->Initialize();

	// Create Enemy Slasher Entity
	enemyMng_ = std::make_unique<EnemyManager>(player_->GetPlayerTransform(), *this);

	// Create spawn enemy
	auto slasherClone = std::make_unique<Slasher>(*this, player_->GetPlayerTransform());
	auto sideSpawner = std::make_unique<SideSpawner>(std::move(slasherClone), slasher_start_pos, *enemyMng_);
	sideSpawner->SetOffSet(side_spawn_offset_x, side_spawn_offset_y);
	spawners_.emplace_back(std::move(sideSpawner));
	
	collisionMng_->SetGravity(Vector2(0, gravity_force_y));
}

void GameScene::ProcessInput()
{
	if(sceneInput_.IsTriggered(L"pause"))
	{
		sceneMng_.PushScene(std::move(active_scene(new PauseScene(sceneMng_, sceneInput_, (*player_->input_)))));
	}
}

void GameScene::Update(const float& deltaTime)
{
	(this->*updateFunc_)(deltaTime);
}

void GameScene::FadeInUpdate(const float& deltaTime)
{
	Camera::Instance().Update();
	entityMng_->Update(deltaTime);
	if (waitTimer_ <= 0)
	{
		updateFunc_ = &GameScene::GameUpdate;
		renderFunc_ = &GameScene::GameRender;
	}
	waitTimer_ -= deltaTime;
}

void GameScene::GameUpdate(const float& deltaTime)
{
	player_->Input(deltaTime);
	for (auto& spawner : spawners_)
	{
		spawner->Update(deltaTime);
	}
	collisionMng_->ApplyForce(deltaTime);
	enemyMng_->Update(deltaTime);
	entityMng_->Update(deltaTime);
	Camera::Instance().Update();
	environment_->Update(deltaTime);
	ProcessEnterBossArea();
	collisionMng_->PlatformResolution(deltaTime);
	collisionMng_->Update(deltaTime);
	collisionMng_->ProjectileCollision();
	effectMng_->Update(deltaTime);
	player_->UpdateState();
}

void GameScene::ProcessEnterBossArea()
{
	Vector2 bossPos;
	if (collisionMng_->IsEnterBossArea("ASURA", bossPos) && !isBossAdded)
	{
		auto asuraClone = std::make_unique<Asura>(*this, player_->GetPlayerTransform());
		auto bossSpawner = std::make_unique<BossSpawner>(std::move(asuraClone), bossPos, *enemyMng_);
		spawners_.emplace_back(std::move(bossSpawner));
		Camera::Instance().LockCamera();
		isBossAdded = true;
	}
}

void GameScene::Render()
{
	(this->*renderFunc_)();
}

void GameScene::FadeInRender()
{
	environment_->RenderBackGround();
	entityMng_->Render();
	auto blendpara = 255 * waitTimer_ / wait_fade_time;
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendpara);
	DxLib::DrawBox(windowBox_.Left(), windowBox_.Top(),
		windowBox_.Right(), windowBox_.Bottom(),
		0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::GameRender()
{
	environment_->RenderBackGround();
	entityMng_->Render();
	collisionMng_->Render();
	effectMng_->Render();
	player_->RenderUI();
}

GameScene::~GameScene()
{
}
