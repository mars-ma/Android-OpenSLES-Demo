package dev.mars.openslesdemo;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.CheckBox;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    String[] pers = new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.RECORD_AUDIO};
    private OpenSLRecorder recorder;
    private OpenSLPlayer player;
    private SpeexUtils speexUtils;
    private AudioUtils audioUtils;
    CheckBox cb1,cb2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        cb1 = (CheckBox) findViewById(R.id.cb1);
        cb2 = (CheckBox) findViewById(R.id.cb2);
        recorder = new OpenSLRecorder();
        player = new OpenSLPlayer();
        speexUtils = new SpeexUtils();
        audioUtils= new AudioUtils();
    }


    public void startRecord(View view) {
        if (hasPermission()) {
            startToRecord();
        } else {
            requestPermissions();
        }
    }

    public void startToRecord(){
        if(!recorder.startToRecord(Common.SAMPLERATE,Common.PERIOD_TIME,Common.CHANNELS,Common.DEFAULT_PCM_FILE_PATH)){
            Toast.makeText(MainActivity.this,"Already in recording state!",Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(MainActivity.this,"Start recording!",Toast.LENGTH_SHORT).show();
        }
    }

    public void stopRecord(View view) {
        if(!recorder.stopRecording()){
            Toast.makeText(MainActivity.this,"Not in recording state!",Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(MainActivity.this,"Recording stopped!",Toast.LENGTH_SHORT).show();
        }
    }

    private void requestPermissions(){
        if(isLollipop()) {
            requestPermissions(pers, 0);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if(requestCode==0){
            if(hasPermission()){
                startToRecord();
            }else{
                Toast.makeText(MainActivity.this,"Unable to get permissions",Toast.LENGTH_SHORT).show();
            }
        }
    }

    private boolean hasPermission() {
        return hasPermission(pers);
    }


    private boolean hasPermission(String[] pers) {
        if(isLollipop()){
            for(String per:pers){
                if(checkSelfPermission(per)!= PackageManager.PERMISSION_GRANTED){
                    return false;
                }
            }
        }
        return true;
    }

    private boolean isLollipop(){
        return Build.VERSION.SDK_INT>Build.VERSION_CODES.LOLLIPOP_MR1;
    }


    public void startPlay(View view) {
        if(!player.startToPlay(Common.SAMPLERATE,Common.PERIOD_TIME,Common.CHANNELS,Common.DEFAULT_PCM_FILE_PATH)){
            Toast.makeText(MainActivity.this,"Is playing!",Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(MainActivity.this,"Start playing!",Toast.LENGTH_SHORT).show();
        }
    }

    public void stopPlay(View view) {
        if(!player.stopPlaying()){
            Toast.makeText(MainActivity.this,"Not playing!",Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(MainActivity.this,"Playing stopped!",Toast.LENGTH_SHORT).show();
        }
    }

    public void encodeWithSpeex(View view) {
        speexUtils.encode(Common.DEFAULT_PCM_FILE_PATH,Common.DEFAULT_SPEEX_FILE_PATH);
    }

    public void decodeWithSpeex(View view) {
        speexUtils.decode(Common.DEFAULT_SPEEX_FILE_PATH,Common.DEFAULT_PCM_OUTPUT_FILE_PATH);
    }

    public void playOutpuPCM(View view) {
        if(!player.startToPlay(Common.SAMPLERATE,Common.PERIOD_TIME,Common.CHANNELS,Common.DEFAULT_PCM_OUTPUT_FILE_PATH)){
            Toast.makeText(MainActivity.this,"Is playing!",Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(MainActivity.this,"Start playing!",Toast.LENGTH_SHORT).show();
        }
    }

    public void recordAndPlayPCM(View view) {
        if(!audioUtils.recordAndPlayPCM(cb1.isChecked(),cb2.isChecked())){
            Toast.makeText(MainActivity.this,"Is recording and playing!",Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(MainActivity.this,"Start recording and playing!",Toast.LENGTH_SHORT).show();
        }
    }

    public void stopRecordAndPlayPCM(View view) {
        if(!audioUtils.stopRecordAndPlay()){
            Toast.makeText(MainActivity.this,"not in recording and playing state!",Toast.LENGTH_SHORT).show();
        }else{
            Toast.makeText(MainActivity.this,"recording and playing stoped!",Toast.LENGTH_SHORT).show();
        }
    }
}
