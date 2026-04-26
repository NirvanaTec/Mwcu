#include "javaTools.h"

std::string javaTools::getSystemProperty(JNIEnv *env, const char *propertyName) {
    // 获取System类
    const auto systemClass = env->FindClass("java/lang/System");
    if (!systemClass) {
        env->ExceptionClear(); // 清除FindClass设置的异常
        return "";
    }

    // 获取getProperty方法
    const auto getPropertyMethod = env->GetStaticMethodID(
        systemClass, "getProperty", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!getPropertyMethod) {
        env->ExceptionClear(); // 清除GetStaticMethodID设置的异常
        env->DeleteLocalRef(systemClass);
        return "";
    }

    // 创建属性名称字符串
    const auto propertyNameStr = env->NewStringUTF(propertyName);
    if (!propertyNameStr) {
        env->DeleteLocalRef(systemClass);
        return "";
    }

    // 调用getProperty方法
    const auto resultObj = env->CallStaticObjectMethod(
        systemClass, getPropertyMethod, propertyNameStr);
    const auto resultStr = reinterpret_cast<jstring>(resultObj);

    std::string result;

    if (resultStr) {
        if (const auto resultCStr = env->GetStringUTFChars(resultStr, nullptr)) {
            result = std::string(resultCStr);
            env->ReleaseStringUTFChars(resultStr, resultCStr);
        }
        env->DeleteLocalRef(resultStr);
    }

    // 清理本地引用
    env->DeleteLocalRef(propertyNameStr);
    env->DeleteLocalRef(systemClass);

    return result;
}
