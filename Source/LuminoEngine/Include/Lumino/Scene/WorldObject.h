
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


	virtual void OnRender(DrawList* context);

	/// �I������
	virtual void OnDestroy();

LN_INTERNAL_ACCESS:
	const Matrix& GetCombinedGlobalMatrix() const { return m_combinedGlobalMatrix; }

private:
	void UpdateFrame();
	void Render(DrawList* context);
	void ReleaseComponents();

	List<RefPtr<Component>>		m_components;

	List<RefPtr<WorldObject>>	m_children;
	WorldObject*				m_parent;
	Matrix						m_combinedGlobalMatrix;
	bool						m_isAutoRelease;

	friend class World;
};

LN_NAMESPACE_END
