﻿
#pragma once
#include "FlatCommon.h"

extern "C"
{
/**
    @brief 3次元のベクトルを定義します。
*/
struct LnVector3
{
    float x;
    float y;
    float z;
};
/**
    @brief クォータニオンを定義します。
    @details このクォータニオンクラスの乗算の順番は左から右です。つまり、q1 * q2 は、先に q1、次に q2 の回転を適用する事と同じです。
    この乗算順序は数学的な四元数の定義からは逆行している点に注意してください。
*/
struct LnQuaternion
{
    float x;
    float y;
    float z;
    float w;
};


/**
    @brief ピクセルフォーマット
*/
typedef enum tagLnPixelFormat
{
    /**
        @brief Unknown
    */
    LN_PIXEL_FORMAT_UNKNOWN = 0,

    /**
        @brief 8bit アルファ値のみのフォーマット
    */
    LN_PIXEL_FORMAT_A8 = 1,

    /**
        @brief RGBA オーダーの各要素 8bit フォーマット
    */
    LN_PIXEL_FORMAT_RGBA8 = 2,

    /**
        @brief RGB オーダーの各要素 8bit フォーマット
    */
    LN_PIXEL_FORMAT_RGB8 = 3,

    /**
        @brief RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    */
    LN_PIXEL_FORMAT_RGBA32F = 4,

} LnPixelFormat;

/**
    @brief テクスチャのピクセルフォーマット
*/
typedef enum tagLnTextureFormat
{
    /**
        @brief Unknown
    */
    LN_TEXTURE_FORMAT_UNKNOWN = 0,

    /**
        @brief RGBA オーダーの各要素 8bit フォーマット
    */
    LN_TEXTURE_FORMAT_RGBA8 = 1,

    /**
        @brief RGB オーダーの各要素 8bit フォーマット
    */
    LN_TEXTURE_FORMAT_RGB8 = 2,

    /**
        @brief RGBA オーダーの各要素 16bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_RGBA16F = 3,

    /**
        @brief RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_RGBA32F = 4,

    /**
        @brief 16bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_R16F = 5,

    /**
        @brief 32bit 浮動小数点フォーマット
    */
    LN_TEXTURE_FORMAT_R32F = 6,

    /**
        @brief 32bit の符号なし整数フォーマット
    */
    LN_TEXTURE_FORMAT_R32U = 7,

} LnTextureFormat;

/**
    @brief 深度バッファのフォーマット
*/
typedef enum tagLnDepthBufferFormat
{
    /**
        @brief S8 32 ビット
    */
    LN_DEPTH_BUFFER_FORMAT_D24S8 = 0,

} LnDepthBufferFormat;

typedef void(*LnUIEventHandlerCallback)(LnHandle __eventOwner, LnHandle p1);

typedef LnResult(*LnTestDelegateCallback)(LnHandle, int p1, int* outReturn);
LN_FLAT_API LnResult LnTestDelegate_Create(LnTestDelegateCallback callback, LnHandle* outDelegate);


/**
    @brief ベクトルの長さを返します。
    @param[in] vector3 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnVector3_Length(const LnVector3* vector3, float* outReturn);


/**
    @brief ベクトルの長さの 2 乗を返します。
    @param[in] vector3 : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnVector3_LengthSquared(const LnVector3* vector3, float* outReturn);


/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] x : 処理の基になるベクトルの X 要
    @param[in] y : 処理の基になるベクトルの Y 要
    @param[in] z : 処理の基になるベクトルの Z 要
    @param[out] outReturn : instance.
    @return 正規化されたベクトル
*/
LN_FLAT_API LnResult LnVector3_NormalizeXYZ(float x, float y, float z, LnVector3* outReturn);


/**
    @brief 指定ベクトルを正規化したベクトルを返します。
    @param[in] vec : 処理の基になるベクトル
    @param[out] outReturn : instance.
    @return 正規化されたベクトル
*/
LN_FLAT_API LnResult LnVector3_Normalize(const LnVector3* vec, LnVector3* outReturn);




//==============================================================================
// ln::Object

typedef LnResult(*LnObject_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnObject_OnSerialize_SetOverrideCallback(LnObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnObject_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::Serializer

/**
    @brief write
    @param[in] serializer : instance
*/
LN_FLAT_API LnResult LnSerializer_WriteBool(LnHandle serializer, const LnChar* name, LnBool value);
LN_FLAT_API LnResult LnSerializer_WriteBoolA(LnHandle serializer, const char* name, LnBool value);

/**
    @brief write
    @param[in] serializer : instance
*/
LN_FLAT_API LnResult LnSerializer_WriteInt(LnHandle serializer, const LnChar* name, int value);
LN_FLAT_API LnResult LnSerializer_WriteIntA(LnHandle serializer, const char* name, int value);

/**
    @brief write
    @param[in] serializer : instance
*/
LN_FLAT_API LnResult LnSerializer_WriteFloat(LnHandle serializer, const LnChar* name, float value);
LN_FLAT_API LnResult LnSerializer_WriteFloatA(LnHandle serializer, const char* name, float value);

/**
    @brief write
    @param[in] serializer : instance
*/
LN_FLAT_API LnResult LnSerializer_WriteString(LnHandle serializer, const LnChar* name, const LnChar* value);
LN_FLAT_API LnResult LnSerializer_WriteStringA(LnHandle serializer, const char* name, const char* value);

/**
    @brief write
    @param[in] serializer : instance
*/
LN_FLAT_API LnResult LnSerializer_WriteObject(LnHandle serializer, const LnChar* name, LnHandle value);
LN_FLAT_API LnResult LnSerializer_WriteObjectA(LnHandle serializer, const char* name, LnHandle value);

/**
    @brief read
    @param[in] serializer : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSerializer_ReadBool(LnHandle serializer, const LnChar* name, LnBool* outReturn);
LN_FLAT_API LnResult LnSerializer_ReadBoolA(LnHandle serializer, const char* name, LnBool* outReturn);

/**
    @brief read
    @param[in] serializer : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSerializer_ReadInt(LnHandle serializer, const LnChar* name, int* outReturn);
LN_FLAT_API LnResult LnSerializer_ReadIntA(LnHandle serializer, const char* name, int* outReturn);

/**
    @brief read
    @param[in] serializer : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSerializer_ReadFloat(LnHandle serializer, const LnChar* name, float* outReturn);
LN_FLAT_API LnResult LnSerializer_ReadFloatA(LnHandle serializer, const char* name, float* outReturn);

/**
    @brief read
    @param[in] serializer : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSerializer_ReadString(LnHandle serializer, const LnChar* name, const LnChar** outReturn);
LN_FLAT_API LnResult LnSerializer_ReadStringA(LnHandle serializer, const char* name, const char** outReturn);

/**
    @brief read
    @param[in] serializer : instance
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnSerializer_ReadObject(LnHandle serializer, const LnChar* name, LnHandle* outReturn);
LN_FLAT_API LnResult LnSerializer_ReadObjectA(LnHandle serializer, const char* name, LnHandle* outReturn);

/**
    @brief serialize
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnSerializer_Serialize(LnHandle value, const LnChar* basePath, const LnChar** outReturn);
LN_FLAT_API LnResult LnSerializer_SerializeA(LnHandle value, const char* basePath, const char** outReturn);

/**
    @brief serialize
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
*/
LN_FLAT_API LnResult LnSerializer_Deserialize(const LnChar* str, const LnChar* basePath, LnHandle* outReturn);
LN_FLAT_API LnResult LnSerializer_DeserializeA(const char* str, const char* basePath, LnHandle* outReturn);

typedef LnResult(*LnSerializer_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnSerializer_OnSerialize_SetOverrideCallback(LnSerializer_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnSerializer_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::EngineSettings

/**
    @brief メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480)
*/
LN_FLAT_API LnResult LnEngineSettings_SetMainWindowSize(int width, int height);

/**
    @brief メインウィンドウに対して作成されるバックバッファのサイズを設定します。(default: クライアント領域のサイズと同等)
*/
LN_FLAT_API LnResult LnEngineSettings_SetMainBackBufferSize(int width, int height);

/**
    @brief メインウィンドウのタイトル文字列を設定します。
*/
LN_FLAT_API LnResult LnEngineSettings_SetMainWindowTitle(const LnChar* title);
LN_FLAT_API LnResult LnEngineSettings_SetMainWindowTitleA(const char* title);

/**
    @brief アセットが保存されているディレクトリを登録します。
*/
LN_FLAT_API LnResult LnEngineSettings_AddAssetDirectory(const LnChar* path);
LN_FLAT_API LnResult LnEngineSettings_AddAssetDirectoryA(const char* path);

/**
    @brief アセットファイルを登録します。
*/
LN_FLAT_API LnResult LnEngineSettings_AddAssetArchive(const LnChar* fileFullPath, const LnChar* password);
LN_FLAT_API LnResult LnEngineSettings_AddAssetArchiveA(const char* fileFullPath, const char* password);

/**
    @brief デバッグ用のログファイルの出力有無を設定します。(default: Debug ビルドの場合true、それ以外は false)
*/
LN_FLAT_API LnResult LnEngineSettings_SetEngineLogEnabled(LnBool enabled);

/**
    @brief デバッグ用のログファイルの出力先ファイルパスを設定します。(default: Empty(実行ファイルのディレクトリへ出力))
*/
LN_FLAT_API LnResult LnEngineSettings_SetEngineLogFilePath(const LnChar* filePath);
LN_FLAT_API LnResult LnEngineSettings_SetEngineLogFilePathA(const char* filePath);

//==============================================================================
// ln::Engine

/**
    @brief エンジンの初期化処理を行います。
*/
LN_FLAT_API LnResult LnEngine_Initialize();

/**
    @brief エンジンの終了処理を行います。
*/
LN_FLAT_API LnResult LnEngine_Finalize();

/**
    @brief 
    @param[out] outReturn : instance.
    @return アプリケーションの終了が要求されている場合は false を返します。
*/
LN_FLAT_API LnResult LnEngine_Update(LnBool* outReturn);

/**
    @brief 。
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnEngine_MainUIView(LnHandle* outReturn);

//==============================================================================
// ln::Application

/**
    @brief エンジンの初期化処理が完了した後に呼び出されます。
    @param[in] application : instance
*/
LN_FLAT_API LnResult LnApplication_OnInit(LnHandle application);

/**
    @brief 毎フレーム呼び出されます。
    @param[in] application : instance
*/
LN_FLAT_API LnResult LnApplication_OnUpdate(LnHandle application);

/**
    @brief 
    @param[out] outApplication : instance.
*/
LN_FLAT_API LnResult LnApplication_Create(LnHandle* outApplication);

typedef LnResult(*LnApplication_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnApplication_OnSerialize_SetOverrideCallback(LnApplication_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnApplication_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnApplication_OnInit_OverrideCallback)(LnHandle application);
LN_FLAT_API LnResult LnApplication_OnInit_SetOverrideCallback(LnApplication_OnInit_OverrideCallback callback);
LN_FLAT_API LnResult LnApplication_OnInit_CallOverrideBase(LnHandle application);
typedef LnResult(*LnApplication_OnUpdate_OverrideCallback)(LnHandle application);
LN_FLAT_API LnResult LnApplication_OnUpdate_SetOverrideCallback(LnApplication_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnApplication_OnUpdate_CallOverrideBase(LnHandle application);

//==============================================================================
// ln::GraphicsResource

typedef LnResult(*LnGraphicsResource_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnGraphicsResource_OnSerialize_SetOverrideCallback(LnGraphicsResource_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnGraphicsResource_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::Texture

typedef LnResult(*LnTexture_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnTexture_OnSerialize_SetOverrideCallback(LnTexture_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnTexture_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::Texture2D

/**
    @brief アセットからテクスチャを読み込みます。
    @param[in] filePath : 読み込むファイルのパス
    @param[out] outReturn : instance. (このオブジェクトは不要になったら LnObject_Release で参照を開放する必要があります)
    @details サポートしているフォーマットは次の通りです。PNG(.png), JPG(.jpg), TGA(.tga), BMP(.bmp), GIF(.gif)
*/
LN_FLAT_API LnResult LnTexture2D_Load(const LnChar* filePath, LnHandle* outReturn);
LN_FLAT_API LnResult LnTexture2D_LoadA(const char* filePath, LnHandle* outReturn);

/**
    @brief テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    @param[in] width : 幅 (px 単位)
    @param[in] height : 高さ (px 単位)
    @param[out] outTexture2D : instance.
    @return 作成されたテクスチャ
*/
LN_FLAT_API LnResult LnTexture2D_Create(int width, int height, LnHandle* outTexture2D);

/**
    @brief テクスチャを作成します。
    @param[in] width : 幅 (px 単位)
    @param[in] height : 高さ (px 単位)
    @param[in] format : ピクセルフォーマット
    @param[out] outTexture2D : instance.
*/
LN_FLAT_API LnResult LnTexture2D_CreateWithFormat(int width, int height, LnTextureFormat format, LnHandle* outTexture2D);

/**
    @brief ローカルのファイルを読み込み、テクスチャを作成します。
    @param[in] filePath : 読み込むファイルのパス
    @param[in] format : ピクセルフォーマット
    @param[out] outTexture2D : instance.
    @details このメソッドは TextureImporter のユーティリティです。
*/
LN_FLAT_API LnResult LnTexture2D_CreateFromFile(const LnChar* filePath, LnTextureFormat format, LnHandle* outTexture2D);
LN_FLAT_API LnResult LnTexture2D_CreateFromFileA(const char* filePath, LnTextureFormat format, LnHandle* outTexture2D);

typedef LnResult(*LnTexture2D_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnTexture2D_OnSerialize_SetOverrideCallback(LnTexture2D_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnTexture2D_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::Component

typedef LnResult(*LnComponent_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnComponent_OnSerialize_SetOverrideCallback(LnComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::VisualComponent

/**
    @brief 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    @param[in] visualcomponent : instance
*/
LN_FLAT_API LnResult LnVisualComponent_SetVisible(LnHandle visualcomponent, LnBool value);

/**
    @brief 可視状態を取得します。
    @param[in] visualcomponent : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnVisualComponent_IsVisible(LnHandle visualcomponent, LnBool* outReturn);

typedef LnResult(*LnVisualComponent_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnVisualComponent_OnSerialize_SetOverrideCallback(LnVisualComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnVisualComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::SpriteComponent

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] spritecomponent : instance
*/
LN_FLAT_API LnResult LnSpriteComponent_SetTexture(LnHandle spritecomponent, LnHandle texture);

typedef LnResult(*LnSpriteComponent_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnSpriteComponent_OnSerialize_SetOverrideCallback(LnSpriteComponent_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnSpriteComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::ComponentList

/**
    @brief 
    @param[in] componentlist : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnComponentList_GetLength(LnHandle componentlist, int* outReturn);

/**
    @brief 
    @param[in] componentlist : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnComponentList_GetItem(LnHandle componentlist, int index, LnHandle* outReturn);

typedef LnResult(*LnComponentList_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnComponentList_OnSerialize_SetOverrideCallback(LnComponentList_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnComponentList_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::WorldObject

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetPosition(LnHandle worldobject, const LnVector3* pos);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetPositionXYZ(LnHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの位置を位置を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldObject_GetPosition(LnHandle worldobject, LnVector3* outReturn);

/**
    @brief このオブジェクトの回転を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetRotation(LnHandle worldobject, const LnQuaternion* rot);

/**
    @brief このオブジェクトの回転をオイラー角から設定します。(radian)
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetEulerAngles(LnHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの回転を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldObject_GetRotation(LnHandle worldobject, LnQuaternion* outReturn);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetScale(LnHandle worldobject, const LnVector3* scale);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetScaleS(LnHandle worldobject, float xyz);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetScaleXYZ(LnHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトの拡大率を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldObject_GetScale(LnHandle worldobject, LnVector3* outReturn);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetCenterPoint(LnHandle worldobject, const LnVector3* value);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] worldobject : instance
*/
LN_FLAT_API LnResult LnWorldObject_SetCenterPointXYZ(LnHandle worldobject, float x, float y, float z);

/**
    @brief このオブジェクトのローカルの中心位置を取得します。
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldObject_GetCenterPoint(LnHandle worldobject, LnVector3* outReturn);

/**
    @brief 
    @param[in] worldobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnWorldObject_GetComponents(LnHandle worldobject, LnHandle* outReturn);

typedef LnResult(*LnWorldObject_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnWorldObject_OnSerialize_SetOverrideCallback(LnWorldObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnWorldObject_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnWorldObject_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnWorldObject_OnUpdate_SetOverrideCallback(LnWorldObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnWorldObject_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

//==============================================================================
// ln::VisualObject

/**
    @brief 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    @param[in] visualobject : instance
*/
LN_FLAT_API LnResult LnVisualObject_SetVisible(LnHandle visualobject, LnBool value);

/**
    @brief 可視状態を取得します。
    @param[in] visualobject : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnVisualObject_IsVisible(LnHandle visualobject, LnBool* outReturn);

typedef LnResult(*LnVisualObject_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnVisualObject_OnSerialize_SetOverrideCallback(LnVisualObject_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnVisualObject_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnVisualObject_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnVisualObject_OnUpdate_SetOverrideCallback(LnVisualObject_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnVisualObject_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

//==============================================================================
// ln::Sprite

/**
    @brief スプライトが表示するテクスチャを設定します。
    @param[in] sprite : instance
*/
LN_FLAT_API LnResult LnSprite_SetTexture(LnHandle sprite, LnHandle value);

/**
    @brief 
    @param[in] sprite : instance
*/
LN_FLAT_API LnResult LnSprite_SetSourceRectXYWH(LnHandle sprite, float x, float y, float width, float height);

/**
    @brief test
    @param[in] sprite : instance
*/
LN_FLAT_API LnResult LnSprite_SetCallerTest(LnHandle sprite, LnHandle callback);

/**
    @brief init
    @param[out] outSprite : instance.
*/
LN_FLAT_API LnResult LnSprite_Create(LnHandle texture, float width, float height, LnHandle* outSprite);

typedef LnResult(*LnSprite_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnSprite_OnSerialize_SetOverrideCallback(LnSprite_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnSprite_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);
typedef LnResult(*LnSprite_OnUpdate_OverrideCallback)(LnHandle worldobject, float elapsedSeconds);
LN_FLAT_API LnResult LnSprite_OnUpdate_SetOverrideCallback(LnSprite_OnUpdate_OverrideCallback callback);
LN_FLAT_API LnResult LnSprite_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds);

//==============================================================================
// ln::UIEventArgs

/**
    @brief イベントの発生元となった要素を取得します。
    @param[in] uieventargs : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIEventArgs_Sender(LnHandle uieventargs, LnHandle* outReturn);

typedef LnResult(*LnUIEventArgs_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUIEventArgs_OnSerialize_SetOverrideCallback(LnUIEventArgs_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUIEventArgs_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::UILayoutElement

typedef LnResult(*LnUILayoutElement_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUILayoutElement_OnSerialize_SetOverrideCallback(LnUILayoutElement_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUILayoutElement_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::UIElement

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetPosition(LnHandle uielement, const LnVector3* pos);

/**
    @brief このオブジェクトの位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetPositionXYZ(LnHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトの位置を位置を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetPosition(LnHandle uielement, LnVector3* outReturn);

/**
    @brief このオブジェクトの回転を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetRotation(LnHandle uielement, const LnQuaternion* rot);

/**
    @brief このオブジェクトの回転をオイラー角から設定します。(radian)
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetEulerAngles(LnHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトの回転を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetRotation(LnHandle uielement, LnQuaternion* outReturn);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetScale(LnHandle uielement, const LnVector3* scale);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetScaleS(LnHandle uielement, float xyz);

/**
    @brief このオブジェクトの拡大率を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetScaleXY(LnHandle uielement, float x, float y);

/**
    @brief このオブジェクトの拡大率を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetScale(LnHandle uielement, LnVector3* outReturn);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetCenterPoint(LnHandle uielement, const LnVector3* value);

/**
    @brief このオブジェクトのローカルの中心位置を設定します。
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_SetCenterPointXYZ(LnHandle uielement, float x, float y, float z);

/**
    @brief このオブジェクトのローカルの中心位置を取得します。
    @param[in] uielement : instance
    @param[out] outReturn : instance.
*/
LN_FLAT_API LnResult LnUIElement_GetCenterPoint(LnHandle uielement, LnVector3* outReturn);

/**
    @brief Add element to container.
    @param[in] uielement : instance
*/
LN_FLAT_API LnResult LnUIElement_AddChild(LnHandle uielement, LnHandle child);

typedef LnResult(*LnUIElement_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUIElement_OnSerialize_SetOverrideCallback(LnUIElement_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUIElement_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::UIControl

typedef LnResult(*LnUIControl_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUIControl_OnSerialize_SetOverrideCallback(LnUIControl_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUIControl_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::UIButtonBase

/**
    @brief set text.
    @param[in] uibuttonbase : instance
*/
LN_FLAT_API LnResult LnUIButtonBase_SetText(LnHandle uibuttonbase, const LnChar* text);
LN_FLAT_API LnResult LnUIButtonBase_SetTextA(LnHandle uibuttonbase, const char* text);

typedef LnResult(*LnUIButtonBase_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUIButtonBase_OnSerialize_SetOverrideCallback(LnUIButtonBase_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUIButtonBase_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);

//==============================================================================
// ln::UIButton

/**
    @brief init.
    @param[out] outUIButton : instance.
*/
LN_FLAT_API LnResult LnUIButton_Create(LnHandle* outUIButton);

/**
    @brief Clicked イベントの通知を受け取るコールバックを登録します。
    @param[in] uibutton : instance
*/
LN_FLAT_API LnResult LnUIButton_ConnectOnClicked(LnHandle uibutton, LnUIEventHandlerCallback handler);

typedef LnResult(*LnUIButton_OnSerialize_OverrideCallback)(LnHandle object, LnHandle ar);
LN_FLAT_API LnResult LnUIButton_OnSerialize_SetOverrideCallback(LnUIButton_OnSerialize_OverrideCallback callback);
LN_FLAT_API LnResult LnUIButton_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar);



} // extern "C"

