package com.example.myapplication;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.Socket;

import androidx.appcompat.app.AppCompatActivity;

import java.net.Socket;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;

public class HuanjingActivity extends AppCompatActivity {
    String wendu, shidu, guangzhao, co2, shengxiang, fengli;
    private InputStream is;
    private InputStreamReader isr;
    private BufferedReader br;
    private Socket socket;
    private String response;

    // 引入 Handler 和 UPDATE_INTERVAL
    private Handler mHandler;
    private static final int UPDATE_INTERVAL = 2000; // 更新间隔时间，单位毫秒

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_huanjing);

        // 初始化 Handler
        mHandler = new Handler();

        // 在新线程中执行 TCP 连接
        new Thread(new Runnable() {
            @Override
            public void run() {
                // 取出 TCP 连接状态
                ConnectionInfo connectionInfo = ConnectionInfo.getInstance();
                boolean isConnected = connectionInfo.isConnected();

                // 如果连接状态为 true，则自动建立 TCP 连接
                if (isConnected) {
                    // 在这里添加建立 TCP 连接的代码，可以参考之前的连接代码
                    String ip = connectionInfo.getIpAddress();
                    int port = connectionInfo.getPort();

                    try {
                        socket = new Socket(ip, port);
                        System.out.println(socket.isConnected());
                        // 示范代码
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(HuanjingActivity.this, "TCP状态：True", Toast.LENGTH_SHORT).show();
                            }
                        });
                    } catch (UnknownHostException e) {
                        e.printStackTrace();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(HuanjingActivity.this, "未知主机异常", Toast.LENGTH_SHORT).show();
                            }
                        });
                    } catch (IOException e) {
                        e.printStackTrace();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(HuanjingActivity.this, "IO异常", Toast.LENGTH_SHORT).show();
                            }
                        });
                    }
                }
            }
        }).start();

        // 点击返回
        findViewById(R.id.toolbar_huanjing).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 返回
                stopUpdateTask();
                disconnect_tcp();
                ConnectionInfo.setReconnectNeeded(true);
                finish();
            }
        });

        findViewById(R.id.button_wendu).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(HuanjingActivity.this, WenduActivity.class);
                startActivity(intent);
            }
        });

        findViewById(R.id.button_shidu).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(HuanjingActivity.this, ShiduActivity.class);
                startActivity(intent);
            }
        });

        findViewById(R.id.button_guangzhao).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(HuanjingActivity.this, GuangzhaoActivity.class);
                startActivity(intent);
            }
        });

        findViewById(R.id.button_co2).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(HuanjingActivity.this, Co2Activity.class);
                startActivity(intent);
            }
        });

        findViewById(R.id.button_shengxiang).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(HuanjingActivity.this, ShengxiangActivity.class);
                startActivity(intent);
            }
        });

        findViewById(R.id.button_fengli).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(HuanjingActivity.this, FengliActivity.class);
                startActivity(intent);
            }
        });

        findViewById(R.id.button_qita1).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(HuanjingActivity.this, "闲置控件，尚未开发", Toast.LENGTH_SHORT).show();
            }
        });

        findViewById(R.id.button_qita2).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(HuanjingActivity.this, "闲置控件，尚未开发", Toast.LENGTH_SHORT).show();
            }
        });

        // 启动定时更新任务
        startUpdateTask();
    }

    private void startUpdateTask() {
        // 使用定时任务定时获取信息并更新 UI
        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                // 获取信息的代码，这里简化为一个示例
                // 此处应该是从 TCP 中获取最新的数据
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            socket.setSoTimeout(2000); // 设置超时时间为2秒

                            is = socket.getInputStream();
                            isr = new InputStreamReader(is);
                            br = new BufferedReader(isr);
                            response = br.readLine();
                            String[] dataPairs = response.split("&");

                            for (String pair : dataPairs) {
                                String[] keyValue = pair.split("=");

                                if (keyValue.length == 2) {
                                    String key = keyValue[0];
                                    String value = keyValue[1];

                                    // 处理收到的数据，根据需要更新 UI
                                    if (key.equals("wendu")) {
                                        wendu = value;
                                    } else if (key.equals("shidu")) {
                                        shidu = value;
                                    } else if (key.equals("guangzhao")) {
                                        guangzhao = value;
                                    } else if (key.equals("co2")) {
                                        co2 = value;
                                    } else if (key.equals("shengxiang")) {
                                        shengxiang = value;
                                    } else if (key.equals("fengli")) {
                                        fengli = value;
                                    }
                                }
                            }
                        } catch (SocketTimeoutException e) {
                            // 捕获超时异常，可以执行一些其他操作
                            e.printStackTrace();
                            // 中断线程
                            Thread.currentThread().interrupt();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }).start();

                // 更新按钮上的信息
                updateButtonInfo();

                // 继续下一次的定时任务
                mHandler.postDelayed(this, UPDATE_INTERVAL);
            }
        }, UPDATE_INTERVAL);
    }

    private void stopUpdateTask() {
        // 停止定时任务
        mHandler.removeCallbacksAndMessages(null);
    }

    private void updateButtonInfo() {
        // 此处更新按钮上的信息，你可以根据具体的业务逻辑进行修改
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                // 示例：更新温度按钮
                Button wenduButton = findViewById(R.id.button_wendu);
                wenduButton.setText("温度：" + wendu);
                Button shiduButton = findViewById(R.id.button_shidu);
                shiduButton.setText("湿度：" + shidu);
                Button guangzhaoButton = findViewById(R.id.button_guangzhao);
                guangzhaoButton.setText("光照：" + guangzhao);
                Button co2Button = findViewById(R.id.button_co2);
                co2Button.setText("气体：" + co2);
                Button shengxiangButton = findViewById(R.id.button_shengxiang);
                shengxiangButton.setText("声响：" + shengxiang);
                Button fengliButton = findViewById(R.id.button_fengli);
                fengliButton.setText("风力：" + fengli);
            }
        });
    }

    private void disconnect_tcp() {
        try {
            if (br != null) {
                br.close();
            }
            if (socket != null && socket.isConnected()) {
                socket.close();
            }

            if (socket != null && !socket.isConnected()) {
                System.out.println("Socket disconnected");
            } else {
                System.out.println("Socket is still connected");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
