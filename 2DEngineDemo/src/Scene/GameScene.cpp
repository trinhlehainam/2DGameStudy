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
#include "../System/LevelManager.h"

#include "../GameObject/Entity.h"
#include "../GameObject/Player/Player.h"
#include "../GameObject/Enemy/Slasher.h"
#include "../GameObject/Enemy/SideSpawner.h"
#include "../GameObject/Environment.h"
#include "../GameObject/Enemy/BossSpawner.h"
#include "../GameObject/Enemy/Asura.h"
#include "../GameObject/Enemy/FlyingEye.h"
#include "../GameObject/Enemy/Mushroom.h"
#include "../GameObject/Enemy/Skeleton.h"
#include "../GameObject/BornFire.h"

#include "../Component/TransformComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/Collider/AABBColliderComponent.h"
#include "../Component/Collider/CircleColliderComponent.h"
#include "../Component/Collider/RigidBody2D.h"

#include "../Input/KeyboardInput.h"

namespace
{
	float waitTimer_ = 0.0f;
	constexpr float wait_fade_in_time = 0.6f;
	constexpr float wait_fade_out_time = 0.6f;
	constexpr float wait_respawn_player_time = 0.3f;
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
	waitTimer_ = wait_fade_in_time;
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
	assetMng_->AddTexture("map", L"assets/Image/Tilemap/tileset.png");

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
	assetMng_->AddTexture("player-die", L"assets/Image/Character/Player/adventurer-death-sheet.png");
	assetMng_->AddTexture("player-draw-sword", L"assets/Image/Character/Player/adventurer-swd-drw-sheet.png");
	assetMng_->AddTexture("player-withdraw-sword", L"assets/Image/Character/Player/adventurer-swd-wd-sheet.png");
	assetMng_->AddTexture("player-attack1", L"assets/Image/Character/Player/adventurer-attack1-sheet.png");
	assetMng_->AddTexture("player-attack2", L"assets/Image/Character/Player/adventurer-attack2-sheet.png");
	assetMng_->AddTexture("player-attack3", L"assets/Image/Character/Player/adventurer-attack3-sheet.png");
	assetMng_->AddTexture("player-slide-wall", L"assets/Image/Character/Player/adventurer-wall-slide-sheet.png");
	assetMng_->AddTexture("player-air-attack", L"assets/Image/Character/Player/adventurer-air-attack-sheet.png");
	assetMng_->AddTexture("player-air-charge", L"assets/Image/Character/Player/adventurer-air-attack2-sheet.png");
	assetMng_->AddTexture("player-slash-down", L"assets/Image/Character/Player/adventurer-air-attack3-loop-sheet.png");
	assetMng_->AddTexture("player-end-slash-down", L"assets/Image/Character/Player/adventurer-air-attack-3-end-sheet.png");

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

	assetMng_->AddTexture("flying-eye-attack", L"assets/Image/Character/Enemy/Flying Eye/flying-eye-attack-sheet.png");
	assetMng_->AddTexture("flying-eye-flight", L"assets/Image/Character/Enemy/Flying Eye/flying-eye-flight-sheet.png");
	assetMng_->AddTexture("flying-eye-hurt", L"assets/Image/Character/Enemy/Flying Eye/flying-eye-hurt-sheet.png");
	assetMng_->AddTexture("flying-eye-death", L"assets/Image/Character/Enemy/Flying Eye/fyling-eye-death-sheet.png");

	assetMng_->AddTexture("mushroom-Attack", L"assets/Image/Character/Enemy/Mushroom/mushroom-Attack-sheet.png");
	assetMng_->AddTexture("mushroom-Death", L"assets/Image/Character/Enemy/Mushroom/mushroom-Death-sheet.png");
	assetMng_->AddTexture("mushroom-Hurt", L"assets/Image/Character/Enemy/Mushroom/mushroom-Hurt-sheet.png");
	assetMng_->AddTexture("mushroom-Idle", L"assets/Image/Character/Enemy/Mushroom/mushroom-Idle-sheet.png");
	assetMng_->AddTexture("mushroom-Run", L"assets/Image/Character/Enemy/Mushroom/mushroom-Run-sheet.png");

	assetMng_->AddTexture("skeleton-Attack", L"assets/Image/Character/Enemy/Skeleton/skeleton-Attack-sheet.png");
	assetMng_->AddTexture("skeleton-Death", L"assets/Image/Character/Enemy/Skeleton/skeleton-Death-sheet.png");
	assetMng_->AddTexture("skeleton-Hurt", L"assets/Image/Character/Enemy/Skeleton/skeleton-Hurt-sheet.png");
	assetMng_->AddTexture("skeleton-Idle", L"assets/Image/Character/Enemy/Skeleton/skeleton-Idle-sheet.png");
	assetMng_->AddTexture("skeleton-Run", L"assets/Image/Character/Enemy/Skeleton/skeleton-Walk-sheet.png");

	assetMng_->AddTexture("boss-asura", L"assets/Image/Character/Enemy/asura/ashura.png");
	
	assetMng_->AddTexture("spider-king-walk", L"assets/Image/Character/Enemy/Skeleton King/SkeletonKing_Walk_Sheet.png");
	assetMng_->AddTexture("spider-king-dead", L"assets/Image/Character/Enemy/Skeleton King/SkeletonKing_Death_Sheet.png");
	assetMng_->AddTexture("spider-king-bullet", L"assets/Image/Character/Enemy/Skeleton King/SkeletonKing_Obstacle_Sheet.png");
	assetMng_->AddTexture("spider-king-bullet-exp", L"assets/Image/Character/Enemy/Skeleton King/SkeletonKing_Ostacle_Death_Sheet.png");

	assetMng_->AddTexture("environment-1", L"assets/Image/Environment/environment_1.png");
	assetMng_->AddTexture("environment-2", L"assets/Image/Environment/environment_2.png");

	assetMng_->AddTexture("blood", L"assets/Image/Effect/blood.png");
	assetMng_->AddTexture("bomb-explosion", L"assets/Image/Effect/bomb_exp.png");
	assetMng_->AddTexture("energy-ball", L"assets/Image/Effect/chargeball.png");
	assetMng_->AddTexture("blood-explosion", L"assets/Image/Effect/blood_exp.png");
	assetMng_->AddTexture("eliminate-energy-bullet", L"assets/Image/Effect/eliminate_b.png");
	
	// character UI
	assetMng_->AddTexture("UI_jump", L"assets/Image/UI/Command/jump.png");
	assetMng_->AddTexture("UI_attack", L"assets/Image/UI/Command/Attack.png");
	assetMng_->AddTexture("UI_change_weapon", L"assets/Image/UI/Command/change_wepon.png");

	assetMng_->AddTexture("UI_MoveLeft", L"assets/Image/UI/Command/run.png");
	//assetMng_->AddTexture("UI_MoveRight", L"assets/Image/UI/Command/run.png");
	// keymap
	assetMng_->AddTexture("Z_jump", L"assets/Image/UI/Key/z_alternative_paper.png");
	assetMng_->AddTexture("X_attack", L"assets/Image/UI/Key/x_alternative_paper.png");
	assetMng_->AddTexture("C_change_weapon", L"assets/Image/UI/Key/c_alternative_paper.png");

	assetMng_->AddTexture("LeftArrow_Move", L"assets/Image/UI/Key/arrowleft_alternative_paper.png");
	assetMng_->AddTexture("RightArrow_Move", L"assets/Image/UI/Key/arrowright_alternative_paper.png");

	assetMng_->AddTexture("born-fire", L"assets/Image/UI/bornfire.png");

	// Create Title Map
	map_ = std::make_unique<Map>(*entityMng_,*collisionMng_,16,2);
	map_->LoadMapLayer("BACKGROUND",assetMng_->GetTexture("map"),"assets/Image/Tilemap/background.map",
		NUM_TILE_X, NUM_TILE_Y);
	map_->LoadMapLayer("PLATFORM", assetMng_->GetTexture("map"), "assets/Image/Tilemap/platform.map",
		NUM_TILE_X, NUM_TILE_Y);
	map_->LoadMapLayer("FOREGROUND", assetMng_->GetTexture("map"), "assets/Image/Tilemap/foreground.map",
		NUM_TILE_X, NUM_TILE_Y);

	// Load Collision Map
	map_->LoadCollisionLayer("TERRAIN","TERRAIN", "assets/Image/Tilemap/terrain.map", NUM_TILE_X, NUM_TILE_Y,
		32,32);
	map_->LoadCollisionLayer("TERRAIN", "TERRAIN", "assets/Image/Tilemap/bamboo-terrain.map", NUM_TILE_X, NUM_TILE_Y,
		bamboo_collider_w, bamboo_collider_h, bamboo_collider_offset_x, bamboo_collider_offset_y);
	map_->LoadCollisionLayer("TERRAIN", "TRAP", "assets/Image/Tilemap/bamboo-h-trap.map", NUM_TILE_X, NUM_TILE_Y,
		bamboo_horizontal_trap_w, bamboo_horizontal_trap_h, bamboo_horizontal_trap_offset_x, bamboo_horizontal_trap_offset_y);
	map_->LoadCollisionLayer("TERRAIN", "TRAP", "assets/Image/Tilemap/bamboo-v-trap.map", NUM_TILE_X, NUM_TILE_Y,
		bamboo_vertical_trap_w, bamboo_vertical_trap_h, bamboo_vertical_trap_offset_x, bamboo_vertical_trap_offset_y);
	map_->LoadCollisionLayer("TERRAIN", "TRAP", "assets/Image/Tilemap/metal-trap.map", NUM_TILE_X, NUM_TILE_Y,
		32, 32);
	map_->LoadCollisionLayer("ENEMY", "FLYING-EYE", "assets/Image/Tilemap/flying-eye.map", NUM_TILE_X, NUM_TILE_Y, 32, 32);
	map_->LoadCollisionLayer("ENEMY", "MUSHROOM", "assets/Image/Tilemap/mushroom.map", NUM_TILE_X, NUM_TILE_Y, 32, 32);
	map_->LoadCollisionLayer("ENEMY", "SKELETON", "assets/Image/Tilemap/skeleton.map", NUM_TILE_X, NUM_TILE_Y, 32, 32);
	map_->LoadCollisionLayer("CHECK-POINT", "BORN-FIRE", "assets/Image/Tilemap/bornfire.map", NUM_TILE_X, NUM_TILE_Y, 32, 32);

	// Create player Entity
	player_ = std::make_unique<Player>(*this);
	player_->Initialize();

	// Create Enemy Slasher Entity
	enemyMng_ = std::make_unique<EnemyManager>(player_->GetPlayerTransform(), *this);

	LoadEnemy();

	// Create spawn enemyTile
	/*auto slasherClone = std::make_unique<Slasher>(*this, player_->GetPlayerTransform());
	auto sideSpawner = std::make_unique<SideSpawner>(std::move(slasherClone), slasher_start_pos, *enemyMng_);
	sideSpawner->SetOffSet(side_spawn_offset_x, side_spawn_offset_y);
	spawners_.emplace_back(std::move(sideSpawner));*/

	// Initialize Camera ( Track Camera to Player )
	Camera::Instance().SetViewSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Camera::Instance().SetOffset(Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 3));
	Camera::Instance().SetLimit(Vector2(WORLD_MAP_X - WINDOW_WIDTH, WORLD_MAP_Y - WINDOW_HEIGHT));
	auto playerTransform = player_->self_->GetComponent<TransformComponent>();
	Camera::Instance().SetTargetEntity(playerTransform);
	Camera::Instance().SetPosition(playerTransform->pos);
	
	collisionMng_->SetGravity(Vector2(0, gravity_force_y));
	levelMng_ = std::make_unique<LevelManager>(*this, (*player_));
	LoadCheckPoint();
}

void GameScene::LoadEnemy()
{
	for (auto& enemyTile : entityMng_->mapLayers_["ENEMY"])
	{
		auto tileTransform = enemyTile->GetComponent<TransformComponent>();
		if (enemyTile->GetName() == "FLYING-EYE")
		{
			auto flyingEye = std::make_unique<FlyingEye>(*this, player_->GetPlayerTransform());
			flyingEye->Initialize();
			flyingEye->SetPosition(tileTransform->pos);
			enemyMng_->AddEnemy(std::move(flyingEye));
		}
		if (enemyTile->GetName() == "MUSHROOM")
		{
			auto mushroom = std::make_unique<Mushroom>(*this, player_->GetPlayerTransform());
			mushroom->Initialize();
			auto mushroomTransform = mushroom->GetOwner()->GetComponent<TransformComponent>();
			mushroom->SetPosition(tileTransform->pos - 
				Vector2(mushroomTransform->w * mushroomTransform->scale, mushroomTransform->h * mushroomTransform->scale));
			enemyMng_->AddEnemy(std::move(mushroom));
		}
		if (enemyTile->GetName() == "SKELETON")
		{
			auto skeleton = std::make_unique<Skeleton>(*this, player_->GetPlayerTransform());
			skeleton->Initialize();
			auto skeletonTransform = skeleton->GetOwner()->GetComponent<TransformComponent>();
			skeleton->SetPosition(tileTransform->pos - 
				Vector2(skeletonTransform->w * skeletonTransform->scale, skeletonTransform->h * skeletonTransform->scale));
			enemyMng_->AddEnemy(std::move(skeleton));
		}
	}
}

void GameScene::LoadCheckPoint()
{
	for (const auto& checkPoint : entityMng_->mapLayers_["CHECK-POINT"])
	{
		auto tileTransform = checkPoint->GetComponent<TransformComponent>();
		if(checkPoint->GetName() == "BORN-FIRE")
		{
			auto& bornFire = levelMng_->AddCheckPoint<BornFire>();
			auto pos = tileTransform->pos - Vector2(0, bornFire->Height() - tileTransform->h * tileTransform->scale);
			bornFire->SetPosition(pos);
		}
	}
}

void GameScene::CheckRespawnPlayer()
{
	if (!player_->IsAlive())
	{
		waitTimer_ = wait_fade_out_time;
		updateFunc_ = &GameScene::FadeOutUpdate;
		renderFunc_ = &GameScene::FadeOutRender;
	}
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

void GameScene::FadeOutUpdate(const float& deltaTime)
{
	if (waitTimer_ <= 0.0f)
	{
		waitTimer_ = wait_respawn_player_time;
		updateFunc_ = &GameScene::RespawnPlayerUpdate;
		renderFunc_ = &GameScene::WaitRespawnPlayerRender;
	}
	waitTimer_ -= deltaTime;
}

void GameScene::GameUpdate(const float& deltaTime)
{
	player_->Input(deltaTime);
	/*----------------------------------------------------------------------------*/
	// Update player's STATE right after received player's INPUT
	// Purpose : update player's state before updating player's animation to AVOID BLINKING animation ( late animation )
	player_->UpdateState();
	/*----------------------------------------------------------------------------*/
	for (auto& spawner : spawners_)
	{
		spawner->Update(deltaTime);
	}
	collisionMng_->ApplyForce(deltaTime);
	CheckRespawnPlayer();
	enemyMng_->Update(deltaTime);
	entityMng_->Update(deltaTime);
	combatMng_->Update(deltaTime);
	environment_->Update(deltaTime);
	/*ProcessEnterBossArea();*/
	collisionMng_->ProcessCheckPoint();
	collisionMng_->PlatformResolution(deltaTime);
	collisionMng_->Update(deltaTime);
	collisionMng_->CombatCollision();
	levelMng_->Update(deltaTime);
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

void GameScene::RespawnPlayerUpdate(const float& deltaTime)
{
	levelMng_->RespawnPlayer();
	enemyMng_->ClearEnemy();
	entityMng_->ClearDestroyEntity();
	collisionMng_->ClearDestroyCollider();
	Camera::Instance().Update();
	LoadEnemy();
	if (waitTimer_ <= 0)
	{
		waitTimer_ = wait_fade_in_time;
		updateFunc_ = &GameScene::FadeInUpdate;
		renderFunc_ = &GameScene::FadeInRender;
	}
	waitTimer_ -= deltaTime;
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
	auto blendpara = 255 * waitTimer_ / wait_fade_in_time;
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendpara);
	DxLib::DrawBox(windowBox_.Left(), windowBox_.Top(),
		windowBox_.Right(), windowBox_.Bottom(),
		0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::FadeOutRender()
{
	environment_->RenderBackGround();
	entityMng_->Render();
	player_->RenderUI();
	auto blendpara = 255 * (wait_fade_out_time - waitTimer_) / wait_fade_out_time;
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendpara);
	DxLib::DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::WaitRespawnPlayerRender()
{
	DxLib::DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		0x000000, true);
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
