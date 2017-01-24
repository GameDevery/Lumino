using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;
using BinderMaker.Parser2;

namespace BinderMaker.Parser
{
#if true
    /// <summary>
    /// enum�Astruct�A�֐��|�C���^�̒�`�t�@�C�� (�w�b�_�t�@�C��) ����͂���
    /// </summary>
    class CLTypedefHeaderParser
    {
        #region struct

        // struct �����o
        private static readonly Parser<CLStructMember> StructMember =
            from type       in ParserUtils.TypeName.GenericToken()      // �^��
            from name       in ParserUtils.Identifier.GenericToken()    // �����o��
            from colon      in Parse.Char(';').GenericToken()           // ����
            from comment    in ParserUtils.DoxyLineComment2.Or(Parse.Return(""))     // �R�����g�������o����(opt)
            select new CLStructMember(type, name, comment);
        
        // struct ��`
        private static readonly Parser<CLStruct> StructDecl =
            from comment    in ParserUtils.DoxyLineComment1             // �R�����g
            from key1       in Parse.String("typedef").GenericToken()   // "typedef"
            from key12      in Parse.String("struct").GenericToken()    // "struct"
            from name       in ParserUtils.Identifier.GenericToken()    // ���O ("tagXXXX" �ƂȂ��Ă���͂�)
            from lbrace     in Parse.Char('{').GenericToken()           // {
            from members    in StructMember.Many()                      //  �����o
            from rbrace     in Parse.Char('}').GenericToken()           // }
            from name2      in ParserUtils.Identifier.GenericToken()    // typedef ��
            from end        in Parse.Char(';').GenericToken()           // ;
            select new CLStruct(comment, name2, members);

        #endregion

        #region enum

        // enum �����o�̒萔��`
        private static readonly Parser<string> EnumMemberValue =
            from equal      in Parse.Char('=').GenericToken()
            from value      in ParserUtils.IdentifierOrNumeric.GenericToken()
            select value;

        // enum �����o
        private static readonly Parser<CLEnumMember> EnumMember =
            from name       in ParserUtils.Identifier.GenericToken()
            from value      in EnumMemberValue.Or(Parse.Return(""))     // �萔�� opt
            from comma      in Parse.Char(',').GenericToken()           // �I�[,
            from comment    in ParserUtils.DoxyLineComment2.Or(Parse.Return(""))     // �R�����g�������o����(opt)
            select new CLEnumMember(name, value, comment);

        // enum ��`
        private static readonly Parser<CLEntity> EnumDecl =
            from comment    in ParserUtils.DoxyLineComment1             // �R�����g
            from key1       in Parse.String("typedef").GenericToken()   // "typedef"
            from start      in Parse.String("enum").GenericToken()      // "enum"
            from name       in ParserUtils.Identifier.GenericToken()    // ���O ("tagXXXX" �ƂȂ��Ă���͂�)
            from lbrace     in Parse.Char('{').GenericToken()           // {
            from members    in EnumMember.Many()                        // �����o
            from rbrace     in Parse.Char('}').GenericToken()           // }
            from name2      in ParserUtils.Identifier.GenericToken()    // typedef ��
            from end        in Parse.Char(';').GenericToken()           // ;
            select new CLEnum(comment, name2, members);

        #endregion

        #region �֐��|�C���^

        // �֐��|�C���^
        private static readonly Parser<CLEntity> FuncPointerDecl =
            from doc        in ApiDocument.DocumentComment        // �R�����g
            from start      in Parse.String("typedef").GenericToken()
            from type       in ParserUtils.TypeName.GenericToken()      // �߂�l�^
            from lparen1    in Parse.Char('(').GenericToken()
            from ast        in Parse.Char('*').GenericToken()
            from name       in ParserUtils.Identifier.GenericToken()    // ���O
            from rparen1    in Parse.Char(')').GenericToken()
            from lparen     in Parse.Char('(').GenericToken()
            from params1    in ApiFunc.FuncParamDecls.Or(Parse.Return<IEnumerable<Decls.ParamDecl>>(null))    // �������X�gopt
            from rparen     in Parse.Char(')').GenericToken()
            from end1       in Parse.Char(';').GenericToken()
            select new CLDelegate(doc, type, name, params1);

        #endregion

        #region �t���[�����[�N

        // ��`�v�f
        private static readonly Parser<CLEntity> DeclContent =
                StructDecl
            .Or(EnumDecl)
            .Or(FuncPointerDecl);

        // ���[�g�v�f
        private static readonly Parser<IEnumerable<CLEntity>> CompileUnit =
            from lead       in Parse.AnyChar.Except(Parse.String("/**")).Many()   // �t�@�C���擪����ŏ��̃h�L�������g�R�����g�܂ł�ǂݔ�΂� (/** �͏���Ȃ�)
            from decls      in DeclContent.Many()
            from any        in Parse.AnyChar.Many()     // ��͓K���ȕ���
            select decls;

        #endregion

        #region Methods

        public List<CLEntity> Analyze(string filePath)
        {
            string text = System.IO.File.ReadAllText(filePath);
            text = text.Replace("\r\n", "\n");  // ���s�R�[�h�� LF �ɓ���
            var decls = CompileUnit.Parse(text);
            return new List<CLEntity>(decls);
        }

        #endregion
    }
#endif
}
