package com.example.jnidemo;

/**
 * Pont JNI vers le module natif shield-core.
 * Toutes les méthodes natives sont regroupées ici.
 */
public class SecMan {

    // Chargement de la bibliothèque native au démarrage
    static {
        System.loadLibrary("shield-core");
    }

    // Retourne true si un environnement suspect est détecté
    public native boolean runIntegrityCheck();

    // Retourne la signature du module natif
    public native String getNativeSignature();

    // Calcule n^k côté C++ et retourne le résultat
    public native long computePower(int n, int k);
}
