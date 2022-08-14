#pragma once
#include "Entity.h"
#include <vector>
#include <memory>
#include <functional>
#include <any>
/*
class CallbackBase
{
public:
	float time_left{ 0.0f };
	virtual void Run() = 0;
};

template <typename T, typename... Arguments>
class Callback : public CallbackBase
{
	std::function<void()> fn;

public:
	Callback(const std::shared_ptr<T>& ptr, void (T::*function)(Arguments...), Arguments... arguments) {
		std::weak_ptr w_ptr = ptr;
		fn = [w_ptr, function, arguments...]() {
			auto s_ptr = w_ptr.lock();
			if (s_ptr) {
				auto* obj = s_ptr.get();
				(*obj.*function)(arguments...);
			}
		};

	}
	void Run() {
		fn();
	}
};*/

enum class EventType
{
	None,
	Damage,
	Kill,
	Hit
};

class Objective
{
protected:
	bool completed = false;
public:
	virtual std::string AsText() const = 0;
	virtual void CheckEvent(EventType nType, int nCount, Entity* a, Entity* b) = 0;
	bool Completed() {
		return completed;
	}
public:
	static std::unique_ptr<Objective> Make();
};

class EventHandler
{
	std::unique_ptr<Objective> objective;
	
	//std::vector<std::unique_ptr<CallbackBase>> vCallbacks;

	struct DelayedFunction {
		void (*fn)(void*);
		float time_left;
		void* data;
	};
	std::vector<DelayedFunction> vDelayedFunctions;



public:
	EventHandler();
	
	


	//Damage # enemies with $
	//Kill # $s

	void Event(EventType nType, int nCount, Entity* a, Entity* b);
	void Update(float deltatime);
	
	//void RegisterCallback(CallbackBase* callback, int ms);

	void DelayedFunctionInvoke(void fn(void*), float ms, void* ptr = nullptr) {
		vDelayedFunctions.push_back({fn, ms / 1000.f, ptr});
	}

	void Reset();

	const Objective& Objective() { return *objective; }
};

