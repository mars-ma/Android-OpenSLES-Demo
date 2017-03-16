package dev.mars.openslesdemo;

import java.io.UnsupportedEncodingException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by ma.xuanwei on 2017/3/16.
 */

public class SpeexUtils {
    private ExecutorService executor = Executors.newSingleThreadExecutor();
    private NativeLib nativeBridge;

    public SpeexUtils() {
        nativeBridge = new NativeLib();
    }

    public void encode(final String pcm, final String speex) {
        executor.execute(new Runnable() {
            @Override
            public void run() {
                nativeBridge.encode(pcm,speex);
            }
        });
    }

    public void decode(final String defaultSpeexFilePath, final String defaultPcmOutputFilePath) {
        executor.execute(new Runnable() {
            @Override
            public void run() {
              nativeBridge.decode(defaultSpeexFilePath, defaultPcmOutputFilePath);
            }
        });
    }
}
