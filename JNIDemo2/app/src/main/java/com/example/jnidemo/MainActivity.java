package com.example.jnidemo;

import android.os.Debug;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private SecMan shield;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        shield = new SecMan();

        TextView tvBadge   = findViewById(R.id.tvBadge);
        TextView tvStatus  = findViewById(R.id.tvStatus);
        TextView tvSig     = findViewById(R.id.tvSig);
        TextView tvCompute = findViewById(R.id.tvCompute);
        View     cardSig   = findViewById(R.id.cardSig);
        View     cardComp  = findViewById(R.id.cardComp);

        // Lance la vérification d'intégrité via le module natif
        boolean compromised = shield.runIntegrityCheck();

        // Dans MainActivity.java
        if (Debug.isDebuggerConnected()) {
            Log.e("SHIELD_CORE", "Debugger Java détecté");
            compromised = true;
        }
        // Ajout détection Java (fiable)
        if (Debug.isDebuggerConnected()) {
            compromised = true;
        }

        if (compromised) {
            // Environnement suspect — on bloque les fonctions sensibles
            tvBadge.setText("⚠");
            tvBadge.setBackgroundResource(R.drawable.badge_alerte);
            tvStatus.setText("ENVIRONNEMENT COMPROMIS\nFonctions natives désactivées");
            tvStatus.setTextColor(getColor(R.color.alert));

            cardSig.setAlpha(0.35f);
            cardComp.setAlpha(0.35f);
            tvSig.setText("— accès refusé —");
            tvCompute.setText("— accès refusé —");

        } else {
            // Tout est propre — on affiche les résultats natifs
            tvBadge.setText("✓");
            tvBadge.setBackgroundResource(R.drawable.badge_ok);
            tvStatus.setText("INTÉGRITÉ VÉRIFIÉE\nModule natif opérationnel");
            tvStatus.setTextColor(getColor(R.color.ok));

            tvSig.setText(shield.getNativeSignature());

            long power = shield.computePower(2, 16);
            tvCompute.setText("2¹⁶  =  " + power);
        }
    }
}