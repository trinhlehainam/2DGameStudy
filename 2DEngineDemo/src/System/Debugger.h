#pragma once

class Debugger
{
private:
	Debugger() = default;
	Debugger(const Debugger&) = delete;
	void operator = (const Debugger&) = delete;
	~Debugger() = default;
public:
	static Debugger& Instance();

	void Update();

	void DisplayPerformance();
};

