
#pragma once

LN_NAMESPACE_BEGIN
class ScriptGraphNode;
class ScriptGraphPin;
class ScriptContext;
class ScriptGraph;
class ScriptGraphInterface;
using ScriptGraphNodePtr = RefPtr<ScriptGraphNode>;
using ScriptGraphPinPtr = RefPtr<ScriptGraphPin>;
using ScriptGraphPtr = RefPtr<ScriptGraph>;

enum class ScriptGraphNodeCategory
{
	Command,
	Function,
};

enum class ScriptGraphPinCategory
{
	CommandFlow,
	DataFlow,
};

enum class ScriptGraphPinDirection
{
	Input,
	Output,
};

/**
	@brief	
*/
class ScriptGraphNode
	: public Object
{
public:
	ScriptGraphNodeCategory GetCategory() const { return m_category; }

protected:
	virtual void Execute(ScriptContext* sc) = 0;

	ScriptGraphPin* CreatePin(ScriptGraphPinCategory category, ScriptGraphPinDirection direction, const StringRef& name);

protected:
	ScriptGraphNode();
	void Initialize(ScriptGraphNodeCategory category);

private:
	ScriptGraphNodeCategory	m_category;
	ScriptGraphPinPtr		m_inputCommandFlowPin;
	List<ScriptGraphPinPtr>	m_outputCommandFlowPinList;

	friend class ScriptContext;
};

/**
	@brief	
*/
class ScriptGraphSimpleCommandNode
	: public ScriptGraphNode
{
public:
	ScriptGraphSimpleCommandNode()
	{
		m_flowInput = CreatePin(ScriptGraphPinCategory::CommandFlow, ScriptGraphPinDirection::Input, _T("name"));
		m_flowOutput = CreatePin(ScriptGraphPinCategory::CommandFlow, ScriptGraphPinDirection::Output, _T("name"));
	}

	virtual ~ScriptGraphSimpleCommandNode() = default;

	ScriptGraphPin* GetFlowInputPin() const { return m_flowInput; }
	ScriptGraphPin* GetFlowOutputPin() const { return m_flowOutput; }

protected:
	//virtual void Execute(ScriptContext* sc);

private:
	ScriptGraphPin*		m_flowInput;
	ScriptGraphPin*		m_flowOutput;
};

/**
	@brief	
*/
class EntryPointNode
	: public ScriptGraphNode
{
public:
	const String& GetEntryPointName() const { return m_entryPointName; }
	ScriptGraphPin* GetFlowOutputPin() const { return m_flowOutput; }

LN_INTERNAL_ACCESS:
	EntryPointNode();
	void Initialize(const StringRef& name);

protected:
	virtual void Execute(ScriptContext* sc);

private:
	String				m_entryPointName;
	ScriptGraphPin*		m_flowOutput;
};

/**
	@brief	
	@note
			- CommandNode �� InputPin �́A���� Link �ł���B
			- CommandNode �� OutputPin �́A���� Link �ł��Ȃ��B
			- FunctionNode �� InputPin �́A���� Link �ł��Ȃ��B
			- FunctionNode �� OutputPin �́A���� Link �ł���B
*/
class ScriptGraphPin
	: public Object
{
public:
	virtual ~ScriptGraphPin() = default;
	
	ScriptGraphNode* GetOwnerNode() const { return m_ownerNode; }

	/** �����N��̃m�[�h���擾����B�����N����Ă��Ȃ���� nullptr ��Ԃ��B*/
	ScriptGraphNode* GetLinkedToNode();

	/** �w�肵���s���ւ̃����N���쐬���܂��B*/
	void MakeLinkTo(ScriptGraphPin* toPin);

	/** �w�肵���s���Ƃ̃����N���������܂��B*/
	void BreakLinkTo(ScriptGraphPin* toPin);

	// �ڑ��ۃ`�F�b�N�͕ʊ֐���

LN_INTERNAL_ACCESS:
	ScriptGraphPin();
	void Initialize(ScriptGraphNode* ownerNode, ScriptGraphPinCategory category, ScriptGraphPinDirection direction);

private:
	ScriptGraphNode*		m_ownerNode;
	ScriptGraphPinCategory	m_category;
	ScriptGraphPinDirection	m_direction;
	List<ScriptGraphPin*>	m_linkedTo;
};

/**
	@brief	
*/
class ScriptContext
	: public Object
{
public:
	ScriptContext();


	// �����p
	void CallInterface(ScriptGraphInterface* inter);

	void Goto(ScriptGraphPin* nextFlowPin);

	bool IsCompleted() const;

private:
	void GotoNode(ScriptGraphNode* next);
	void Step();

	ScriptGraphNode*	m_pc;				// ProgramCounter. ���� Step �Ŏ��s����� Command �m�[�h
	ScriptGraphNode*	m_lastExecutePC;
};

/**
	@brief	
*/
class ScriptGraph
	: public Object
{
public:
	void AddGraphNode(ScriptGraphNode* node);

private:
	List<RefPtr<ScriptGraphNode>>	m_nodeList;
};

/**
	@brief	
*/
class ScriptGraphInterface
	: public Object
{
public:
	ScriptGraph* GetGraph() const { return m_graph; }
	EntryPointNode* GetEntryPoint() const { return m_entryPoint; }

LN_INTERNAL_ACCESS:
	ScriptGraphInterface();
	virtual ~ScriptGraphInterface() = default;
	void Initialize();

private:
	ScriptGraphPtr	m_graph;
	EntryPointNode*	m_entryPoint;
};

/**
	@brief	
*/
class ScriptGraphClass
	: public Object
{
public:


private:
	List<EntryPointNode*>	m_entryPointList;
	ScriptGraphPtr			m_graph;
};

/**
	@brief	
*/
class ScriptGraphNodeLibrary
	: public Object
{
public:


private:

};

/**
	@brief	
*/
class ScriptHelper
{
public:
	static void LinkPins(ScriptGraphPin* pin1, ScriptGraphPin* pin2)
	{
		LN_FAIL_CHECK_ARG(pin1 != nullptr) return;
		LN_FAIL_CHECK_ARG(pin2 != nullptr) return;
		// TODO: �ڑ��ۃ`�F�b�N
		pin1->MakeLinkTo(pin2);
	}
};

//------------------------------------------------------------------------------

/**
	@brief	
*/
class ScriptNode_Print
	: public ScriptGraphSimpleCommandNode
{
public:
	ScriptNode_Print() = default;
	virtual ~ScriptNode_Print() = default;

protected:
	virtual void Execute(ScriptContext* sc) override;
};


namespace detail {

class ScriptingManager
	: public RefObject
{
public:
	struct ConfigData
	{
	};

public:
	ScriptingManager();
	virtual ~ScriptingManager();
	void Initialize(const ConfigData& configData);

private:
};

} // namespace detail
LN_NAMESPACE_END
