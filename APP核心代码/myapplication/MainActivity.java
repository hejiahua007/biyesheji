package com.example.myapplication;

import static com.example.myapplication.ConnectionInfo.isReconnectNeeded;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import android.os.Handler;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private Handler mMainHandler;
    private Socket socket;
    private ExecutorService mThreadPool;
    private InputStream is;
    private InputStreamReader isr;
    private BufferedReader br;
    private OutputStream outputStream;
    private Button btnConnect, btnDisconnect, btnSend, btnReceive;
    private Button btnhuanjing, btnshujuku, btntubiao, btnjiaju;
    private EditText mEdit, editIP, editPort;
    private String wendu, shidu, guangzhao, co2, shengxiang, fengli;
    private TextView receive_message;
    private String response;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        btnConnect = findViewById(R.id.connect);
        btnDisconnect = findViewById(R.id.disconnect);
        btnSend = findViewById(R.id.send);
        btnReceive = findViewById(R.id.Receive);
        mEdit = findViewById(R.id.edit);
        editIP = findViewById(R.id.editIP);
        editPort = findViewById(R.id.editPort);
        receive_message = findViewById(R.id.receive_message);
        btnhuanjing = findViewById(R.id.button_huanjing);
        btnshujuku = findViewById(R.id.button_shujuku);
        btntubiao = findViewById(R.id.button_tubiao);
        btnjiaju = findViewById(R.id.button_jiaju);

        mThreadPool = Executors.newCachedThreadPool();
        mMainHandler = new Handler(Looper.getMainLooper()) {
            @Override
            public void handleMessage(Message msg) {
                switch (msg.what) {
                    case 0:
                        receive_message.setText(response);
                        break;
                }
            }
        };
        // 取出 TCP 连接状态
        ConnectionInfo connectionInfo = ConnectionInfo.getInstance();
        boolean isConnected = connectionInfo.isConnected();
        if (isConnected) {
            // 在这里添加建立 TCP 连接的代码，可以参考之前的连接代码
            String ip = connectionInfo.getIpAddress();
            int port = connectionInfo.getPort();

            try {
                socket = new Socket(ip, port);
                System.out.println(socket.isConnected());
                // 示范代码
                Toast.makeText(MainActivity.this, "TCP状态：True", Toast.LENGTH_SHORT).show();
            } catch (UnknownHostException e) {
                e.printStackTrace();
                Toast.makeText(MainActivity.this, "未知主机异常", Toast.LENGTH_SHORT).show();
            } catch (IOException e) {
                e.printStackTrace();
                Toast.makeText(MainActivity.this, "IO异常", Toast.LENGTH_SHORT).show();
            }
        }
        else{
            Toast.makeText(MainActivity.this, "TCP状态：False", Toast.LENGTH_SHORT).show();
        }

        btnConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mThreadPool.execute(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            String ip = editIP.getText().toString();
                            int port = Integer.parseInt(editPort.getText().toString());
                            ConnectionInfo.getInstance().setConnectionInfo(true, ip, port);
                            // 连接服务器
                            socket = new Socket(ip, port);
                            // 如果连接成功
                            if (socket.isConnected()) {
                                // 切换到主线程更新UI
                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        // 更新按钮文本
                                        btnConnect.setText("连接（已连接）");
                                        // 提示已连接
                                        Toast.makeText(MainActivity.this, "已连接", Toast.LENGTH_SHORT).show();
                                    }
                                });
                            }
                            System.out.println(socket.isConnected());
                        } catch (IOException e) {
                            e.printStackTrace();
                            // 切换到主线程显示连接失败提示
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    Toast.makeText(MainActivity.this, "连接失败", Toast.LENGTH_SHORT).show();
                                }
                            });
                        }
                    }
                });
            }
        });


        btnReceive.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mThreadPool.execute(new Runnable() {
                    @Override
                    public void run() {
                        try {
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
                                }
                            }

                            Message msg = Message.obtain();
                            msg.what = 0;
                            mMainHandler.sendMessage(msg);

                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                });
            }
        });

        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mThreadPool.execute(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            outputStream = socket.getOutputStream();
                            outputStream.write((mEdit.getText().toString() + "\n").getBytes("utf-8"));
                            outputStream.flush();

                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                });
            }
        });

        btnDisconnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                disconnect_tcp();
                btnConnect.setText("连接（未连接）");
            }
        });

        btnhuanjing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (socket != null && socket.isConnected()) {
                    disconnect_tcp();
                    Intent intent = new Intent(MainActivity.this, HuanjingActivity.class);
                    startActivity(intent);
                } else {
                    Toast.makeText(MainActivity.this, "未建立TCP连接", Toast.LENGTH_SHORT).show();
                }
            }
        });

        btnshujuku.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (socket != null && socket.isConnected()) {
                    disconnect_tcp();
                    Intent intent = new Intent(MainActivity.this, ShujukuActivity.class);
                    startActivity(intent);
                } else {
                    Toast.makeText(MainActivity.this, "未建立TCP连接", Toast.LENGTH_SHORT).show();
                }
            }
        });

        btntubiao.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (socket != null && socket.isConnected()) {
                    disconnect_tcp();
                    Intent intent = new Intent(MainActivity.this, TubiaoActivity.class);
                    startActivity(intent);
                } else {
                    Toast.makeText(MainActivity.this, "未建立TCP连接", Toast.LENGTH_SHORT).show();
                }
            }
        });

        btnjiaju.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (socket != null && socket.isConnected()) {
                    disconnect_tcp();
                    Intent intent = new Intent(MainActivity.this, JiajuActivity.class);
                    startActivity(intent);
                } else {
                    Toast.makeText(MainActivity.this, "未建立TCP连接", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void disconnect_tcp() {
        try {
            if (outputStream != null) {
                outputStream.close();
            }
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
    @Override
    protected void onResume() {
        super.onResume();
        if (isReconnectNeeded()) {
            // 重新建立 TCP 连接的代码，可以参考之前的连接代码
            ConnectionInfo connectionInfo = ConnectionInfo.getInstance();
            String ip = connectionInfo.getIpAddress();
            int port = connectionInfo.getPort();

            mThreadPool.execute(new Runnable() {
                @Override
                public void run() {
                    try {
                        socket = new Socket(ip, port);
                        System.out.println(socket.isConnected());
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            });
            ConnectionInfo.setReconnectNeeded(false);; // 重置标记
        }
    }
}
