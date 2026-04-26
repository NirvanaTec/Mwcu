#include "library.h"

#include <cstring>

#include "tools/javaTools.h"
#include "tools/socketTools.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>

// authlib-1.5.25.jar
// public native int AuthenticationAccessToken(int var1, String var2);
JNIEXPORT jint JNICALL Java_com_mojang_authlib_AuthenticationCpp_AuthenticationAccessToken(
    JNIEnv *env, jclass, jint port, jstring server_id) {
    // 检查端口范围
    if (port <= 0 || port > 65535) {
        env->ThrowNew(env->FindClass("java/lang/IllegalArgumentException"), "端口号必须在1-65535之间");
        return 2;
    }

    // 从Java系统属性动态获取游戏ID和用户ID
    const auto gameId = javaTools::getSystemProperty(env, "launcherGameId");
    const auto userId = javaTools::getSystemProperty(env, "userId");

    // 如果获取失败，抛出JNI异常
    if (gameId.empty()) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "无法获取launcherGameId");
        return 2;
    }

    if (userId.empty()) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "无法获取userId");
        return 2;
    }

    TcpClient client;

    // 连接到本地TCP服务器（127.0.0.1）
    const auto serverPort = static_cast<int>(port);
    std::cout << "正在连接到本地TCP服务器[" << serverPort << "]..." << std::endl;
    if (!client.connectToServer("127.0.0.1", serverPort)) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "无法连接到本地TCP服务器");
        return 2;
    }

    jint responseCode = 2; // 默认错误代码

    // 发送游戏ID（先发送长度，再发送内容）
    const auto gameIdLength = static_cast<uint32_t>(gameId.length());
    std::cout << "游戏ID[" << gameIdLength << "]: " << gameId << std::endl;
    if (!client.sendData(&gameIdLength, sizeof(gameIdLength))) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "发送游戏ID长度失败");
        client.disconnect();
        return 2;
    }
    if (!client.sendData(gameId.c_str(), gameIdLength)) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "发送游戏ID内容失败");
        client.disconnect();
        return 2;
    }

    // 发送用户ID（先发送长度，再发送内容）
    const auto userIdLength = static_cast<uint32_t>(userId.length());
    std::cout << "用户ID[" << userIdLength << "]: " << userId << std::endl;
    if (!client.sendData(&userIdLength, sizeof(userIdLength))) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "发送用户ID长度失败");
        client.disconnect();
        return 2;
    }
    if (!client.sendData(userId.c_str(), userIdLength)) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "发送用户ID内容失败");
        client.disconnect();
        return 2;
    }

    // 发送服务器ID（先发送长度，再发送内容）
    if (server_id == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/IllegalArgumentException"), "server_id 不能为null");
        return 2;
    }
    const auto serverIdChars = env->GetStringUTFChars(server_id, nullptr);
    if (serverIdChars == nullptr) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "无法获取服务器ID字符串");
        return 2;
    }
    const auto serverIdLength = static_cast<uint32_t>(strlen(serverIdChars));
    std::cout << "服务器ID[" << serverIdLength << "]: " << serverIdChars << std::endl;
    if (!client.sendData(&serverIdLength, sizeof(serverIdLength))) {
        env->ReleaseStringUTFChars(server_id, serverIdChars);
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "发送服务器ID长度失败");
        client.disconnect();
        return 2;
    }
    if (!client.sendData(serverIdChars, serverIdLength)) {
        env->ReleaseStringUTFChars(server_id, serverIdChars);
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "发送服务器ID内容失败");
        client.disconnect();
        return 2;
    }
    env->ReleaseStringUTFChars(server_id, serverIdChars);

    // 接收响应代码
    if (!client.receiveData(&responseCode, sizeof(responseCode))) {
        env->ThrowNew(env->FindClass("java/lang/RuntimeException"), "接收响应代码失败");
        client.disconnect();
        return 2;
    }

    client.disconnect();

    return responseCode;
}

// modlauncher-9.1.0.jar
// public static native boolean init();
JNIEXPORT jboolean JNICALL Java_com_netease_mc_mod_network_common_Library_init(JNIEnv *, jclass) {
    return JNI_TRUE;
}

// modlauncher-9.1.0.jar
// public static native void log(String var0);
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_log(JNIEnv *env, jclass, jstring message) {
    if (message == nullptr) {
        return;
    }
    const auto message_cstr = env->GetStringUTFChars(message, nullptr);
    if (!message_cstr) {
        return; // 字符串转换失败
    }
    std::cout << "日志: " << message_cstr << std::endl;
    env->ReleaseStringUTFChars(message, message_cstr);
}

// modlauncher-9.1.0.jar
// public static native void preInit();
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_preInit(JNIEnv *, jclass) {
}

// modlauncher-9.1.0.jar
// public static native void GetToken(byte[] var0, int var1);
// cpw.mods.modlauncher@9.1.8/com.netease.mc.mod.network.common.Encrption.<init>
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_GetToken(
    JNIEnv *env, jclass, jbyteArray tokenArray, jint length) {
    // 检查参数
    if (tokenArray == nullptr || length <= 0) {
        return;
    }

    // 获取token字符串
    const auto token = javaTools::getSystemProperty(env, "Token");

    // 将token转换为字节数组并复制到Java数组
    if (const auto buffer = env->GetByteArrayElements(tokenArray, nullptr); buffer != nullptr) {
        const auto copyLength = std::min(static_cast<size_t>(length), token.length());
        memcpy(buffer, token.c_str(), copyLength);
        // JNI_COMMIT: 复制内容并释放缓冲区
        env->ReleaseByteArrayElements(tokenArray, buffer, JNI_COMMIT);
    }
}

// modlauncher-9.1.0.jar
// public static native void ChaChaProcess(long var0, byte[] var2, int var3);
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_ChaChaProcess(
    JNIEnv *, jclass, jlong, jbyteArray, jint) {
}

// modlauncher-9.1.0.jar
// public native byte[] DecryptClassBytes(byte[] var1);
JNIEXPORT jbyteArray JNICALL Java_com_netease_mc_mod_network_common_Library_DecryptClassBytes(
    JNIEnv *env, jclass, jbyteArray var1) {
    // 目前只是返回原数组的副本
    if (var1 == nullptr) {
        return nullptr;
    }

    const auto length = env->GetArrayLength(var1);
    if (length <= 0) {
        return env->NewByteArray(0);
    }

    const auto result = env->NewByteArray(length);
    if (result == nullptr) {
        return nullptr;
    }

    auto *src = env->GetByteArrayElements(var1, nullptr);
    if (src == nullptr) {
        // 注意：这里不需要删除 result，因为 JVM 会自动处理
        // 但返回 nullptr 表示失败
        return nullptr;
    }

    env->SetByteArrayRegion(result, 0, length, src);
    env->ReleaseByteArrayElements(var1, src, JNI_ABORT);

    return result;
}

// modlauncher-9.1.0.jar
// public static native int AuthenticationAccessToken(int var0, String var1);
JNIEXPORT jint JNICALL Java_com_netease_mc_mod_network_common_Library_AuthenticationAccessToken(
    JNIEnv *env, jclass clazz, jint port, jstring server_id) {
    return Java_com_mojang_authlib_AuthenticationCpp_AuthenticationAccessToken(env, clazz, port, server_id);
}

// modlauncher-9.1.0.jar
// public static native void close();
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_close(JNIEnv *, jclass) {
}

// modlauncher-9.1.0.jar
// public static native void DeleteChaCha(long var0);
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_DeleteChaCha(
    JNIEnv *, jclass, jlong) {
}

// modlauncher-9.1.0.jar
// public static native int Skip32(boolean var0, byte[] var1, int var2);
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_Skip32(
    JNIEnv *, jclass, jboolean, jbyteArray, jint) {
}


// modlauncher-9.1.0.jar
// public static native long NewChaCha(int var0, byte[] var1);
// cpw.mods.modlauncher@9.1.8/com.netease.mc.mod.network.common.Encrption.<init>
JNIEXPORT jlong JNICALL Java_com_netease_mc_mod_network_common_Library_NewChaCha(
    JNIEnv *, jclass, jint, jbyteArray) {
    return 0;
}

// modlauncher-9.1.0.jar
// public static native int reviewName(String var0);
JNIEXPORT jint JNICALL Java_com_netease_mc_mod_network_common_Library_reviewName(JNIEnv *, jclass, jstring) {
    return 0;
}

// modlauncher-9.1.0.jar
// public static native int reviewWord(String var0);
JNIEXPORT jint JNICALL Java_com_netease_mc_mod_network_common_Library_reviewWord(JNIEnv *, jclass, jstring) {
    return 0;
}

// modlauncher-9.1.0.jar
// public static native void test();
JNIEXPORT void JNICALL Java_com_netease_mc_mod_network_common_Library_test(JNIEnv *, jclass) {
}
