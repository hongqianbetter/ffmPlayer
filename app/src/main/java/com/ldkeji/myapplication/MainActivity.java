package com.ldkeji.myapplication;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {


    private SurfaceView surfaceView;
    private final String PATH="rtmp://58.200.131.2:1935/livetv/hunantv";
    private MyPlayer player;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        surfaceView = findViewById(R.id.surfaceView);
        player = new MyPlayer();
        player.setDataSource(PATH);
        player.setSurfaceView(surfaceView);

        player.setOnPreparedListener(new MyPlayer.OnPreparedListener() {
            @Override
            public void onPrepared() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        new AlertDialog.Builder(MainActivity.this)
                                .setTitle("UI")
                                .setMessage("准备好了，开始播放 ...")
                                .setPositiveButton("老夫知道了", null)
                                .show();
                    }
                });
                player.start(); // 开始播放
            }

            @Override
            public void onError(final String errorText) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        new AlertDialog.Builder(MainActivity.this)
                                .setTitle("Error")
                                .setMessage("已经发生错误，请查阅:" + errorText)
                                .setPositiveButton("我来个去，什么情况", null)
                                .show();
                    }
                });
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        player.prepare();
    }


    @Override
    protected void onPause() {
        super.onPause();
        player.stop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        player.release();
    }
}
