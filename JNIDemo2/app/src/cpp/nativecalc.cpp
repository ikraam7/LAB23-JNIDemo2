#include <jni.h>
#include <string>
#include <algorithm>
#include <climits>
#include <android/log.h>

// Macros pour les logs Android (tag visible dans Logcat)
#define TAG "NATIVECALC"
#define LOG_I(...) __android_log_print(ANDROID_LOG_INFO,  TAG, __VA_ARGS__)
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// ── 1. Message de bienvenue ──────────────────────────────────────────────────
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_nativecalc_MainActivity_greetNative(
        JNIEnv* env,
        jobject /* thiz */) {

    LOG_I("greetNative appelé");
    return env->NewStringUTF("Bonjour depuis le C++ natif !");
}

// ── 2. Factorielle avec protection overflow ──────────────────────────────────
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_nativecalc_MainActivity_computeFactorial(
        JNIEnv* env,
        jobject /* thiz */,
        jint n) {

    if (n < 0) {
        LOG_E("computeFactorial: valeur négative (%d)", n);
        return -1; // code erreur : entrée invalide
    }

    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
        if (result > INT_MAX) {
            LOG_E("computeFactorial: dépassement pour n=%d", n);
            return -2; // code erreur : overflow
        }
    }

    LOG_I("computeFactorial(%d) = %lld", n, result);
    return static_cast<jint>(result);
}

// ── 3. Inversion de chaîne Java → C++ → Java ────────────────────────────────
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_nativecalc_MainActivity_flipString(
        JNIEnv* env,
        jobject /* thiz */,
        jstring input) {

    if (input == nullptr) {
        LOG_E("flipString: chaîne nulle");
        return env->NewStringUTF("[erreur: null]");
    }

    // Récupère le pointeur vers les octets UTF-8 de la String Java
    const char* raw = env->GetStringUTFChars(input, nullptr);
    if (raw == nullptr) return env->NewStringUTF("[erreur JNI]");

    std::string s(raw);
    env->ReleaseStringUTFChars(input, raw); // libère la mémoire native

    std::reverse(s.begin(), s.end());

    LOG_I("flipString → %s", s.c_str());
    return env->NewStringUTF(s.c_str());
}

// ── 4. Somme d'un tableau int[] ──────────────────────────────────────────────
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_nativecalc_MainActivity_arrayTotal(
        JNIEnv* env,
        jobject /* thiz */,
        jintArray data) {

    if (data == nullptr) {
        LOG_E("arrayTotal: tableau nul");
        return -1;
    }

    jsize len      = env->GetArrayLength(data);
    jint* elems    = env->GetIntArrayElements(data, nullptr);

    if (elems == nullptr) {
        LOG_E("arrayTotal: impossible de lire le tableau");
        return -2;
    }

    long long total = 0;
    for (jsize i = 0; i < len; i++) total += elems[i];

    // 0 = copie les modifications et libère
    env->ReleaseIntArrayElements(data, elems, 0);

    if (total > INT_MAX) {
        LOG_E("arrayTotal: overflow de la somme");
        return -3;
    }

    LOG_I("arrayTotal = %lld", total);
    return static_cast<jint>(total);
}//
// Created by IKRAM on 22/04/2026.
//
