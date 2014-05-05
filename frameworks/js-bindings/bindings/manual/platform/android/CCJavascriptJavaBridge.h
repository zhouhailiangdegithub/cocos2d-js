#ifndef __CC_JAVASCRIPT_JAVA_BRIDGE__
#define __CC_JAVASCRIPT_JAVA_BRIDGE__

#include <jni.h>
#include <string>
#include <vector>
#include "jsapi.h"
#include "jsfriendapi.h"
#include "js_bindings_config.h"
#include "jsb_helper.h"

using namespace std;

#define JSJ_ERR_OK                 (0)
#define JSJ_ERR_TYPE_NOT_SUPPORT   (-1)
#define JSJ_ERR_INVALID_SIGNATURES (-2)
#define JSJ_ERR_METHOD_NOT_FOUND   (-3)
#define JSJ_ERR_EXCEPTION_OCCURRED (-4)
#define JSJ_ERR_VM_THREAD_DETACHED (-5)
#define JSJ_ERR_VM_FAILURE         (-6)

#define  LOG_TAG    "CCJavascriptJavaBridge"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

class JavascriptJavaBridge
{

private:
    typedef enum
    {
        TypeInvalid = -1,
        TypeVoid    = 0,
        TypeInteger = 1,
        TypeFloat   = 2,
        TypeBoolean = 3,
        TypeString  = 4,
        TypeVector  = 5,
        TypeFunction= 6,
    } ValueType;

    typedef vector<ValueType> ValueTypes;

    typedef union
    {
        int     intValue;
        float   floatValue;
        int     boolValue;
        string *stringValue;
    } ReturnValue;

    class CallInfo
    {
    public:
        CallInfo(const char *className, const char *methodName, const char *methodSig)
        : m_valid(false)
        , m_error(JSJ_ERR_OK)
        , m_className(className)
        , m_methodName(methodName)
        , m_methodSig(methodSig)
        , m_returnType(TypeVoid)
        , m_argumentsCount(0)
        , m_retjstring(NULL)
        , m_env(NULL)
        , m_classID(NULL)
        , m_methodID(NULL)
        {
            memset(&m_ret, 0, sizeof(m_ret));
            m_valid =  validateMethodSig() && getMethodInfo();
        }
        ~CallInfo(void);

        bool isValid(void) {
            return m_valid;
        }

        int getErrorCode(void) {
            return m_error;
        }

        JNIEnv *getEnv(void) {
            return m_env;
        }

        int argumentTypeAtIndex(size_t index) {
            return m_argumentsType.at(index);
        }

        int getArgumentsCount(){
            return m_argumentsCount;
        }

        bool execute(void);
        bool executeWithArgs(jvalue *args);


    private:
        bool        m_valid;
        int         m_error;

        string      m_className;
        string      m_methodName;
        string      m_methodSig;
        int         m_argumentsCount;
        ValueTypes  m_argumentsType;
        ValueType   m_returnType;

        ReturnValue m_ret;
        jstring     m_retjstring;

        JNIEnv     *m_env;
        jclass      m_classID;
        jmethodID   m_methodID;

        bool validateMethodSig(void);
        bool getMethodInfo(void);
        ValueType checkType(const string& sig, size_t *pos);
    };


    bool callJavaStaticMethod(const char* className, const char* methodName, const char* signature, jvalue *args);
    bool callJavaStaticMethod(CallInfo &call, jvalue *args);

public:
    
    JS_BINDED_CLASS_GLUE(JavascriptJavaBridge);
    JS_BINDED_CONSTRUCTOR(JavascriptJavaBridge);
    JS_BINDED_FUNC(JavascriptJavaBridge, callStaticMethod);

};

#endif