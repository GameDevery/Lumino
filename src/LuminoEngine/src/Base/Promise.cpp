﻿
#include "Internal.hpp"
#include <LuminoEngine/Base/Task.hpp>
#include <LuminoEngine/Base/Promise.hpp>
#include "../Engine/EngineManager.hpp"


namespace ln {

void PromiseBase::enqueueInvoke(const Ref<PromiseBase>& p)
{
	Ref<PromiseBase> pb = p;
	auto t = Task::create([pb]() {
		pb->invoke();
		enqueueThen(pb);
	});

	t->start();
}

void PromiseBase::enqueueThen(const Ref<PromiseBase>& p)
{
	Ref<PromiseBase> pb = p;
	auto t = Task::create([pb]() {
		pb->callNext();
	});

	detail::EngineDomain::engineManager()->mainThreadTaskDispatcher()->post(t);
}

} // namespace ln

