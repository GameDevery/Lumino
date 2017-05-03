
#pragma once
#include "../Common.h"
#include "../Game/Component.h"

LN_NAMESPACE_BEGIN
class DrawList;
class World;
class WorldObject;
using WorldObjectPtr = RefPtr<WorldObject>;

/**
	@brief		
*/
class WorldObject
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	Transform	transform;

public:
	static WorldObjectPtr Create();

	WorldObject();
	virtual ~WorldObject();
	void Initialize();

	void AddComponent(Component* component);

	/// �J�n����
	//virtual void OnStart();

	/// �t���[���X�V
	virtual void OnUpdate();

	/// �I������
	virtual void OnDestroy();

	virtual void OnRender(DrawList* context);

private:
	void Render(DrawList* context);
	void ReleaseComponents();
	List<RefPtr<Component>>	m_components;

	friend class World;
};

LN_NAMESPACE_END
