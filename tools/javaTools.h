#ifndef MWCU_JAVATOOLS_H
#define MWCU_JAVATOOLS_H

#include <string>

#ifdef _WIN32
#include "../include/win/jni/jni.h"
#else
#include "../include/linux/jni/jni.h"
#endif

class javaTools {
public:
    static std::string getSystemProperty(JNIEnv *env, const char *propertyName);
};

#endif
