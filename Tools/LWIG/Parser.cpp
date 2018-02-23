#include <memory>
#include "Parser.h"


#ifdef _MSC_VER     // start of disabling MSVC warnings
#pragma warning(push)
#pragma warning(disable:4146 4180 4244 4258 4267 4291 4345 4351 4355 4456 4457 4458 4459 4503 4624 4722 4800 4996)
#endif
#include <iostream>
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/DeclVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "clang/AST/TypeLoc.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/AST/Comment.h"
#include "clang/AST/CommentVisitor.h"

#ifdef _MSC_VER     // end of disabling MSVC warnings
#pragma warning(pop)
#endif


class SymbolDatabase;

namespace local
{
using namespace clang;
using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;


class LWIGVisitor : public DeclVisitor<LWIGVisitor, bool>
{
private:
	const SourceManager& m_sourceManager;
	::SymbolDatabase* m_db;

public:
	LWIGVisitor(CompilerInstance* CI, ::SymbolDatabase* db)
		: m_sourceManager(CI->getASTContext().getSourceManager())
		, m_db(db)
	{
	}

	std::vector<std::string> getAnnotation(Decl* decl)
	{
		std::vector<std::string> attrs;
		if (decl->hasAttrs())
		{
			for (auto& attr : decl->getAttrs())
			{
				auto* anno = dyn_cast<AnnotateAttr>(attr);
				if (anno != nullptr)
				{
					attrs.push_back(anno->getAnnotation().str());
				}
			}
		}
		return attrs;
	}

	void EnumerateDecl(DeclContext* aDeclContext)
	{
		for (DeclContext::decl_iterator i = aDeclContext->decls_begin(), e = aDeclContext->decls_end(); i != e; i++)
		{
			Decl *D = *i;
			//if (indentation.IndentLevel == 0) {
			//	errs() << "TopLevel : " << D->getDeclKindName();                                    // Decl�̌^�\��
			//	if (NamedDecl *N = dyn_cast<NamedDecl>(D))  errs() << " " << N->getNameAsString();  // NamedDecl�Ȃ疼�O�\��
			//	errs() << " (" << D->getLocation().printToString(SM) << ")\n";                      // �\�[�X��̏ꏊ�\��
			//}

			errs() << "TopLevel : " << D->getDeclKindName();
			if (NamedDecl *N = dyn_cast<NamedDecl>(D))  errs() << " " << N->getNameAsString();
			errs() << "\n";

			Visit(D);
		}
	}


	// class/struct/union�̏���
	bool VisitCXXRecordDecl(CXXRecordDecl *aCXXRecordDecl/*, bool aForce = false*/)
	{
		// �Q�Ɨp(class foo;�̂悤�Ȑ錾)�Ȃ�ǐՂ��Ȃ�
		if (!aCXXRecordDecl->isCompleteDefinition()) {
			return true;
		}

		auto attrs = getAnnotation(aCXXRecordDecl);

		//// ���O�����Ȃ�\�����Ȃ�(�������A�����\�����ꂽ��\������:Elaborated�p)
		//if (!aCXXRecordDecl->getIdentifier() && !aForce) {
		//	return true;
		//}


		// �N���X��`�̏���
		//errs() << "CXXRecordDecl : " << aCXXRecordDecl->getNameAsString() << " {\n";
		{
			//INDENTATION;

			//// �^�̎��
			//errs() << indentation << "Kind : ";
			//switch (aCXXRecordDecl->TagDecl::getTagKind()) {
			//case TTK_Struct:    errs() << "struct\n";       break;
			//case TTK_Interface: errs() << "__interface\n";  break;
			//case TTK_Union:     errs() << "union\n";        break;
			//case TTK_Class:     errs() << "class\n";        break;
			//case TTK_Enum:      errs() << "enum\n";         break;
			//default:            errs() << "unknown!!\n";    break;
			//}

			//// �A�m�e�[�V����(in �A�g���r���[�g)
			//printAnnotation(aCXXRecordDecl, "  Annotation : ", "\n");

			//// �e������t���O
			//if (aCXXRecordDecl->isUsed())           errs() << indentation << "  Used\n";
			//if (aCXXRecordDecl->isReferenced())     errs() << indentation << "  Referenced\n";
			//if (aCXXRecordDecl->isPolymorphic())    errs() << indentation << "  Polymorphic\n";
			//if (aCXXRecordDecl->isAbstract())       errs() << indentation << "  Abstract\n";
			//if (aCXXRecordDecl->isEmpty())          errs() << indentation << "  isEmpty\n";
			//if (isa<ClassTemplatePartialSpecializationDecl>(aCXXRecordDecl)) {
			//	errs() << indentation << "  This is a Template-Parcial-Specialization.\n";
			//}
			//else if (isa<ClassTemplateSpecializationDecl>(aCXXRecordDecl)) {
			//	errs() << indentation << "  This is a Template-Specialization.\n";
			//}

			//// ���N���X�̖�������
			//for (CXXRecordDecl::base_class_iterator Base = aCXXRecordDecl->bases_begin(), BaseEnd = aCXXRecordDecl->bases_end();
			//	Base != BaseEnd;
			//	++Base) {                                          // ���^�������o��(�Ⴆ�΁APolicy.Bool=0�̎��Abool�^��"_Bool"�ƂȂ�)
			//	errs() << indentation << "Base : ";
			//	printAccessSpec(Base->getAccessSpecifier());
			//	if (Base->isVirtual()) errs() << "virtual ";
			//	errs() << Base->getType().getAsString(Policy) << "\n";
			//}

			// �����o�[�̖�������
			EnumerateDecl(aCXXRecordDecl);
		}
		//errs() << indentation << "}\n";
		//errs() << indentation << "====================================>>>\n";
		return true;
	}

	// �����o�֐�
	bool VisitCXXMethodDecl(CXXMethodDecl* D)
	{
		auto attrs = getAnnotation(D);
		return true;
	}

	// �����o�ϐ�
	bool VisitFieldDecl(FieldDecl *aFieldDecl)
	{
		return true;
	}

	// typedef
	bool VisitTypedefDecl(TypedefDecl *aTypedefDecl)
	{
		return true;
	}

	// namespace
	bool VisitNamespaceDecl(NamespaceDecl *aNamespaceDecl)
	{
		return true;
	}

	// using
	bool VisitUsingDirectiveDecl(UsingDirectiveDecl *aUsingDirectiveDecl)
	{
		return true;
	}
};

//------------------------------------------------------------------------------
// �ȉ��A���܂蕶��

// ���̃C���^�t�F�[�X�́A�v���v���Z�b�T�̓�����ώ@������@��񋟂��܂��B
// https://clang.llvm.org/doxygen/classclang_1_1PPCallbacks.html
// #XXXX ���������Ƃ���A�}�N���W�J���s��ꂽ�Ƃ��ɌĂ΂��R�[���o�b�N���`�����肷��B
class LocalPPCallbacks : public PPCallbacks
{
public:
	LocalPPCallbacks(Preprocessor& pp, CompilerInstance* ci)
		: m_pp(pp)
		, m_ci(ci)
	{}

	virtual void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD, SourceRange Range, const MacroArgs *Args) override
	{
		auto stringRef = Lexer::getSourceText(
			CharSourceRange::getTokenRange(Range),
			m_ci->getSourceManager(),
			m_ci->getLangOpts());


		// Token �̎�ނ̖��O��Ԃ��B indentifer �ȂǁB
		//auto* name = MacroNameTok.getName();
		//printf("%s %d\n", stringRef.str().c_str(), Range.getBegin().getRawEncoding());
		auto loc = Range.getBegin();
		std::cout << "  " << m_ci->getSourceManager().getFileID(loc).getHashValue() << " " << loc.getRawEncoding() << "\n";

		const MacroInfo* MI = MD.getMacroInfo();

		MI->dump();

		for (unsigned ArgNo = 0U; ArgNo < MI->getNumArgs(); ++ArgNo)
		{
			const IdentifierInfo *Arg = *(MI->arg_begin() + ArgNo);
			const Token *ResultArgToks = Args->getUnexpArgument(ArgNo);
		}
	}

private:
	Preprocessor & m_pp;
	CompilerInstance*	m_ci;
};

// ASTConsumer �́AAST �̃G���g���|�C���g�ƂȂ鉽�炩�̗v�f���������Ƃ��ɂ����ʒm����B
// �ʏ�� HandleTranslationUnit() ������������΂悢�B
// https://clang.llvm.org/docs/RAVFrontendAction.html
// clang �͐F�X�� AST �̍쐬���@�������Ă���炵���AHandleTranslationUnit() �ȊO��
// �ʏ�̖|��P�ʈȊO�̉�͂ŉ����������Ƃ��Ɏg���悤���B
class LocalASTConsumer : public ASTConsumer
{
private:
	std::unique_ptr<LWIGVisitor> m_visitor;

public:
	explicit LocalASTConsumer(CompilerInstance* CI, ::SymbolDatabase* db)
		: m_visitor(std::make_unique<LWIGVisitor>(CI, db))
	{
		Preprocessor &PP = CI->getPreprocessor();
		PP.addPPCallbacks(llvm::make_unique<LocalPPCallbacks>(PP, CI));
	}

	virtual void HandleTranslationUnit(ASTContext& Context)
	{
		m_visitor->EnumerateDecl(Context.getTranslationUnitDecl());
	}
};

// FrontendAction �́A�R���p�C���t�����g�G���h�̋��ʂ̃G���g���|�C���g�B��̓I�ɉ��������́H��\�����߂Ɏg���B
// https://clang.llvm.org/docs/RAVFrontendAction.html
// https://clang.llvm.org/doxygen/classclang_1_1ASTFrontendAction.html
// �W������ ASTFrontendAction �̔h���Ƃ��āA�P�� AST ���_���v������AHTML �ɕϊ�������Ƃ������A�N�V�������p�ӂ���Ă���B
// ����� AST ��S�������Ńg���o�[�X�������̂� ASTFrontendAction ���g���B
class LocalFrontendAction : public ASTFrontendAction
{
public:
	::SymbolDatabase * m_db;

	LocalFrontendAction(::SymbolDatabase* db)
		: m_db(db)
	{}

	virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef file)
	{
		return llvm::make_unique<LocalASTConsumer>(&CI, m_db);
	}
};


// SilClangAnalyzer �̃|�C���^�� ���̃N���X�����ɂ킽�����߂̃t�@�N�g��
std::unique_ptr<FrontendActionFactory> NewLocalFrontendActionFactory(::SymbolDatabase* db)
{
	class SimpleFrontendActionFactory : public FrontendActionFactory
	{
	public:
		::SymbolDatabase * m_db;

		SimpleFrontendActionFactory(::SymbolDatabase* db)
			: m_db(db)
		{}

		clang::FrontendAction *create() override { return new LocalFrontendAction(m_db); }
	};

	return std::unique_ptr<FrontendActionFactory>(new SimpleFrontendActionFactory(db));
}

} // namespace

//------------------------------------------------------------------------------

int HeaderParser::parse(const Path& filePath, ::SymbolDatabase* db)
{
	// TODO: Path ���璼�� toLocalPath
	std::string localFilePath = filePath.getString().toStdString();

	const char* argv[] =
	{
		"",	// program name
		localFilePath.c_str(),
		"--",
		//"-I", "C:/Proj/LN/Lumino/Source/LuminoCore/Include",
		//"-I", "C:/Proj/LN/Lumino/Source/LuminoEngine/Include",
		"-D", "LN_NAMESPACE_BEGIN",
		"-D", "LN_NAMESPACE_END",
		"-fms-compatibility",		// Enable full Microsoft Visual C++ compatibility
		"-fms-extensions",			// Enable full Microsoft Visual C++ compatibility
		"-fmsc-version=1900",		// Microsoft compiler version number to report in _MSC_VER (0 = don't define it (default))
	};
	int argc = sizeof(argv) / sizeof(const char*);

	::clang::tooling::CommonOptionsParser op(argc, argv, ::llvm::cl::GeneralCategory);
	::clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());
	return Tool.run(local::NewLocalFrontendActionFactory(db).get());
}


