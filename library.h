#ifndef MWCU_LIBRARY_H
#define MWCU_LIBRARY_H

#ifdef _WIN32
#include "include/win/jni/jni.h"
#include "include/win/jni/win32/jni_md.h"
#else
#include "include/linux/jni/jni.h"
#include "include/linux/jni/linux/jni_md.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// authlib-1.5.25.jar
// public native int AuthenticationAccessToken(int var1, String var2);
JNIEXPORT jint JNICALL Java_com_mojang_authlib_AuthenticationCpp_AuthenticationAccessToken(
    JNIEnv *env, jclass clazz, jint port, jstring server_id);

// modlauncher-9.1.0.jar
// public static native boolean init();
JNIEXPORT jboolean JNICALL Java_com_netease_mc_mod_network_common_Library_init(JNIEnv *env, jclass clazz);

// modlauncher-9.1.0.jar
// public static native void log(String var0);
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_log(JNIEnv *env, jclass clazz, jstring message);

// modlauncher-9.1.0.jar
// public static native void preInit();
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_preInit(JNIEnv *env, jclass clazz);

// modlauncher-9.1.0.jar
// public static native void GetToken(byte[] var0, int var1);
// cpw.mods.modlauncher@9.1.8/com.netease.mc.mod.network.common.Encrption.<init>
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_GetToken(
    JNIEnv *env, jclass clazz, jbyteArray tokenArray, jint length);

// modlauncher-9.1.0.jar
// public static native void ChaChaProcess(long var0, byte[] var2, int var3);
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_ChaChaProcess(
    JNIEnv *env, jclass clazz, jlong var0, jbyteArray var2, jint var3);

// modlauncher-9.1.0.jar
// public native byte[] DecryptClassBytes(byte[] var1);
JNIEXPORT jbyteArray JNICALL Java_com_netease_mc_mod_network_common_Library_DecryptClassBytes(
    JNIEnv *env, jclass clazz, jbyteArray var1);

// modlauncher-9.1.0.jar
// public static native int AuthenticationAccessToken(int var0, String var1);
JNIEXPORT jint JNICALL Java_com_netease_mc_mod_network_common_Library_AuthenticationAccessToken(
    JNIEnv *env, jclass clazz, jint port, jstring server_id);

// modlauncher-9.1.0.jar
// public static native void close();
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_close(JNIEnv *env, jclass clazz);

// modlauncher-9.1.0.jar
// public static native void DeleteChaCha(long var0);
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_DeleteChaCha(
    JNIEnv *env, jclass clazz, jlong var0);

// modlauncher-9.1.0.jar
// public static native int Skip32(boolean var0, byte[] var1, int var2);
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_Skip32(
    JNIEnv *env, jclass clazz, jboolean var0, jbyteArray var1, jint var2);

// modlauncher-9.1.0.jar
// public static native long NewChaCha(int var0, byte[] var1);
// cpw.mods.modlauncher@9.1.8/com.netease.mc.mod.network.common.Encrption.<init>
JNIEXPORT jlong JNICALL Java_com_netease_mc_mod_network_common_Library_NewChaCha(
    JNIEnv *env, jclass clazz, jint var0, jbyteArray var1);

// modlauncher-9.1.0.jar
// public static native int reviewName(String var0);
JNIEXPORT jint JNICALL Java_com_netease_mc_mod_network_common_Library_reviewName(
    JNIEnv *env, jclass clazz, jstring var0);

// modlauncher-9.1.0.jar
// public static native int reviewWord(String var0);
JNIEXPORT jint JNICALL Java_com_netease_mc_mod_network_common_Library_reviewWord(
    JNIEnv *env, jclass clazz, jstring var0);

// modlauncher-9.1.0.jar
// public static native void test();
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_test(JNIEnv *env, jclass clazz);


#ifdef __cplusplus
}
#endif

#endif // MWCU_LIBRARY_H
