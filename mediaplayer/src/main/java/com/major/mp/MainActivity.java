package com.major.mp;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.widget.SeekBar;
import android.widget.Toast;

import java.io.File;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "log_MainActivity";

    private static final String FILE_NAME = "input2.mp4";

    private SurfaceView mSurfaceView;
    private SeekBar seekBar;
    private int progress;
    private MediaPlayer mPlayer;
    private File mFile;
    private boolean taskRunning;

    private SeekBar.OnSeekBarChangeListener mOnSeekBarChangeListener = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {

        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.activity_main);
        mSurfaceView = findViewById(R.id.surfaceView);
        seekBar = findViewById(R.id.seekBar);

        mPlayer = new MediaPlayer();
        mPlayer.setSurfaceView(mSurfaceView);
        mFile = new File(Environment.getExternalStorageDirectory(), FILE_NAME);
        Log.w(TAG, "file " + mFile.getAbsolutePath());

        initEvent();

    }

    private void initEvent() {
        findViewById(R.id.play_video).setOnClickListener(this);
        findViewById(R.id.stop_video).setOnClickListener(this);

        seekBar.setOnSeekBarChangeListener(mOnSeekBarChangeListener);
        mPlayer.setOnPrepareListener(() -> {
            // 回调在子线程
            mPlayer.start();
        });
        mPlayer.setOnErrorListener(error -> {
            // 回调在子线程
            Log.e(TAG, "onError: code = " + error);
        });
    }

    @Override
    protected void onDestroy() {
        mPlayer.release();
        super.onDestroy();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.play_video:
                Log.d(TAG, "playVideo: " + Thread.currentThread().getName());
                if (taskRunning) {
                    Toast.makeText(this, "当前正在播放", Toast.LENGTH_SHORT).show();
                } else {
                    taskRunning = true;
                    mPlayer.prepare(mFile.getAbsolutePath());
                }
                break;
            case R.id.stop_video:
//                mPlayer.stop();
                break;
        }
    }
}
