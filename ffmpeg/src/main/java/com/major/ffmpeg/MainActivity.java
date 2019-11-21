package com.major.ffmpeg;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

/**
 * Desc: TODO
 * <p>
 * Author: meijie
 * PackageName: com.major.ffmpeg
 * ProjectName: NativeDemo
 * Date: 2019/9/25 9:32
 */
public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "log_ma";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.btn_1).setOnClickListener(this);
        findViewById(R.id.btn_2).setOnClickListener(this);
        findViewById(R.id.btn_3).setOnClickListener(this);
        findViewById(R.id.btn_4).setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_1:
                FFMPEGHelper.init();
                String ver = FFMPEGHelper.getVer();
                Log.i(TAG, "ver " + ver);
                Toast.makeText(MainActivity.this, "version " + ver, Toast.LENGTH_SHORT).show();

                break;
            case R.id.btn_2:
                String input = "/sdcard/langrenqingge.mp3";
                String output = "/sdcard/lang.pcm";
                int rst = FFMPEGHelper.decodeAudio(input, output);
                Log.i(TAG, "rst " + rst);


                break;
            case R.id.btn_3:

                break;
            case R.id.btn_4:

                break;
        }
    }
}
