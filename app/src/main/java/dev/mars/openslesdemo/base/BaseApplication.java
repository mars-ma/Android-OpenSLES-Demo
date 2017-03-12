package dev.mars.openslesdemo.base;

import android.app.Application;

import dev.mars.openslesdemo.FFMpegUtils;
import dev.mars.openslesdemo.LogUtils;

/**
 * Created by mars_ma on 2017/3/12.
 */

public class BaseApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        FFMpegUtils ffMpegUtils = new FFMpegUtils();
        LogUtils.DEBUG("configuration:"+ffMpegUtils.getConfiguration());
    }
}
