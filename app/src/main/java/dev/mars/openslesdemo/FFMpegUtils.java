package dev.mars.openslesdemo;

/**
 * Created by mars_ma on 2017/3/11.
 */

public class FFMpegUtils {

    private FFMpegNative ffMpegNative;

    public FFMpegUtils(){
        ffMpegNative = new FFMpegNative();
    }

    public String getConfiguration(){
        return ffMpegNative.getConfiguration();
    }
}
