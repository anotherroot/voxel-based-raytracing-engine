package com.example.test_android;


import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;

import org.libsdl.app.SDLActivity;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends SDLActivity {
    static {
        System.loadLibrary("test_android");
    }
    /* A fancy way of getting the class name */
//    private static final String TAG = MainActivity.class.getSimpleName();
//
//    private static final String[] ASSET_NAMES = new String[]{
//            "test.png"
//    };
//
//
//    @Override
//    protected String[] getArguments() {
//        return new String[]{getFilesDir().getAbsolutePath()};
//    }
//
//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//
//        Log.v(TAG, "Copying assets to accessible locations");
//        AssetManager assetManager = this.getAssets();
//        for (String assetName: ASSET_NAMES) {
//            try {
//                Log.v(TAG, "Copying " + assetName);
//                InputStream ais = assetManager.open(assetName);
//                FileOutputStream fos = openFileOutput(assetName, MODE_PRIVATE);
//                final int BUFSZ = 8192;
//                byte[] buffer = new byte[BUFSZ];
//                int readlen = 0;
//                do {
//                    readlen = ais.read(buffer, 0, BUFSZ);
//                    if (readlen < 0) {
//                        break;
//                    }
//                    fos.write(buffer, 0, readlen);
//                } while (readlen > 0);
//                fos.close();
//                ais.close();
//            } catch(IOException e){
//                Log.e(TAG, "Could not open " + assetName + " from assets, that should not happen", e);
//            }
//        }
//    }

    //    @Override
//    protected void onStart() {
//        super.onStart();
//        if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
//                != PackageManager.PERMISSION_GRANTED) {
//
//            // Should we show an explanation?
//            if (shouldShowRequestPermissionRationale(
//                    Manifest.permission.READ_EXTERNAL_STORAGE)) {
//                // Explain to the user why we need to read the contacts
//            }
//
//            requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE},
//                    1337);
//
//            // MY_PERMISSIONS_REQUEST_READ_EXTERNAL_STORAGE is an
//            // app-defined int constant that should be quite unique
//        }
//    }
//
//    @Override
//    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
//        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
//        switch (requestCode) {
//            case 1337:
//                // If request is cancelled, the result arrays are empty.
//                if (grantResults.length > 0 &&
//                        grantResults[0] == PackageManager.PERMISSION_GRANTED) {
//                    // Permission is granted. Continue the action or workflow
//                    // in your app.
//                }  else {
//                    // Explain to the user that the feature is unavailable because
//                    // the features requires a permission that the user has denied.
//                    // At the same time, respect the user's decision. Don't link to
//                    // system settings in an effort to convince the user to change
//                    // their decision.
//                }
//                return;
//        }
//        // Other 'case' lines to check for other
//        // permissions this app might request.
//    }




    @Override
    protected String[] getLibraries() {
        return new String[]{
                "hidapi",
                "SDL2",
                "test_android"
        };
    }

}