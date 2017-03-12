package dev.mars.openslesdemo;

/**
 * Created by mars_ma on 2017/3/12.
 */

public class FFMpegNative {
    static{
        System.loadLibrary("ffmpeg");
    }

    public native String getConfiguration();
}
