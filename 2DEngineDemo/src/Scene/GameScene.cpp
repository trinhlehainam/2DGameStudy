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
#include "../System/CombatManager.h"

#include "../GameObject/Entity.h"
#include "../GameObject/Player/Player.h"
#include "../GameObject/Enemy/Slasher.h"
#include "../GameObject/Enemy/SideSpawner.h"
#include "../GameObject/Environment.h"
#include "../GameObject/Enemy/BossSpawner.h"
#include "../GameObject/Enemy/Asura.h"

#include "../Component/TransformComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/Collider/AABBColliderComponent.h"
#include "../Component/Collider/CircleColliderComponent.h"
#include "../Component/Collider/RigidBody2D.h"

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
	combatMng_ = std::make_unique<CombatManager>();
	
	LoadLevel(1);

	environment_ = std::make_unique<Environment>(*this);
}

void GameScene::LoadLevel(const int& level)
{
	// Load assets
	assetMng_->AddTexture("bg", L"assets/Title.png");
	assetMng_->AddTexture("map", L"assets/Image/Tilemap/Assets.png");

	assetMng_->AddTexture("player-run", L"assets/Image/Character/Player/adventurer-run-sheet.png");
	assetMng_->AddTexture("player-sword-run", L"assets/Image/Character/Player/adventurer-swd-run-sheet.png");
	assetMng_->AddTexture("player-fast-run", L"assets/Image/Character/Player/adventurer-fast-run-sheet.png");
	assetMng_->AddTexture("player-idle", L"assets/Image/Character/Player/adventurer-idle-sheet.png");
	assetMng_->AddTexture("player-sword-idle", L"assets/Image/Character/Player/adventurer-swd-idle-sheet.png");
	assetMng_->AddTexture("player-jump", L"assets/Image/Character/Player/adventurer-roll-sheet.png");
	assetMng_->AddTexture("player-fall", L"assets/Image/Character/Player/adventurer-fall-sheet.png");
	assetMng_->AddTexture("player-hurt", L"assets/Image/Character/Player/adventurer-hurt-sheet.png");
	assetMng_->AddTexture("player-cast", L"assets/Image/Character/Player/adventurer-cast-sheet.png");
	assetMng_->AddTexture("player-crouch", L"assets/Image/Character/Player/adventurer-crouch-sheet.png");
	assetMng_->AddTexture("player-crouch-walk", L"assets/Image/Character/Player/adventurer-crouch-walk-sheet.png");
	assetMng_->AddTexture("player-die", L"assets/Image/Character/Player/adventurer-die-sheet.png");
	assetMng_->AddTexture("player-draw-sword", L"assets/Image/Character/Player/adventurer-swd-drw-sheet.png");
	assetMng_->AddTexture("player-withdraw-sword", L"assets/Image/Character/Player/adventurer-swd-wd-sheet.png");
	assetMng_->AddTexture("player-attack1", L"assets/Image/Character/Player/adventurer-attack1-sheet.png");
	assetMng_->AddTexture("player-attack2", L"assets/Image/Character/Player/adventurer-attack2-sheet.png");
	assetMng_->AddTexture("player-attack3", L"assets/Image/Character/Player/adventurer-attack3-sheet.png");

	assetMng_->AddTexture("bomb-equip", L"assets/Image/Character/Player/Equipment/bombshot.png");
	assetMng_->AddTexture("shuriken-equip", L"assets/Image/Character/Player/Equipment/shuriken.png");
	assetMng_->AddTexture("chain-equip", L"assets/Image/Character/Player/Equipment/chainsickle.png");
	assetMng_->AddTexture("shuriken-icon", L"assets/Image/UI/shuriken.png");
	assetMng_->AddTexture("chain-icon", L"assets/Image/UI/chain.png");
	assetMng_->AddTexture("bomb-icon", L"assets/Image/UI/bomb.png");
	assetMng_->AddTexture("sword-icon", L"assets/Image/UI/sword.png");
	assetMng_->AddTexture("energy-bullet", L"assets/Image/Character/Enemy/asura/fireball_b.png");

	assetMng_->AddTexture("slasher-run", L"assets/Image/Character/Enemy/slasher/slasher-run-Sheet.png");
	assetMng_->AddTexture("slasher-slash", L"assets/Image/Character/Enemy/slasher/slasher-slash-Sheet.png");
	assetMng_->AddTexture("slasher-hurt", L"assets/Image/Character/Enemy/slasher/slasher-hurt-Sheet.png");
	assetMng_->AddTexture("slasher-death", L"assets/Image/Character/Enemy/slasher/slasher-death-Sheet.png");
	assetMng_->AddTexture("slasher-lying", L"assets/Image/Character/Enemy/slasher/slasher-lying-Sheet.png");

	assetMng_->AddTexture("boss-asura", L"assets/Image/Character/Enemy/asura/ashura.png");
	
	assetMng_->AddTexture("environment-1", L"assets/Image/Environment/environment_1.png");
	assetMng_->AddTexture("environment-2", L"assets/Image/Environment/environment_2.png");

	assetMng_->AddTexture("blood", L"assets/Image/Effect/blood.png");
	assetMng_->AddTexture("bomb-explosion", L"assets/Image/Effect/bomb_exp.png");
	assetMng_->AddTexture("energy-ball", L"assets/Image/Effect/chargeball.png");
	assetMng_->AddTexture("blood-explosion", L"assets/Image/Effect/blood_exp.png");
	assetMng_->AddTexture("eliminate-energy-bullet", L"assets/Image/Effect/eliminate_b.png");
	
	// Create Title Map
	map_ = std::make_unique<Map>(*entityMng_,*collisionMng_,16,2);
	map_->LoadMapLayer("BACKGROUND",assetMng_->GetTexture("map"),"assets/Image/Tilemap/background.map",
		MAP_SIZE_X, MAP_SIZE_Y, MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y);
	map_->LoadMapLayer("FOREGROUND", assetMng_->GetTexture("map"), "assets/Image/Tilemap/foreground.map",
		MAP_SIZE_X, MAP_SIZE_Y, MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y);

	// Load Collision Map
	map_->LoadCollisionLayer("TERRAIN","GROUND", "assets/Image/Tilemap/ground.map", MAP_SIZE_X, MAP_SIZE_Y);
	map_->LoadCollisionLayer("TERRAIN","BRICK", "assets/Image/Tilemap/brick.map", MAP_SIZE_X, MAP_SIZE_Y);
	map_->LoadCollisionLayer("STAGE", "ASURA", "assets/Image/Tilemap/boss-position.map", MAP_SIZE_X, MAP_SIZE_Y);

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

	// Initialize Camera ( Track Camera to Player )
	Camera::Instance().SetViewSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Camera::Instance().SetOffset(Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3));
	Camera::Instance().SetLimit(Vector2(WORLD_MAP_X - WINDOW_WIDTH, 0));
	auto playerTransform = player_->self_->GetComponent<TransformComponent>();
	Camera::Instance().SetTargetEntity(playerTransform);
	Camera::Instance().SetPosition(playerTransform->pos);
	
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
	/*----------------------------------------------------------------------------*/
	// Update player's STATE right after received player's INPUT
	// Purpose : update player's state before updating player's animation to AVOID BLINKING animation ( late animation )
	player_->UpdateState();
	/*----------------------------------------------------------------------------*/
	enemyMng_->Update(deltaTime);
	entityMng_->Update(deltaTime);
	combatMng_->Update(deltaTime);
	environment_->Update(deltaTime);
	ProcessEnterBossArea();
	collisionMng_->PlatformResolution(deltaTime);
	collisionMng_->Update(deltaTime);
	collisionMng_->CombatCollision();
	effectMng_->Update(deltaTime);
	Camera::Instance().Update();
}

void GameScene::BossSceneUpdate(const float& deltaTime)
{
	(*spawners_.rbegin())->Update(deltaTime);
	enemyMng_->BossSceneUpdate(deltaTime);
	entityMng_->BossSceneUpdate(deltaTime);
	Camera::Instance().Update();
	if (timer_ <= 0)
	{
		updateFunc_ = &GameScene::GameUpdate;
	}
	timer_ -= deltaTime;
}

void GameScene::ProcessEnterBossArea()
{
	Vector2 bossPos;
	if (collisionMng_->IsEnterBossArea("ASURA", bossPos) && !isBossAdded)
	{
		auto asuraClone = std::make_unique<Asura>(*this, player_->GetPlayerTransform());
		auto bossSpawner = std::make_unique<BossSpawner>(std::move(asuraClone), bossPos, *enemyMng_);
		auto playerTransform = player_->self_->GetComponent<TransformComponent>();
		playerTransform->SetLeftLimit(bossPos.X - WINDOW_WIDTH / 2);
		playerTransform->SetRightLimit(bossPos.X + WINDOW_WIDTH / 2 - playerTransform->w);
		spawners_.emplace_back(std::move(bossSpawner));
		Camera::Instance().LockCameraAt(bossPos);
		isBossAdded = true;
		timer_ = 4.0f;
		updateFunc_ = &GameScene::BossSceneUpdate;
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
	player_->RenderUI();
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
	/*collisionMng_->Render();*/	// collision debug
	effectMng_->Render();
	player_->RenderUI();
}

GameScene::~GameScene()
{
}
