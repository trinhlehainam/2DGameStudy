#pragma once

#include <memory>

class SceneManager;
class KeyboardInput;

class Engine
{
private:
	Engine() = default;
	Engine(const Engine&) = delete;
	Engine(Engine&&) = delete;
	Engine& operator = (const Engine&) = delete;
	Engine& operator = (Engine&&) = delete;

	std::unique_ptr<SceneManager> sceneMng_;
	std::unique_ptr<KeyboardInput> sceneInput_;
	unsigned int lastTickFrame = 0;

	~Engine() = default;
public:
	static Engine& Instance();
	void Start();
	void Run();
	void Stop();
};

