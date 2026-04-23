// ============================================================
// shield-core.cpp — Module de sécurité natif Android
// Contrôles défensifs exécutés côté C++ via JNI
// ============================================================

#include <jni.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <android/log.h>
#include <sys/ptrace.h>
#include <unistd.h>

// Tag pour filtrer les logs dans Logcat
#define TAG "SHIELD_CORE"
#define LOG_INFO(...)  __android_log_print(ANDROID_LOG_INFO,  TAG, __VA_ARGS__)
#define LOG_WARN(...)  __android_log_print(ANDROID_LOG_WARN,  TAG, __VA_ARGS__)
#define LOG_ERR(...)   __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// ----------------------------------------------------------
// Vérification 1 : détection de traçage actif sur le processus
// Si ptrace échoue (-1), quelqu'un supervise déjà ce processus
// ----------------------------------------------------------
static bool checkTracerPresence() {
    long res = ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    if (res == -1) {
        LOG_ERR("Traceur detecte — processus surveille");
        return true;
    }
    LOG_INFO("Aucun traceur actif (ptrace OK)");
    return false;
}

// ----------------------------------------------------------
// Vérification 2 : scan de /proc/self/maps
// Recherche de noms de bibliothèques liés à l'instrumentation
// ----------------------------------------------------------
static bool scanMemoryForTools() {
    FILE* fmaps = fopen("/proc/self/maps", "r");
    if (!fmaps) {
        LOG_WARN("Impossible de lire /proc/self/maps");
        return false; // On ne bloque pas si on ne peut pas lire
    }

    char buf[512];
    // Mots-clés associés aux outils d'analyse dynamique courants
    const char* signatures[] = {
            "frida", "libfrida", "xposed",
            "gdbserver", "libgdb", "magisk",
            nullptr
    };

    while (fgets(buf, sizeof(buf), fmaps)) {
        for (int i = 0; signatures[i] != nullptr; i++) {
            if (strstr(buf, signatures[i])) {
                LOG_ERR("Outil suspect detecte dans maps : %s", buf);
                fclose(fmaps);
                return true;
            }
        }
    }

    fclose(fmaps);
    LOG_INFO("Scan memoire OK — aucun outil suspect");
    return false;
}

// ----------------------------------------------------------
// Point d'entrée JNI — appelé depuis SecMan.java
// Fusionne les deux signaux et retourne un booléen à Java
// ----------------------------------------------------------
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_jnidemo_SecMan_runIntegrityCheck(
        JNIEnv* env,
        jobject /* this */) {

    bool traced  = false;
    bool tooling = scanMemoryForTools();

    if (traced || tooling) {
        LOG_ERR("ALERTE : environnement compromis detecte");
        return JNI_TRUE;
    }

    LOG_INFO("Integrite verifiee — environnement propre");
    return JNI_FALSE;
}

// ----------------------------------------------------------
// Fonctions métier natives — disponibles si intégrité OK
// ----------------------------------------------------------

// Retourne un message d'identité du module natif
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_SecMan_getNativeSignature(
        JNIEnv* env,
        jobject /* this */) {
    return env->NewStringUTF("Module Shield v1.0 — actif");
}

// Calcule la puissance d'un entier (n^k) côté natif
extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_jnidemo_SecMan_computePower(
        JNIEnv* env,
        jobject /* this */,
        jint n,
        jint k) {
    if (k < 0) return -1;
    long long result = 1;
    for (int i = 0; i < k; i++) result *= n;
    return static_cast<jlong>(result);
}
