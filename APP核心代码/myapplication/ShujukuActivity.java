package com.example.myapplication;

import android.app.DatePickerDialog;
import android.app.TimePickerDialog;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.TimePicker;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Calendar;
import java.io.OutputStream;
import java.util.List;

public class ShujukuActivity extends AppCompatActivity {
    String time,wendu, shidu, guangzhao, co2, shengxiang, fengli;
    private InputStream is;
    private InputStreamReader isr;
    private BufferedReader br;
    private String response,response_data;
    private Socket socket;
    private OutputStream outputStream;
    private Button btn_date;
    private Button btn_time;
    private int year, month, day, hour, minute;
    RecyclerView recyclerView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_shujuku);

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
                        //
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(ShujukuActivity.this, "TCP状态：True", Toast.LENGTH_SHORT).show();
                            }
                        });
                    } catch (UnknownHostException e) {
                        e.printStackTrace();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(ShujukuActivity.this, "未知主机异常", Toast.LENGTH_SHORT).show();
                            }
                        });
                    } catch (IOException e) {
                        e.printStackTrace();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(ShujukuActivity.this, "IO异常", Toast.LENGTH_SHORT).show();
                            }
                        });
                    }
                }
            }
        }).start();

        // 点击返回
        findViewById(R.id.toolbar_shujuku).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 返回
                disconnect_tcp();
                ConnectionInfo.setReconnectNeeded(true);
                finish();
            }
        });

        // 获取当前的日期和时间
        Calendar calendar = Calendar.getInstance();
        year = calendar.get(Calendar.YEAR);
        month = calendar.get(Calendar.MONTH);
        day = calendar.get(Calendar.DAY_OF_MONTH);
        hour = calendar.get(Calendar.HOUR_OF_DAY);
        minute = calendar.get(Calendar.MINUTE);

        // 获取 Button 控件的引用
        btn_date = findViewById(R.id.btn_date);
        btn_time = findViewById(R.id.btn_time);

        // 为 Button 设置点击事件
        btn_date.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 创建 DatePickerDialog 对象，传入当前的日期和监听器
                DatePickerDialog datePickerDialog = new DatePickerDialog(ShujukuActivity.this, new DatePickerDialog.OnDateSetListener() {
                    @Override
                    public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                        // 在监听器中处理用户选择的日期
                        ShujukuActivity.this.year = year;
                        ShujukuActivity.this.month = month;
                        day = dayOfMonth;
                        // 显示用户选择的日期
                        Toast.makeText(ShujukuActivity.this, "您选择的日期是：" + ShujukuActivity.this.year + "年" + (ShujukuActivity.this.month + 2) + "月" + ShujukuActivity.this.day + "日", Toast.LENGTH_SHORT).show();
                        btn_date.setText("选择日期（"+ ShujukuActivity.this.year +"-" + ShujukuActivity.this.month + "-" + day + ")");
                    }
                }, year, month, day);
                // 显示日期选择器对话框
                datePickerDialog.show();
            }
        });

        btn_time.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                recyclerView = findViewById(R.id.recyclerview);
                ArrayList<UserModel> userlist = new ArrayList<>();
                userlist.add(new UserModel());

                TimePickerDialog timePickerDialog = new TimePickerDialog(ShujukuActivity.this, new TimePickerDialog.OnTimeSetListener() {
                    @Override
                    public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
                        // 处理用户选择的时间
                        hour = hourOfDay;
                        ShujukuActivity.this.minute = minute;

                        Toast.makeText(ShujukuActivity.this, "您选择的时间是：" + hour + "时" + ShujukuActivity.this.minute + "分", Toast.LENGTH_SHORT).show();
                        btn_time.setText("选择时间（"+ hour +":" + ShujukuActivity.this.minute + ")");
                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                sendDataToServer();
                            }
                        }).start();

                        // 在新线程中处理网络请求和数据读取
                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                try {
                                    socket.setSoTimeout(3000);

                                    is = socket.getInputStream();
                                    isr = new InputStreamReader(is);
                                    br = new BufferedReader(isr);
                                    if(ShujukuActivity.this.minute != 0){
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
                                                } else if (key.equals("time")) {
                                                    time = value;
                                                }
                                            }
                                        }
                                        userlist.add(new UserModel(time,wendu,shidu,guangzhao,co2,shengxiang,fengli));
                                    }else{
                                        String line;

                                        response_data = br.readLine();
                                        String[] responsesList = response_data.split("\\|\\|");
                                        Log.d("YourResponseList", "responsesList: " + responsesList);

                                        for (String response : responsesList) {
                                            String[] dataPairs = response.split("&");
                                            for (String pair : dataPairs) {
                                                String[] keyValue = pair.split("=");

                                                if (keyValue.length == 2) {
                                                    String key = keyValue[0];
                                                    String value = keyValue[1];

                                                    // 处理收到的数据，根据需要更新 UI
                                                    if (key.equals("1")) {
                                                        wendu = value;
                                                    } else if (key.equals("2")) {
                                                        shidu = value;
                                                    } else if (key.equals("3")) {
                                                        guangzhao = value;
                                                    } else if (key.equals("4")) {
                                                        co2 = value;
                                                    } else if (key.equals("5")) {
                                                        shengxiang = value;
                                                    } else if (key.equals("6")) {
                                                        fengli = value;
                                                    } else if (key.equals("7")) {
                                                        time = value;
                                                    }

                                                }
                                            }
                                            Log.d("YourResponse", "response: " + response);

                                            userlist.add(new UserModel(time,wendu,shidu,guangzhao,co2,shengxiang,fengli));
                                        }

                                    }

                                } catch (SocketTimeoutException e) {
                                    e.printStackTrace();
                                    Thread.currentThread().interrupt();
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }

                                // 在新线程中更新 UI
                                runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {

                                        RecyclerViewAdapter recyclerViewAdapter = new RecyclerViewAdapter(ShujukuActivity.this, userlist);
                                        recyclerView.setAdapter(recyclerViewAdapter);
                                        recyclerView.setLayoutManager(new LinearLayoutManager(ShujukuActivity.this));
                                    }
                                });
                            }
                        }).start();
                    }
                }, hour, minute, true);

                timePickerDialog.show();
            }
        });
    }
    private void disconnect_tcp() {
        try {
            if (outputStream != null) {
                outputStream.close();
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
    // Method to send data to the server via TCP
    private void sendDataToServer() {
        if (socket != null && socket.isConnected()) {
            try {
                outputStream = socket.getOutputStream();
                if(ShujukuActivity.this.minute == 0){
                    outputStream.write(("select_table&" + year + "&" + (month+1) + "&" + day + "&" + hour + "&" + minute + "&\n").getBytes("utf-8"));
                }
                else{
                    outputStream.write(("select_row&" + year + "&" + (month+1) + "&" + day + "&" + hour + "&" + minute + "&\n").getBytes("utf-8"));
                }
                outputStream.flush();

            } catch (IOException e) {
                e.printStackTrace();
                // Handle exception (e.g., show a toast indicating failure)
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(ShujukuActivity.this, "Failed to send data to server", Toast.LENGTH_SHORT).show();
                    }
                });
            }
        } else {
            // Handle case where the socket is not connected
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(ShujukuActivity.this, "Socket is not connected", Toast.LENGTH_SHORT).show();
                }
            });
        }
    }
}
