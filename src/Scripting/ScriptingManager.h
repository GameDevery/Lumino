
#pragma once

LN_NAMESPACE_BEGIN
class NlGraphNode;
class NlGraphPin;
class NlContext;
class NlGraph;
class NlGraphInterface;
using NlGraphNodePtr = RefPtr<NlGraphNode>;
using NlGraphPinPtr = RefPtr<NlGraphPin>;
using NlGraphPtr = RefPtr<NlGraph>;

enum class NlGraphNodeCategory
{
	Command,
	Function,
};

enum class NlGraphPinCategory
{
	CommandFlow,
	DataFlow,
};

enum class NlGraphPinDirection
{
	Input,
	Output,
};

/**
	@brief	
*/
class NlGraphNode
	: public Object
{
public:
	NlGraphNodeCategory GetCategory() const { return m_category; }

protected:
	virtual void Execute(NlContext* sc) = 0;

	NlGraphPin* CreatePin(NlGraphPinCategory category, NlGraphPinDirection direction, const StringRef& name);

protected:
	NlGraphNode();
	void Initialize(NlGraphNodeCategory category);

private:
	NlGraphNodeCategory	m_category;
	NlGraphPinPtr		m_inputCommandFlowPin;
	List<NlGraphPinPtr>	m_outputCommandFlowPinList;

	friend class NlContext;
};

/**
	@brief	
*/
class NlGraphSimpleCommandNode
	: public NlGraphNode
{
public:
	NlGraphSimpleCommandNode()
	{
		m_flowInput = CreatePin(NlGraphPinCategory::CommandFlow, NlGraphPinDirection::Input, _T("name"));
		m_flowOutput = CreatePin(NlGraphPinCategory::CommandFlow, NlGraphPinDirection::Output, _T("name"));
	}

	virtual ~NlGraphSimpleCommandNode() = default;

	NlGraphPin* GetFlowInputPin() const { return m_flowInput; }
	NlGraphPin* GetFlowOutputPin() const { return m_flowOutput; }

protected:
	//virtual void Execute(NlContext* sc);

private:
	NlGraphPin*		m_flowInput;
	NlGraphPin*		m_flowOutput;
};

/**
	@brief	
*/
class EntryPointNode
	: public NlGraphNode
{
public:
	const String& GetEntryPointName() const { return m_entryPointName; }
	NlGraphPin* GetFlowOutputPin() const { return m_flowOutput; }

LN_INTERNAL_ACCESS:
	EntryPointNode();
	void Initialize(const StringRef& name);

protected:
	virtual void Execute(NlContext* sc);

private:
	String				m_entryPointName;
	NlGraphPin*		m_flowOutput;
};

/**
	@brief	
	@note
			- CommandNode �� InputPin �́A���� Link �ł���B
			- CommandNode �� OutputPin �́A���� Link �ł��Ȃ��B
			- FunctionNode �� InputPin �́A���� Link �ł��Ȃ��B
			- FunctionNode �� OutputPin �́A���� Link �ł���B
*/
class NlGraphPin
	: public Object
{
public:
	virtual ~NlGraphPin() = default;
	
	NlGraphNode* GetOwnerNode() const { return m_ownerNode; }

	/** �����N��̃m�[�h���擾����B�����N����Ă��Ȃ���� nullptr ��Ԃ��B*/
	NlGraphNode* GetLinkedToNode();

	/** �w�肵���s���ւ̃����N���쐬���܂��B*/
	void MakeLinkTo(NlGraphPin* toPin);

	/** �w�肵���s���Ƃ̃����N���������܂��B*/
	void BreakLinkTo(NlGraphPin* toPin);

	// �ڑ��ۃ`�F�b�N�͕ʊ֐���

LN_INTERNAL_ACCESS:
	NlGraphPin();
	void Initialize(NlGraphNode* ownerNode, NlGraphPinCategory category, NlGraphPinDirection direction);

private:
	NlGraphNode*		m_ownerNode;
	NlGraphPinCategory	m_category;
	NlGraphPinDirection	m_direction;
	List<NlGraphPin*>	m_linkedTo;
};

/**
	@brief	
*/
class NlContext
	: public Object
{
public:
	NlContext();


	// �����p
	void CallInterface(NlGraphInterface* inter);

	void Goto(NlGraphPin* nextFlowPin);

	bool IsCompleted() const;

private:
	void GotoNode(NlGraphNode* next);
	void Step();

	NlGraphNode*	m_pc;				// ProgramCounter. ���� Step �Ŏ��s����� Command �m�[�h
	NlGraphNode*	m_lastExecutePC;
};

/**
	@brief	
*/
class NlGraph
	: public Object
{
public:
	void AddGraphNode(NlGraphNode* node);

private:
	List<RefPtr<NlGraphNode>>	m_nodeList;
};

/**
	@brief	
*/
class NlGraphInterface
	: public Object
{
public:
	NlGraph* GetGraph() const { return m_graph; }
	EntryPointNode* GetEntryPoint() const { return m_entryPoint; }

LN_INTERNAL_ACCESS:
	NlGraphInterface();
	virtual ~NlGraphInterface() = default;
	void Initialize();

private:
	NlGraphPtr	m_graph;
	EntryPointNode*	m_entryPoint;
};

/**
	@brief	
*/
class NlGraphClass
	: public Object
{
public:


private:
	List<EntryPointNode*>	m_entryPointList;
	NlGraphPtr			m_graph;
};

/**
	@brief	
*/
class NlGraphNodeLibrary
	: public Object
{
public:


private:

};

/**
	@brief	
*/
class NlHelper
{
public:
	static void LinkPins(NlGraphPin* pin1, NlGraphPin* pin2)
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
class NlNode_Print
	: public NlGraphSimpleCommandNode
{
public:
	NlNode_Print() = default;
	virtual ~NlNode_Print() = default;

protected:
	virtual void Execute(NlContext* sc) override;
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
