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

    public static native String getVer();

    /**
     * 音频解码
     *
     * @param input  输入音频路径
     * @param output 解码后PCM保存路径
     * @return 0成功，-1失败
     */
    public native static int decodeAudio(String input, String output);

}
