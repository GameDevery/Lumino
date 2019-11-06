
#include <ruby.h>
#include "LuminoRubyRuntimeManager.h"
#include <Lumino.FlatC.generated.h>



#define LN_TO_RUBY_FUNC(f) reinterpret_cast<VALUE(__cdecl *)(...)>(f)

inline VALUE LNI_TO_RUBY_VALUE(const char* v)   { return rb_str_new2(v); }
inline VALUE LNI_TO_RUBY_VALUE(LnBool v)        { return (v != LN_FALSE) ? Qtrue : Qfalse; }
inline VALUE LNI_TO_RUBY_VALUE(uint8_t v)       { return INT2FIX(v); }
inline VALUE LNI_TO_RUBY_VALUE(int v)           { return INT2NUM(v); }
inline VALUE LNI_TO_RUBY_VALUE(uint32_t v)      { return INT2NUM(v); }
inline VALUE LNI_TO_RUBY_VALUE(int64_t v)       { return INT2NUM(v); }
inline VALUE LNI_TO_RUBY_VALUE(uint64_t v)      { return INT2NUM(v); }
inline VALUE LNI_TO_RUBY_VALUE(float v)         { return DBL2NUM(v); }
inline VALUE LNI_TO_RUBY_VALUE(double v)        { return DBL2NUM(v); }

inline bool LNRB_VALUE_IS_OBJECT(VALUE v) { return (TYPE(v) == T_DATA) || (v == Qnil); }
inline bool LNRB_VALUE_IS_NUMBER(VALUE v) { return (TYPE(v) == T_FIXNUM || TYPE(v) == T_BIGNUM) || (v == Qnil); }
inline bool LNRB_VALUE_IS_FLOAT(VALUE v) { return (TYPE(v) == T_FLOAT) || (LNRB_VALUE_IS_NUMBER(v)) || (v == Qnil); }
inline bool LNRB_VALUE_IS_STRING(VALUE v) { return (TYPE(v) == T_STRING) || (v == Qnil); }
inline bool LNRB_VALUE_IS_BOOL(VALUE v) { return (TYPE(v) == T_FALSE || TYPE(v) == T_TRUE) || (v == Qnil); }

inline int LNRB_VALUE_TO_NUMBER(VALUE v) { return FIX2INT(v); }
inline float LNRB_VALUE_TO_FLOAT(VALUE v) { return (float)NUM2DBL(v); }
inline double LNRB_VALUE_TO_DOUBLE(VALUE v) { return NUM2DBL(v); }
inline const char* LNRB_VALUE_TO_STRING(VALUE v) { return StringValuePtr(v); }
inline LnBool LNRB_VALUE_TO_BOOL(VALUE v) { return (TYPE(v) == T_TRUE) ? LN_TRUE : LN_FALSE; }

extern "C" void InitLuminoRubyRuntimeManager();

extern "C" LN_FLAT_API LnResult LnEngine_Initialize();
extern "C" LN_FLAT_API LnResult LnEngine_Finalize();
extern "C" LN_FLAT_API LnResult LnEngine_Update(LnBool* outReturn);
extern "C" LN_FLAT_API LnResult LnEngine_MainUIView(LnHandle* outReturn);
extern "C" LN_FLAT_API void LnGraphicsResource_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API void LnTexture_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnTexture2D_Create(int width, int height, LnHandle* outTexture2D);
extern "C" LN_FLAT_API LnResult LnTexture2D_CreateWithFormat(int width, int height, LnTextureFormat format, LnHandle* outTexture2D);
extern "C" LN_FLAT_API LnResult LnTexture2D_CreateFromFile(const LnChar* filePath, LnTextureFormat format, LnHandle* outTexture2D);
extern "C" LN_FLAT_API void LnTexture2D_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetPosition(LnHandle worldobject, const LnVector3* pos);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetPositionXYZ(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_Position(LnHandle worldobject, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetRotation(LnHandle worldobject, const LnQuaternion* rot);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetEulerAngles(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_Rotation(LnHandle worldobject, LnQuaternion* outReturn);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetScale(LnHandle worldobject, const LnVector3* scale);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetScaleS(LnHandle worldobject, float xyz);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetScaleXYZ(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_Scale(LnHandle worldobject, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetCenterPoint(LnHandle worldobject, const LnVector3* value);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetCenterPointXYZ(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_CenterPoint(LnHandle worldobject, LnVector3* outReturn);
extern "C" LN_FLAT_API void LnWorldObject_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnVisualObject_SetVisible(LnHandle visualobject, LnBool value);
extern "C" LN_FLAT_API LnResult LnVisualObject_IsVisible(LnHandle visualobject, LnBool* outReturn);
extern "C" LN_FLAT_API void LnVisualObject_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnSprite_SetTexture(LnHandle sprite, LnHandle texture);
extern "C" LN_FLAT_API LnResult LnSprite_SetSourceRectXYWH(LnHandle sprite, float x, float y, float width, float height);
extern "C" LN_FLAT_API LnResult LnSprite_Create(LnHandle texture, float width, float height, LnHandle* outSprite);
extern "C" LN_FLAT_API void LnSprite_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnUIEventArgs_Sender(LnHandle uieventargs, LnHandle* outReturn);
extern "C" LN_FLAT_API void LnUIEventArgs_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API void LnUILayoutElement_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnUIElement_SetPosition(LnHandle uielement, const LnVector3* pos);
extern "C" LN_FLAT_API LnResult LnUIElement_SetPositionXYZ(LnHandle uielement, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnUIElement_Position(LnHandle uielement, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnUIElement_SetRotation(LnHandle uielement, const LnQuaternion* rot);
extern "C" LN_FLAT_API LnResult LnUIElement_SetEulerAngles(LnHandle uielement, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnUIElement_Rotation(LnHandle uielement, LnQuaternion* outReturn);
extern "C" LN_FLAT_API LnResult LnUIElement_SetScale(LnHandle uielement, const LnVector3* scale);
extern "C" LN_FLAT_API LnResult LnUIElement_SetScaleS(LnHandle uielement, float xyz);
extern "C" LN_FLAT_API LnResult LnUIElement_SetScaleXY(LnHandle uielement, float x, float y);
extern "C" LN_FLAT_API LnResult LnUIElement_Scale(LnHandle uielement, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnUIElement_SetCenterPoint(LnHandle uielement, const LnVector3* value);
extern "C" LN_FLAT_API LnResult LnUIElement_SetCenterPointXYZ(LnHandle uielement, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnUIElement_CenterPoint(LnHandle uielement, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnUIElement_AddChild(LnHandle uielement, LnHandle child);
extern "C" LN_FLAT_API void LnUIElement_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnUIControl_AddChild(LnHandle uicontrol, LnHandle child);
extern "C" LN_FLAT_API void LnUIControl_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnUIButtonBase_SetText(LnHandle uibuttonbase, const LnChar* text);
extern "C" LN_FLAT_API void LnUIButtonBase_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnUIButton_Create(LnHandle* outUIButton);
extern "C" LN_FLAT_API LnResult LnUIButton_ConnectOnClicked(LnHandle uibutton, LnUIEventHandlerCallback handler);
extern "C" LN_FLAT_API void LnUIButton_SetManagedTypeInfoId(int64_t id);


VALUE g_enum_PixelFormat;
VALUE g_enum_TextureFormat;
VALUE g_enum_DepthBufferFormat;

VALUE g_rootModule;
VALUE g_class_Engine;
VALUE g_class_GraphicsResource;
VALUE g_class_Texture;
VALUE g_class_Texture2D;
VALUE g_class_WorldObject;
VALUE g_class_VisualObject;
VALUE g_class_Sprite;
VALUE g_class_UIEventArgs;
VALUE g_class_UILayoutElement;
VALUE g_class_UIElement;
VALUE g_class_UIControl;
VALUE g_class_UIButtonBase;
VALUE g_class_UIButton;


//==============================================================================
// ln::Vector3

VALUE g_class_Vector3;

void LnVector3_delete(LnVector3* obj)
{
    free(obj);
}

VALUE LnVector3_allocate( VALUE klass )
{
    VALUE obj;
    LnVector3* internalObj;

    internalObj = (LnVector3*)malloc(sizeof(LnVector3));
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnVector3_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LnVector3_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LnVector3));

    return obj;
}

//==============================================================================
// ln::Quaternion

VALUE g_class_Quaternion;

void LnQuaternion_delete(LnQuaternion* obj)
{
    free(obj);
}

VALUE LnQuaternion_allocate( VALUE klass )
{
    VALUE obj;
    LnQuaternion* internalObj;

    internalObj = (LnQuaternion*)malloc(sizeof(LnQuaternion));
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnQuaternion_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LnQuaternion_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LnQuaternion));

    return obj;
}

//==============================================================================
// ln::Engine

struct Wrap_Engine
{
    Wrap_Engine()
    {}
};

static VALUE Wrap_LnEngine_Initialize(int argc, VALUE* argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnEngine_Initialize();
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Engine::initialize - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngine_Finalize(int argc, VALUE* argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnEngine_Finalize();
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Engine::finalize - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngine_Update(int argc, VALUE* argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        {
            LnBool _outReturn;
            LnResult errorCode = LnEngine_Update(&_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Engine::update - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngine_MainUIView(int argc, VALUE* argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        {
            LnHandle _outReturn;
            LnResult errorCode = LnEngine_MainUIView(&_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LuminoRubyRuntimeManager::instance->wrapObject(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Engine::mainUIView - wrong argument type.");
    return Qnil;
}

//==============================================================================
// ln::GraphicsResource

struct Wrap_GraphicsResource
    : public Wrap_Object
{
    Wrap_GraphicsResource()
    {}
};

static void LnGraphicsResource_delete(Wrap_GraphicsResource* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnGraphicsResource_mark(Wrap_GraphicsResource* obj)
{
	
}

static VALUE LnGraphicsResource_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_GraphicsResource* internalObj;

    internalObj = new Wrap_GraphicsResource();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnGraphicsResource_allocate");
    obj = Data_Wrap_Struct(klass, LnGraphicsResource_mark, LnGraphicsResource_delete, internalObj);

    return obj;
}

static VALUE LnGraphicsResource_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_GraphicsResource* internalObj;

    internalObj = new Wrap_GraphicsResource();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnGraphicsResource_allocate");
    obj = Data_Wrap_Struct(klass, LnGraphicsResource_mark, LnGraphicsResource_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

//==============================================================================
// ln::Texture

struct Wrap_Texture
    : public Wrap_GraphicsResource
{
    Wrap_Texture()
    {}
};

static void LnTexture_delete(Wrap_Texture* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnTexture_mark(Wrap_Texture* obj)
{
	
}

static VALUE LnTexture_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_Texture* internalObj;

    internalObj = new Wrap_Texture();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnTexture_allocate");
    obj = Data_Wrap_Struct(klass, LnTexture_mark, LnTexture_delete, internalObj);

    return obj;
}

static VALUE LnTexture_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_Texture* internalObj;

    internalObj = new Wrap_Texture();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnTexture_allocate");
    obj = Data_Wrap_Struct(klass, LnTexture_mark, LnTexture_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

//==============================================================================
// ln::Texture2D

struct Wrap_Texture2D
    : public Wrap_Texture
{
    Wrap_Texture2D()
    {}
};

static void LnTexture2D_delete(Wrap_Texture2D* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnTexture2D_mark(Wrap_Texture2D* obj)
{
	
}

static VALUE LnTexture2D_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_Texture2D* internalObj;

    internalObj = new Wrap_Texture2D();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnTexture2D_allocate");
    obj = Data_Wrap_Struct(klass, LnTexture2D_mark, LnTexture2D_delete, internalObj);

    return obj;
}

static VALUE LnTexture2D_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_Texture2D* internalObj;

    internalObj = new Wrap_Texture2D();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnTexture2D_allocate");
    obj = Data_Wrap_Struct(klass, LnTexture2D_mark, LnTexture2D_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnTexture2D_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_Texture2D* selfObj;
    Data_Get_Struct(self, Wrap_Texture2D, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "2", &width, &height);
        if (LNRB_VALUE_IS_NUMBER(width) && LNRB_VALUE_IS_NUMBER(height))
        {
            int _width = LNRB_VALUE_TO_NUMBER(width);
            int _height = LNRB_VALUE_TO_NUMBER(height);
            LnResult errorCode = LnTexture2D_Create(_width, _height, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self);
            return Qnil;
        }
    }
    if (3 <= argc && argc <= 3) {
        VALUE width;
        VALUE height;
        VALUE format;
        rb_scan_args(argc, argv, "3", &width, &height, &format);
        if (LNRB_VALUE_IS_NUMBER(width) && LNRB_VALUE_IS_NUMBER(height) && LNRB_VALUE_IS_NUMBER(format))
        {
            int _width = LNRB_VALUE_TO_NUMBER(width);
            int _height = LNRB_VALUE_TO_NUMBER(height);
            LnTextureFormat _format = (LnTextureFormat)FIX2INT(format);
            LnResult errorCode = LnTexture2D_CreateWithFormat(_width, _height, _format, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self);
            return Qnil;
        }
    }
    if (1 <= argc && argc <= 2) {
        VALUE filePath;
        VALUE format;
        rb_scan_args(argc, argv, "11", &filePath, &format);
        if (LNRB_VALUE_IS_STRING(filePath) && LNRB_VALUE_IS_NUMBER(format))
        {
            const char* _filePath = LNRB_VALUE_TO_STRING(filePath);
            LnTextureFormat _format = (format != Qnil) ? (LnTextureFormat)FIX2INT(format) : (LnTextureFormat)1;
            LnResult errorCode = LnTexture2D_CreateFromFileA(_filePath, _format, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Texture2D::init - wrong argument type.");
    return Qnil;
}

//==============================================================================
// ln::WorldObject

struct Wrap_WorldObject
    : public Wrap_Object
{
    Wrap_WorldObject()
    {}
};

static void LnWorldObject_delete(Wrap_WorldObject* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnWorldObject_mark(Wrap_WorldObject* obj)
{
	
}

static VALUE LnWorldObject_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_WorldObject* internalObj;

    internalObj = new Wrap_WorldObject();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnWorldObject_allocate");
    obj = Data_Wrap_Struct(klass, LnWorldObject_mark, LnWorldObject_delete, internalObj);

    return obj;
}

static VALUE LnWorldObject_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_WorldObject* internalObj;

    internalObj = new Wrap_WorldObject();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnWorldObject_allocate");
    obj = Data_Wrap_Struct(klass, LnWorldObject_mark, LnWorldObject_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnWorldObject_SetPosition(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE pos;
        rb_scan_args(argc, argv, "1", &pos);
        if (LNRB_VALUE_IS_OBJECT(pos))
        {
            LnVector3* tmp__pos; Data_Get_Struct(pos, LnVector3, tmp__pos);LnVector3& _pos = *tmp__pos;
            LnResult errorCode = LnWorldObject_SetPosition(selfObj->handle, &_pos);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "21", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = (z != Qnil) ? LNRB_VALUE_TO_FLOAT(z) : 0.000000;
            LnResult errorCode = LnWorldObject_SetPositionXYZ(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setPosition - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Position(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnWorldObject_Position(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::position - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_SetRotation(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE rot;
        rb_scan_args(argc, argv, "1", &rot);
        if (LNRB_VALUE_IS_OBJECT(rot))
        {
            LnQuaternion* tmp__rot; Data_Get_Struct(rot, LnQuaternion, tmp__rot);LnQuaternion& _rot = *tmp__rot;
            LnResult errorCode = LnWorldObject_SetRotation(selfObj->handle, &_rot);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setRotation - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_SetEulerAngles(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            LnResult errorCode = LnWorldObject_SetEulerAngles(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setEulerAngles - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Rotation(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnQuaternion _outReturn;
            LnResult errorCode = LnWorldObject_Rotation(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnQuaternion_allocate(g_class_Quaternion);
            *((LnQuaternion*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::rotation - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_SetScale(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE scale;
        rb_scan_args(argc, argv, "1", &scale);
        if (LNRB_VALUE_IS_OBJECT(scale))
        {
            LnVector3* tmp__scale; Data_Get_Struct(scale, LnVector3, tmp__scale);LnVector3& _scale = *tmp__scale;
            LnResult errorCode = LnWorldObject_SetScale(selfObj->handle, &_scale);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (1 <= argc && argc <= 1) {
        VALUE xyz;
        rb_scan_args(argc, argv, "1", &xyz);
        if (LNRB_VALUE_IS_FLOAT(xyz))
        {
            float _xyz = LNRB_VALUE_TO_FLOAT(xyz);
            LnResult errorCode = LnWorldObject_SetScaleS(selfObj->handle, _xyz);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "21", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = (z != Qnil) ? LNRB_VALUE_TO_FLOAT(z) : 1.000000;
            LnResult errorCode = LnWorldObject_SetScaleXYZ(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setScale - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Scale(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnWorldObject_Scale(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::scale - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_SetCenterPoint(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value))
        {
            LnVector3* tmp__value; Data_Get_Struct(value, LnVector3, tmp__value);LnVector3& _value = *tmp__value;
            LnResult errorCode = LnWorldObject_SetCenterPoint(selfObj->handle, &_value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "21", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = (z != Qnil) ? LNRB_VALUE_TO_FLOAT(z) : 0.000000;
            LnResult errorCode = LnWorldObject_SetCenterPointXYZ(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setCenterPoint - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_CenterPoint(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnWorldObject_CenterPoint(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::centerPoint - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_OnUpdate(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE elapsedSeconds;
        rb_scan_args(argc, argv, "1", &elapsedSeconds);
        if (LNRB_VALUE_IS_FLOAT(elapsedSeconds))
        {
            float _elapsedSeconds = LNRB_VALUE_TO_FLOAT(elapsedSeconds);
            LnResult errorCode = LnWorldObject_OnUpdate_CallOverrideBase(selfObj->handle, _elapsedSeconds);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::onUpdate - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnWorldObject_OnUpdate_OverrideCallback(LnHandle worldobject, float elapsedSeconds)
{
    VALUE obj = LuminoRubyRuntimeManager::instance->wrapObject(worldobject);
    VALUE retval = rb_funcall(obj, rb_intern("on_update"), 1, LNI_TO_RUBY_VALUE(elapsedSeconds));
    return LN_SUCCESS;
}
//==============================================================================
// ln::VisualObject

struct Wrap_VisualObject
    : public Wrap_WorldObject
{
    Wrap_VisualObject()
    {}
};

static void LnVisualObject_delete(Wrap_VisualObject* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnVisualObject_mark(Wrap_VisualObject* obj)
{
	
}

static VALUE LnVisualObject_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_VisualObject* internalObj;

    internalObj = new Wrap_VisualObject();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnVisualObject_allocate");
    obj = Data_Wrap_Struct(klass, LnVisualObject_mark, LnVisualObject_delete, internalObj);

    return obj;
}

static VALUE LnVisualObject_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_VisualObject* internalObj;

    internalObj = new Wrap_VisualObject();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnVisualObject_allocate");
    obj = Data_Wrap_Struct(klass, LnVisualObject_mark, LnVisualObject_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnVisualObject_SetVisible(int argc, VALUE* argv, VALUE self)
{
    Wrap_VisualObject* selfObj;
    Data_Get_Struct(self, Wrap_VisualObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_BOOL(value))
        {
            LnBool _value = LNRB_VALUE_TO_BOOL(value);
            LnResult errorCode = LnVisualObject_SetVisible(selfObj->handle, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::VisualObject::setVisible - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVisualObject_IsVisible(int argc, VALUE* argv, VALUE self)
{
    Wrap_VisualObject* selfObj;
    Data_Get_Struct(self, Wrap_VisualObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnBool _outReturn;
            LnResult errorCode = LnVisualObject_IsVisible(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::VisualObject::isVisible - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnVisualObject_OnUpdate_OverrideCallback(LnHandle worldobject, float elapsedSeconds)
{
    VALUE obj = LuminoRubyRuntimeManager::instance->wrapObject(worldobject);
    VALUE retval = rb_funcall(obj, rb_intern("on_update"), 1, LNI_TO_RUBY_VALUE(elapsedSeconds));
    return LN_SUCCESS;
}
//==============================================================================
// ln::Sprite

struct Wrap_Sprite
    : public Wrap_VisualObject
{
    Wrap_Sprite()
    {}
};

static void LnSprite_delete(Wrap_Sprite* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnSprite_mark(Wrap_Sprite* obj)
{
	
}

static VALUE LnSprite_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_Sprite* internalObj;

    internalObj = new Wrap_Sprite();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnSprite_allocate");
    obj = Data_Wrap_Struct(klass, LnSprite_mark, LnSprite_delete, internalObj);

    return obj;
}

static VALUE LnSprite_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_Sprite* internalObj;

    internalObj = new Wrap_Sprite();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnSprite_allocate");
    obj = Data_Wrap_Struct(klass, LnSprite_mark, LnSprite_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnSprite_SetTexture(int argc, VALUE* argv, VALUE self)
{
    Wrap_Sprite* selfObj;
    Data_Get_Struct(self, Wrap_Sprite, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE texture;
        rb_scan_args(argc, argv, "1", &texture);
        if (LNRB_VALUE_IS_OBJECT(texture))
        {
            LnHandle _texture = LuminoRubyRuntimeManager::instance->getHandle(texture);
            LnResult errorCode = LnSprite_SetTexture(selfObj->handle, _texture);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Sprite::setTexture - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSprite_SetSourceRectXYWH(int argc, VALUE* argv, VALUE self)
{
    Wrap_Sprite* selfObj;
    Data_Get_Struct(self, Wrap_Sprite, selfObj);
    if (4 <= argc && argc <= 4) {
        VALUE x;
        VALUE y;
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "4", &x, &y, &width, &height);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(width) && LNRB_VALUE_IS_FLOAT(height))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _width = LNRB_VALUE_TO_FLOAT(width);
            float _height = LNRB_VALUE_TO_FLOAT(height);
            LnResult errorCode = LnSprite_SetSourceRectXYWH(selfObj->handle, _x, _y, _width, _height);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Sprite::setSourceRect - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSprite_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_Sprite* selfObj;
    Data_Get_Struct(self, Wrap_Sprite, selfObj);
    if (3 <= argc && argc <= 3) {
        VALUE texture;
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "3", &texture, &width, &height);
        if (LNRB_VALUE_IS_OBJECT(texture) && LNRB_VALUE_IS_FLOAT(width) && LNRB_VALUE_IS_FLOAT(height))
        {
            LnHandle _texture = LuminoRubyRuntimeManager::instance->getHandle(texture);
            float _width = LNRB_VALUE_TO_FLOAT(width);
            float _height = LNRB_VALUE_TO_FLOAT(height);
            LnResult errorCode = LnSprite_Create(_texture, _width, _height, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Sprite::init - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnSprite_OnUpdate_OverrideCallback(LnHandle worldobject, float elapsedSeconds)
{
    VALUE obj = LuminoRubyRuntimeManager::instance->wrapObject(worldobject);
    VALUE retval = rb_funcall(obj, rb_intern("on_update"), 1, LNI_TO_RUBY_VALUE(elapsedSeconds));
    return LN_SUCCESS;
}
//==============================================================================
// ln::UIEventArgs

struct Wrap_UIEventArgs
    : public Wrap_Object
{
    Wrap_UIEventArgs()
    {}
};

static void LnUIEventArgs_delete(Wrap_UIEventArgs* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnUIEventArgs_mark(Wrap_UIEventArgs* obj)
{
	
}

static VALUE LnUIEventArgs_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UIEventArgs* internalObj;

    internalObj = new Wrap_UIEventArgs();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIEventArgs_allocate");
    obj = Data_Wrap_Struct(klass, LnUIEventArgs_mark, LnUIEventArgs_delete, internalObj);

    return obj;
}

static VALUE LnUIEventArgs_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UIEventArgs* internalObj;

    internalObj = new Wrap_UIEventArgs();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIEventArgs_allocate");
    obj = Data_Wrap_Struct(klass, LnUIEventArgs_mark, LnUIEventArgs_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnUIEventArgs_Sender(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIEventArgs* selfObj;
    Data_Get_Struct(self, Wrap_UIEventArgs, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnHandle _outReturn;
            LnResult errorCode = LnUIEventArgs_Sender(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LuminoRubyRuntimeManager::instance->wrapObject(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::UIEventArgs::sender - wrong argument type.");
    return Qnil;
}

//==============================================================================
// ln::UILayoutElement

struct Wrap_UILayoutElement
    : public Wrap_Object
{
    Wrap_UILayoutElement()
    {}
};

static void LnUILayoutElement_delete(Wrap_UILayoutElement* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnUILayoutElement_mark(Wrap_UILayoutElement* obj)
{
	
}

static VALUE LnUILayoutElement_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UILayoutElement* internalObj;

    internalObj = new Wrap_UILayoutElement();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUILayoutElement_allocate");
    obj = Data_Wrap_Struct(klass, LnUILayoutElement_mark, LnUILayoutElement_delete, internalObj);

    return obj;
}

static VALUE LnUILayoutElement_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UILayoutElement* internalObj;

    internalObj = new Wrap_UILayoutElement();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUILayoutElement_allocate");
    obj = Data_Wrap_Struct(klass, LnUILayoutElement_mark, LnUILayoutElement_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

//==============================================================================
// ln::UIElement

struct Wrap_UIElement
    : public Wrap_UILayoutElement
{
    Wrap_UIElement()
    {}
};

static void LnUIElement_delete(Wrap_UIElement* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnUIElement_mark(Wrap_UIElement* obj)
{
	
}

static VALUE LnUIElement_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UIElement* internalObj;

    internalObj = new Wrap_UIElement();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIElement_allocate");
    obj = Data_Wrap_Struct(klass, LnUIElement_mark, LnUIElement_delete, internalObj);

    return obj;
}

static VALUE LnUIElement_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UIElement* internalObj;

    internalObj = new Wrap_UIElement();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIElement_allocate");
    obj = Data_Wrap_Struct(klass, LnUIElement_mark, LnUIElement_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnUIElement_SetPosition(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE pos;
        rb_scan_args(argc, argv, "1", &pos);
        if (LNRB_VALUE_IS_OBJECT(pos))
        {
            LnVector3* tmp__pos; Data_Get_Struct(pos, LnVector3, tmp__pos);LnVector3& _pos = *tmp__pos;
            LnResult errorCode = LnUIElement_SetPosition(selfObj->handle, &_pos);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "21", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = (z != Qnil) ? LNRB_VALUE_TO_FLOAT(z) : 0.000000;
            LnResult errorCode = LnUIElement_SetPositionXYZ(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::setPosition - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_Position(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnUIElement_Position(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::position - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_SetRotation(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE rot;
        rb_scan_args(argc, argv, "1", &rot);
        if (LNRB_VALUE_IS_OBJECT(rot))
        {
            LnQuaternion* tmp__rot; Data_Get_Struct(rot, LnQuaternion, tmp__rot);LnQuaternion& _rot = *tmp__rot;
            LnResult errorCode = LnUIElement_SetRotation(selfObj->handle, &_rot);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::setRotation - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_SetEulerAngles(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            LnResult errorCode = LnUIElement_SetEulerAngles(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::setEulerAngles - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_Rotation(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnQuaternion _outReturn;
            LnResult errorCode = LnUIElement_Rotation(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnQuaternion_allocate(g_class_Quaternion);
            *((LnQuaternion*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::rotation - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_SetScale(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE scale;
        rb_scan_args(argc, argv, "1", &scale);
        if (LNRB_VALUE_IS_OBJECT(scale))
        {
            LnVector3* tmp__scale; Data_Get_Struct(scale, LnVector3, tmp__scale);LnVector3& _scale = *tmp__scale;
            LnResult errorCode = LnUIElement_SetScale(selfObj->handle, &_scale);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (1 <= argc && argc <= 1) {
        VALUE xyz;
        rb_scan_args(argc, argv, "1", &xyz);
        if (LNRB_VALUE_IS_FLOAT(xyz))
        {
            float _xyz = LNRB_VALUE_TO_FLOAT(xyz);
            LnResult errorCode = LnUIElement_SetScaleS(selfObj->handle, _xyz);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 2) {
        VALUE x;
        VALUE y;
        rb_scan_args(argc, argv, "2", &x, &y);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            LnResult errorCode = LnUIElement_SetScaleXY(selfObj->handle, _x, _y);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::setScale - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_Scale(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnUIElement_Scale(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::scale - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_SetCenterPoint(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value))
        {
            LnVector3* tmp__value; Data_Get_Struct(value, LnVector3, tmp__value);LnVector3& _value = *tmp__value;
            LnResult errorCode = LnUIElement_SetCenterPoint(selfObj->handle, &_value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "21", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = (z != Qnil) ? LNRB_VALUE_TO_FLOAT(z) : 0.000000;
            LnResult errorCode = LnUIElement_SetCenterPointXYZ(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::setCenterPoint - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_CenterPoint(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnUIElement_CenterPoint(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::centerPoint - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_AddChild(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE child;
        rb_scan_args(argc, argv, "1", &child);
        if (LNRB_VALUE_IS_OBJECT(child))
        {
            LnHandle _child = LuminoRubyRuntimeManager::instance->getHandle(child);
            LnResult errorCode = LnUIElement_AddChild_CallOverrideBase(selfObj->handle, _child);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::addChild - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnUIElement_AddChild_OverrideCallback(LnHandle uielement, LnHandle child)
{
    VALUE obj = LuminoRubyRuntimeManager::instance->wrapObject(uielement);
    VALUE retval = rb_funcall(obj, rb_intern("add_child"), 1, LNI_TO_RUBY_VALUE(child));
    return LN_SUCCESS;
}
//==============================================================================
// ln::UIControl

struct Wrap_UIControl
    : public Wrap_UIElement
{
    Wrap_UIControl()
    {}
};

static void LnUIControl_delete(Wrap_UIControl* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnUIControl_mark(Wrap_UIControl* obj)
{
	
}

static VALUE LnUIControl_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UIControl* internalObj;

    internalObj = new Wrap_UIControl();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIControl_allocate");
    obj = Data_Wrap_Struct(klass, LnUIControl_mark, LnUIControl_delete, internalObj);

    return obj;
}

static VALUE LnUIControl_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UIControl* internalObj;

    internalObj = new Wrap_UIControl();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIControl_allocate");
    obj = Data_Wrap_Struct(klass, LnUIControl_mark, LnUIControl_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnUIControl_AddChild(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIControl* selfObj;
    Data_Get_Struct(self, Wrap_UIControl, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE child;
        rb_scan_args(argc, argv, "1", &child);
        if (LNRB_VALUE_IS_OBJECT(child))
        {
            LnHandle _child = LuminoRubyRuntimeManager::instance->getHandle(child);
            LnResult errorCode = LnUIControl_AddChild_CallOverrideBase(selfObj->handle, _child);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIControl::addChild - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnUIControl_AddChild_OverrideCallback(LnHandle uicontrol, LnHandle child)
{
    VALUE obj = LuminoRubyRuntimeManager::instance->wrapObject(uicontrol);
    VALUE retval = rb_funcall(obj, rb_intern("add_child"), 1, LNI_TO_RUBY_VALUE(child));
    return LN_SUCCESS;
}
//==============================================================================
// ln::UIButtonBase

struct Wrap_UIButtonBase
    : public Wrap_UIControl
{
    Wrap_UIButtonBase()
    {}
};

static void LnUIButtonBase_delete(Wrap_UIButtonBase* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnUIButtonBase_mark(Wrap_UIButtonBase* obj)
{
	
}

static VALUE LnUIButtonBase_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UIButtonBase* internalObj;

    internalObj = new Wrap_UIButtonBase();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIButtonBase_allocate");
    obj = Data_Wrap_Struct(klass, LnUIButtonBase_mark, LnUIButtonBase_delete, internalObj);

    return obj;
}

static VALUE LnUIButtonBase_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UIButtonBase* internalObj;

    internalObj = new Wrap_UIButtonBase();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIButtonBase_allocate");
    obj = Data_Wrap_Struct(klass, LnUIButtonBase_mark, LnUIButtonBase_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnUIButtonBase_SetText(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIButtonBase* selfObj;
    Data_Get_Struct(self, Wrap_UIButtonBase, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE text;
        rb_scan_args(argc, argv, "1", &text);
        if (LNRB_VALUE_IS_STRING(text))
        {
            const char* _text = LNRB_VALUE_TO_STRING(text);
            LnResult errorCode = LnUIButtonBase_SetTextA(selfObj->handle, _text);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIButtonBase::setText - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnUIButtonBase_AddChild_OverrideCallback(LnHandle uicontrol, LnHandle child)
{
    VALUE obj = LuminoRubyRuntimeManager::instance->wrapObject(uicontrol);
    VALUE retval = rb_funcall(obj, rb_intern("add_child"), 1, LNI_TO_RUBY_VALUE(child));
    return LN_SUCCESS;
}
//==============================================================================
// ln::UIButton

struct Wrap_UIButton
    : public Wrap_UIButtonBase
{
    VALUE connectOnClicked_Signal;
    bool connectOnClicked_EventConnect = false;
    Wrap_UIButton()
    {}
};

static void LnUIButton_delete(Wrap_UIButton* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnUIButton_mark(Wrap_UIButton* obj)
{
	rb_gc_mark(obj->connectOnClicked_Signal);

}

static VALUE LnUIButton_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UIButton* internalObj;

    internalObj = new Wrap_UIButton();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIButton_allocate");
    obj = Data_Wrap_Struct(klass, LnUIButton_mark, LnUIButton_delete, internalObj);

    return obj;
}

static VALUE LnUIButton_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UIButton* internalObj;

    internalObj = new Wrap_UIButton();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIButton_allocate");
    obj = Data_Wrap_Struct(klass, LnUIButton_mark, LnUIButton_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnUIButton_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIButton* selfObj;
    Data_Get_Struct(self, Wrap_UIButton, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnUIButton_Create(&selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIButton::init - wrong argument type.");
    return Qnil;
}

static void Wrap_LnUIButton_ConnectOnClicked_SignalCaller(LnHandle self, LnHandle e)
{
    Wrap_UIButton* selfObj;
    Data_Get_Struct(LuminoRubyRuntimeManager::instance->wrapObject(self), Wrap_UIButton, selfObj);
    rb_funcall(selfObj->connectOnClicked_Signal, rb_intern("raise"), 1, LuminoRubyRuntimeManager::instance->wrapObject(e));
}

static VALUE Wrap_LnUIButton_ConnectOnClicked(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIButton* selfObj;
    Data_Get_Struct(self, Wrap_UIButton, selfObj);
    if (!selfObj->connectOnClicked_EventConnect) {  // differed initialization.
        selfObj->connectOnClicked_Signal = rb_funcall(LuminoRubyRuntimeManager::instance->eventSignalClass(), rb_intern("new"), 0);
        LnUIButton_ConnectOnClicked(selfObj->handle, Wrap_LnUIButton_ConnectOnClicked_SignalCaller);
        selfObj->connectOnClicked_EventConnect = true;
    }

    VALUE handler, block;
    rb_scan_args(argc, argv, "01&", &handler, &block);	// def add(handler=nil, &block)
    if (handler != Qnil) rb_funcall(selfObj->connectOnClicked_Signal, rb_intern("add"), 1, handler);
    if (block != Qnil) rb_funcall(selfObj->connectOnClicked_Signal, rb_intern("add"), 1, block);
    return Qnil;
}

LnResult Wrap_LnUIButton_AddChild_OverrideCallback(LnHandle uicontrol, LnHandle child)
{
    VALUE obj = LuminoRubyRuntimeManager::instance->wrapObject(uicontrol);
    VALUE retval = rb_funcall(obj, rb_intern("add_child"), 1, LNI_TO_RUBY_VALUE(child));
    return LN_SUCCESS;
}


extern "C" void Init_Lumino()
{
    InitLuminoRubyRuntimeManager();
    g_rootModule = rb_define_module("Lumino");

    g_enum_PixelFormat = rb_define_module_under(g_rootModule, "PixelFormat");
    rb_define_const(g_enum_PixelFormat, "UNKNOWN", INT2FIX(0)); 
    rb_define_const(g_enum_PixelFormat, "A8", INT2FIX(1)); 
    rb_define_const(g_enum_PixelFormat, "RGBA8", INT2FIX(2)); 
    rb_define_const(g_enum_PixelFormat, "RGB8", INT2FIX(3)); 
    rb_define_const(g_enum_PixelFormat, "RGBA32F", INT2FIX(4)); 

    g_enum_TextureFormat = rb_define_module_under(g_rootModule, "TextureFormat");
    rb_define_const(g_enum_TextureFormat, "UNKNOWN", INT2FIX(0)); 
    rb_define_const(g_enum_TextureFormat, "RGBA8", INT2FIX(1)); 
    rb_define_const(g_enum_TextureFormat, "RGB8", INT2FIX(2)); 
    rb_define_const(g_enum_TextureFormat, "RGBA16F", INT2FIX(3)); 
    rb_define_const(g_enum_TextureFormat, "RGBA32F", INT2FIX(4)); 
    rb_define_const(g_enum_TextureFormat, "R16F", INT2FIX(5)); 
    rb_define_const(g_enum_TextureFormat, "R32F", INT2FIX(6)); 
    rb_define_const(g_enum_TextureFormat, "R32U", INT2FIX(7)); 

    g_enum_DepthBufferFormat = rb_define_module_under(g_rootModule, "DepthBufferFormat");
    rb_define_const(g_enum_DepthBufferFormat, "D24S8", INT2FIX(0));

    g_class_Vector3 = rb_define_class_under(g_rootModule, "Vector3", rb_cObject);
    rb_define_alloc_func(g_class_Vector3, LnVector3_allocate);

    g_class_Quaternion = rb_define_class_under(g_rootModule, "Quaternion", rb_cObject);
    rb_define_alloc_func(g_class_Quaternion, LnQuaternion_allocate);

    g_class_Engine = rb_define_class_under(g_rootModule, "Engine", rb_cObject);
    rb_define_singleton_method(g_class_Engine, "initialize", LN_TO_RUBY_FUNC(Wrap_LnEngine_Initialize), -1);
    rb_define_singleton_method(g_class_Engine, "finalize", LN_TO_RUBY_FUNC(Wrap_LnEngine_Finalize), -1);
    rb_define_singleton_method(g_class_Engine, "update", LN_TO_RUBY_FUNC(Wrap_LnEngine_Update), -1);
    rb_define_singleton_method(g_class_Engine, "main_ui_view", LN_TO_RUBY_FUNC(Wrap_LnEngine_MainUIView), -1);

    g_class_GraphicsResource = rb_define_class_under(g_rootModule, "GraphicsResource", rb_cObject);
    rb_define_alloc_func(g_class_GraphicsResource, LnGraphicsResource_allocate);
    LnGraphicsResource_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_GraphicsResource, LnGraphicsResource_allocateForGetObject));

    g_class_Texture = rb_define_class_under(g_rootModule, "Texture", g_class_GraphicsResource);
    rb_define_alloc_func(g_class_Texture, LnTexture_allocate);
    LnTexture_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Texture, LnTexture_allocateForGetObject));

    g_class_Texture2D = rb_define_class_under(g_rootModule, "Texture2D", g_class_Texture);
    rb_define_alloc_func(g_class_Texture2D, LnTexture2D_allocate);
    rb_define_private_method(g_class_Texture2D, "initialize", LN_TO_RUBY_FUNC(Wrap_LnTexture2D_Create), -1);
    LnTexture2D_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Texture2D, LnTexture2D_allocateForGetObject));

    g_class_WorldObject = rb_define_class_under(g_rootModule, "WorldObject", rb_cObject);
    rb_define_alloc_func(g_class_WorldObject, LnWorldObject_allocate);
    rb_define_method(g_class_WorldObject, "set_position", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_SetPosition), -1);
    rb_define_method(g_class_WorldObject, "position", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Position), -1);
    rb_define_method(g_class_WorldObject, "set_rotation", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_SetRotation), -1);
    rb_define_method(g_class_WorldObject, "set_euler_angles", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_SetEulerAngles), -1);
    rb_define_method(g_class_WorldObject, "rotation", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Rotation), -1);
    rb_define_method(g_class_WorldObject, "set_scale", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_SetScale), -1);
    rb_define_method(g_class_WorldObject, "scale", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Scale), -1);
    rb_define_method(g_class_WorldObject, "set_center_point", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_SetCenterPoint), -1);
    rb_define_method(g_class_WorldObject, "center_point", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_CenterPoint), -1);
    rb_define_method(g_class_WorldObject, "on_update", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_OnUpdate), -1);
    LnWorldObject_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_WorldObject, LnWorldObject_allocateForGetObject));
    LnWorldObject_OnUpdate_SetOverrideCallback(Wrap_LnWorldObject_OnUpdate_OverrideCallback);

    g_class_VisualObject = rb_define_class_under(g_rootModule, "VisualObject", g_class_WorldObject);
    rb_define_alloc_func(g_class_VisualObject, LnVisualObject_allocate);
    rb_define_method(g_class_VisualObject, "set_visible", LN_TO_RUBY_FUNC(Wrap_LnVisualObject_SetVisible), -1);
    rb_define_method(g_class_VisualObject, "is_visible", LN_TO_RUBY_FUNC(Wrap_LnVisualObject_IsVisible), -1);
    LnVisualObject_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_VisualObject, LnVisualObject_allocateForGetObject));
    LnVisualObject_OnUpdate_SetOverrideCallback(Wrap_LnVisualObject_OnUpdate_OverrideCallback);

    g_class_Sprite = rb_define_class_under(g_rootModule, "Sprite", g_class_VisualObject);
    rb_define_alloc_func(g_class_Sprite, LnSprite_allocate);
    rb_define_method(g_class_Sprite, "set_texture", LN_TO_RUBY_FUNC(Wrap_LnSprite_SetTexture), -1);
    rb_define_method(g_class_Sprite, "set_source_rect", LN_TO_RUBY_FUNC(Wrap_LnSprite_SetSourceRectXYWH), -1);
    rb_define_private_method(g_class_Sprite, "initialize", LN_TO_RUBY_FUNC(Wrap_LnSprite_Create), -1);
    LnSprite_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Sprite, LnSprite_allocateForGetObject));
    LnSprite_OnUpdate_SetOverrideCallback(Wrap_LnSprite_OnUpdate_OverrideCallback);

    g_class_UIEventArgs = rb_define_class_under(g_rootModule, "UIEventArgs", rb_cObject);
    rb_define_alloc_func(g_class_UIEventArgs, LnUIEventArgs_allocate);
    rb_define_method(g_class_UIEventArgs, "sender", LN_TO_RUBY_FUNC(Wrap_LnUIEventArgs_Sender), -1);
    LnUIEventArgs_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UIEventArgs, LnUIEventArgs_allocateForGetObject));

    g_class_UILayoutElement = rb_define_class_under(g_rootModule, "UILayoutElement", rb_cObject);
    rb_define_alloc_func(g_class_UILayoutElement, LnUILayoutElement_allocate);
    LnUILayoutElement_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UILayoutElement, LnUILayoutElement_allocateForGetObject));

    g_class_UIElement = rb_define_class_under(g_rootModule, "UIElement", g_class_UILayoutElement);
    rb_define_alloc_func(g_class_UIElement, LnUIElement_allocate);
    rb_define_method(g_class_UIElement, "set_position", LN_TO_RUBY_FUNC(Wrap_LnUIElement_SetPosition), -1);
    rb_define_method(g_class_UIElement, "position", LN_TO_RUBY_FUNC(Wrap_LnUIElement_Position), -1);
    rb_define_method(g_class_UIElement, "set_rotation", LN_TO_RUBY_FUNC(Wrap_LnUIElement_SetRotation), -1);
    rb_define_method(g_class_UIElement, "set_euler_angles", LN_TO_RUBY_FUNC(Wrap_LnUIElement_SetEulerAngles), -1);
    rb_define_method(g_class_UIElement, "rotation", LN_TO_RUBY_FUNC(Wrap_LnUIElement_Rotation), -1);
    rb_define_method(g_class_UIElement, "set_scale", LN_TO_RUBY_FUNC(Wrap_LnUIElement_SetScale), -1);
    rb_define_method(g_class_UIElement, "scale", LN_TO_RUBY_FUNC(Wrap_LnUIElement_Scale), -1);
    rb_define_method(g_class_UIElement, "set_center_point", LN_TO_RUBY_FUNC(Wrap_LnUIElement_SetCenterPoint), -1);
    rb_define_method(g_class_UIElement, "center_point", LN_TO_RUBY_FUNC(Wrap_LnUIElement_CenterPoint), -1);
    rb_define_method(g_class_UIElement, "add_child", LN_TO_RUBY_FUNC(Wrap_LnUIElement_AddChild), -1);
    LnUIElement_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UIElement, LnUIElement_allocateForGetObject));
    LnUIElement_AddChild_SetOverrideCallback(Wrap_LnUIElement_AddChild_OverrideCallback);

    g_class_UIControl = rb_define_class_under(g_rootModule, "UIControl", g_class_UIElement);
    rb_define_alloc_func(g_class_UIControl, LnUIControl_allocate);
    rb_define_method(g_class_UIControl, "add_child", LN_TO_RUBY_FUNC(Wrap_LnUIControl_AddChild), -1);
    LnUIControl_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UIControl, LnUIControl_allocateForGetObject));
    LnUIControl_AddChild_SetOverrideCallback(Wrap_LnUIControl_AddChild_OverrideCallback);

    g_class_UIButtonBase = rb_define_class_under(g_rootModule, "UIButtonBase", g_class_UIControl);
    rb_define_alloc_func(g_class_UIButtonBase, LnUIButtonBase_allocate);
    rb_define_method(g_class_UIButtonBase, "set_text", LN_TO_RUBY_FUNC(Wrap_LnUIButtonBase_SetText), -1);
    LnUIButtonBase_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UIButtonBase, LnUIButtonBase_allocateForGetObject));
    LnUIButtonBase_AddChild_SetOverrideCallback(Wrap_LnUIButtonBase_AddChild_OverrideCallback);

    g_class_UIButton = rb_define_class_under(g_rootModule, "UIButton", g_class_UIButtonBase);
    rb_define_alloc_func(g_class_UIButton, LnUIButton_allocate);
    rb_define_private_method(g_class_UIButton, "initialize", LN_TO_RUBY_FUNC(Wrap_LnUIButton_Create), -1);
    rb_define_method(g_class_UIButton, "connect_on_clicked", LN_TO_RUBY_FUNC(Wrap_LnUIButton_ConnectOnClicked), -1);
    LnUIButton_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UIButton, LnUIButton_allocateForGetObject));
    LnUIButton_AddChild_SetOverrideCallback(Wrap_LnUIButton_AddChild_OverrideCallback);

}


#if 0
#include "LuminoRuby.h"
#include "RubyStructs.h"

//-----------------------------------------------------------------------------
// WrapStructs


__WRAP_STRUCTS__

__GLOBALS__

__FUNCTIONS__

void InitClasses()
{
__DEFINES__
}

//-----------------------------------------------------------------------------
// TypeInfo

void Manager::RegisterTypeInfo()
{
	// dummy
	TypeInfo t;
	t.klass = Qnil;
	t.factory = NULL;
	m_typeInfoList.push_back(t);
	
__TYPEINFO_REGISTERS__
}
#endif
