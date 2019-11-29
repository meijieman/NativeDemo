package com.major.mp;

import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Desc: TODO
 * <p>
 * Author: meijie
 * PackageName: com.major.mp
 * ProjectName: NativeDemo
 * Date: 2019/11/25 9:56
 */
public class MediaPlayer implements SurfaceHolder.Callback {

    private static final String TAG = "MediaPlayer";

    static {
        System.loadLibrary("player");
    }

    // 准备播放，进行初始化
    private native void native_prepare(String path);

    private native void native_start();

    private native void native_stop();

    private native void native_release();

    private native void native_set_surface(Surface surface);


    private SurfaceHolder mHolder;

    private OnProgressListener onProgressListener;
    private OnErrorListener onErrorListener;
    private OnPrepareListener onPrepareListener;

    public void setSurfaceView(SurfaceView surfaceView) {
        if (mHolder != null) {
            mHolder.removeCallback(this);
        }
        mHolder = surfaceView.getHolder();
        mHolder.addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.d(TAG, "surfaceCreated: ");
        native_set_surface(mHolder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.d(TAG, "surfaceChanged: ");
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.d(TAG, "surfaceDestroyed: ");
    }

    /**
     * 准备播放
     *
     * @param path 播放地址
     */
    public void prepare(String path) {
        native_prepare(path);
    }

    /**
     * 开始播放
     */
    public void start() {
        native_start();
    }

    public void stop() {
        native_stop();
    }

    public void release() {
        mHolder.removeCallback(this);
        native_release();
    }

    public void setOnPrepareListener(OnPrepareListener onPrepareListener) {
        this.onPrepareListener = onPrepareListener;
    }

    public void setOnErrorListener(OnErrorListener onErrorListener) {
        this.onErrorListener = onErrorListener;
    }

    public void setOnProgressListener(OnProgressListener onProgressListener) {
        this.onProgressListener = onProgressListener;
    }

    // Native层反射调用，运行在子线程
    public void onPrepare() {
        if (onPrepareListener != null) {
            onPrepareListener.onPrepare();
        }
    }

    // Native层反射调用，运行在子线程
    public void onError(int errorCode) {
        if (onErrorListener != null) {
            onErrorListener.onError(errorCode);
        }
    }

    // Native层反射调用
    public void onProgress(int progress) {
        if (onProgressListener != null) {
            onProgressListener.onProgress(progress);
        }
    }

    /**
     * 播放器准备完成
     */
    public interface OnPrepareListener {
        void onPrepare();
    }

    /**
     * 播放器异常
     */
    public interface OnErrorListener {
        void onError(int error);
    }

    /**
     * 播放器进度回调
     */
    public interface OnProgressListener {
        void onProgress(int progress);
    }
}
