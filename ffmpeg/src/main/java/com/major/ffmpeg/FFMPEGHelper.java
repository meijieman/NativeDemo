package com.major.ffmpeg;

/**
 * Desc: TODO
 * <p>
 * Author: meijie
 * PackageName: com.major.ffmpeg
 * ProjectName: NativeDemo
 * Date: 2019/9/25 9:40
 */
public class FFMPEGHelper {
    static {
        System.loadLibrary("ffmpeg");
    }

    public static native void init();

}
