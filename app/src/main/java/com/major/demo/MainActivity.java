package com.major.demo;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        int i = nativeAdd(100, 2);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI() + " => " + i);

        findViewById(R.id.btn_log).setOnClickListener((v) -> {
            log();
            runThread();
        });

        LinearLayout image = findViewById(R.id.ll_root);


        findViewById(R.id.btn_blur).setOnClickListener((v) -> {
            Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.img_launcher);

            long l = System.currentTimeMillis();
            // 0 - 23
            blurBitmap(bitmap, 10);
            Log.w("tag_ma", "onCreate: " + (System.currentTimeMillis() - l));

            image.setBackground(new BitmapDrawable(bitmap));

        });

        findViewById(R.id.btn_http).setOnClickListener(v -> {

            http();

        });
        findViewById(R.id.btn_test).setOnClickListener(v -> {
            test();
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native int nativeAdd(int a, int b);

    public native void log();

    public native void runThread();

    // 参数r为对bitmap虚化的程度范围
    static native void blurBitmap(Object bitmap, int r);

    public native void http();

    public native void test();
}
