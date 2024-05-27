package com.example.myapplication;

import android.app.DatePickerDialog;
import android.app.TimePickerDialog;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.TimePicker;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
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

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import java.util.ArrayList;
import java.util.List;
public class TubiaoActivity extends AppCompatActivity {
    private InputStream is;
    private InputStreamReader isr;
    private BufferedReader br;
    private String response,response_data;
    private Socket socket;
    private OutputStream outputStream;
    private Button btn_date;
    private Button btn_time;
    private int year, month, day, hour, minute;
    int[] switch_tubiao_state = {0};
    private List<List<Double>> responseDataList = new ArrayList<>();

    private BarChart barChart;
    private LineChart lineChart;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tubiao);
        //--------------------------------------------------------------------------------
        //条形图
        // 通过 id 查找条形图视图
        barChart = findViewById(R.id.bar_chart);

        // 配置条形图的外观
        barChart.setDrawBarShadow(false); // 禁用条形图阴影
        barChart.setDrawValueAboveBar(true); // 在条形图上方显示值
        barChart.setMaxVisibleValueCount(50); // 设置最大可见值数量
        barChart.setPinchZoom(false); // 禁用捏放缩
        barChart.setDrawGridBackground(false); // 禁用网格背景

        // 创建一个条形图数据项列表来存储数据
        List<BarEntry> barEntries = new ArrayList<>();

        // 添加一些用于演示的虚拟数据，你可以用实际数据替换它
        barEntries.add(new BarEntry(0f, 0f));
        barEntries.add(new BarEntry(1f, 0f));
        barEntries.add(new BarEntry(3f, 0f));
        // 创建带有标签的条形图数据集
        BarDataSet barDataSet = new BarDataSet(barEntries, "数据集");

        // 创建带有数据集的条形图数据对象
        BarData barData = new BarData(barDataSet);

        // 将条形图数据设置到条形图中
        barChart.setData(barData);

        // 动画显示条形图
        barChart.animateY(1000);
        //-------------------------------------------------------------------------------------------

        //-------------------------------------------------------------------------------------------
        // 通过 id 查找折线图视图
        lineChart = findViewById(R.id.line_chart);

        // 配置折线图的外观
        lineChart.setDragEnabled(true); // 启用拖动手势
        lineChart.setScaleEnabled(true); // 启用缩放手势
        lineChart.setPinchZoom(true); // 启用捏放缩
        lineChart.setDrawGridBackground(false); // 禁用网格背景
        lineChart.getDescription().setEnabled(false); // 禁用描述文本

        // 创建一个用于存储数据的条目列表
        List<Entry> entries = new ArrayList<>();

        // 添加一些用于演示的虚拟数据，你可以用实际数据替换它
        // x 值是时间戳（毫秒），y 值是摄氏度温度
        entries.add(new Entry(1f, 0f)); // 2024-01-18 12:00:00
        entries.add(new Entry(2f, 0f)); // 2024-01-18 13:00:00
        entries.add(new Entry(3f, 0f)); // 2024-01-18 14:00:00

        // 创建带有标签和颜色的折线图数据集
        LineDataSet lineDataSet = new LineDataSet(entries, "温度折线图");
        lineDataSet.setColor(getResources().getColor(R.color.blue));

        // 创建带有数据集的折线图数据对象
        LineData lineData = new LineData(lineDataSet);

        // 将折线图数据设置到折线图中
        lineChart.setData(lineData);

        // 动画显示折线图
        lineChart.animateX(1000);

        //-------------------------------------------------------------------------------------------


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
                                Toast.makeText(TubiaoActivity.this, "TCP状态：True", Toast.LENGTH_SHORT).show();
                            }
                        });
                    } catch (UnknownHostException e) {
                        e.printStackTrace();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(TubiaoActivity.this, "未知主机异常", Toast.LENGTH_SHORT).show();
                            }
                        });
                    } catch (IOException e) {
                        e.printStackTrace();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(TubiaoActivity.this, "IO异常", Toast.LENGTH_SHORT).show();
                            }
                        });
                    }
                }
            }
        }).start();

        // 点击返回
        findViewById(R.id.image_icon_tubiao).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 返回
                disconnect_tcp();
                ConnectionInfo.setReconnectNeeded(true);
                finish();
            }
        });
        findViewById(R.id.switch_tubiao).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(switch_tubiao_state[0] == 0){
                    switch_tubiao_state[0] = 1;
                    Toast.makeText(TubiaoActivity.this, "精度为min", Toast.LENGTH_SHORT).show();
                } else if (switch_tubiao_state[0] == 1) {
                    switch_tubiao_state[0] = 0;
                    Toast.makeText(TubiaoActivity.this, "精度为h", Toast.LENGTH_SHORT).show();
                }
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
        btn_date = findViewById(R.id.btn_date_tubiao);
        btn_time = findViewById(R.id.btn_time_tubiao);

        // 为 Button 设置点击事件
        btn_date.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 创建 DatePickerDialog 对象，传入当前的日期和监听器
                DatePickerDialog datePickerDialog = new DatePickerDialog(TubiaoActivity.this, new DatePickerDialog.OnDateSetListener() {
                    @Override
                    public void onDateSet(DatePicker view, int year1, int month1, int dayOfMonth) {
                        // 在监听器中处理用户选择的日期
                        year = year1;
                        month = month1;
                        day = dayOfMonth;
                        // 显示用户选择的日期
                        Toast.makeText(TubiaoActivity.this, "您选择的日期是：" + year + "年" + (month + 1) + "月" + day + "日", Toast.LENGTH_SHORT).show();
                        btn_date.setText("选择日期（"+ year +"-" + (month + 1) + "-" + day + ")");
                    }
                }, year, month, day);
                // 显示日期选择器对话框
                datePickerDialog.show();
            }
        });

        btn_time.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                TimePickerDialog timePickerDialog = new TimePickerDialog(TubiaoActivity.this, new TimePickerDialog.OnTimeSetListener() {
                    @Override
                    public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
                        // 处理用户选择的时间
                        hour = hourOfDay;
                        TubiaoActivity.this.minute = minute;

                        Toast.makeText(TubiaoActivity.this, "您选择的时间是：" + hour + "时" + TubiaoActivity.this.minute + "分", Toast.LENGTH_SHORT).show();
                        btn_time.setText("选择时间（"+ hour +":" + TubiaoActivity.this.minute + ")");
                        // 在新线程中处理 sendDataToServer 调用
                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                sendDataToServer();
                            }
                        }).start();

                        if(switch_tubiao_state[0] == 0){
                            // 在新线程中处理网络请求和数据读取
                            new Thread(new Runnable() {
                                @Override
                                public void run() {
                                    try {
                                        responseDataList.clear();
                                        socket.setSoTimeout(3000);
                                        is = socket.getInputStream();
                                        isr = new InputStreamReader(is);
                                        br = new BufferedReader(isr);
                                        String line;
                                        response_data = br.readLine();
                                        String[] responsesList = response_data.split("\\|\\|");
                                        Log.d("YourResponseList", "responsesList: " + responsesList);
                                        for (String response : responsesList) {
//                                        // 处理收到的数据，根据需要更新 UI
//                                        if (key.equals("1")) {
//                                            wendu = value;
//                                        } else if (key.equals("2")) {
//                                            shidu = value;
//                                        } else if (key.equals("3")) {
//                                            guangzhao = value;
//                                        } else if (key.equals("4")) {
//                                            co2 = value;
//                                        } else if (key.equals("5")) {
//                                            shengxiang = value;
//                                        } else if (key.equals("6")) {
//                                            fengli = value;
//                                        } else if (key.equals("7")) {
//                                            time = value;
//                                        }
                                            // 将每次查询的结果解析为列表，并添加到 responseDataList 中
                                            List<Double> dataRow = parseResponse(response);
                                            Log.d("YourAppTag", "Parsed Response: " + dataRow.toString());
                                            responseDataList.add(dataRow);
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
                                            // 处理 UI 更新，例如显示或使用 responseDataList 中的数据
                                        }
                                    });
                                }
                            }).start();
                        } else if (switch_tubiao_state[0] == 1) {
                            // 在新线程中处理网络请求和数据读取
                            new Thread(new Runnable() {
                                @Override
                                public void run() {
                                    try {
                                        responseDataList.clear();
                                        socket.setSoTimeout(3000);
                                        is = socket.getInputStream();
                                        isr = new InputStreamReader(is);
                                        br = new BufferedReader(isr);
                                        String line;
                                        response_data = br.readLine();
                                        String[] responsesList = response_data.split("\\|\\|");
                                        Log.d("YourResponseList", "responsesList: " + responsesList);
                                        for (String response : responsesList) {
                                            // 将每次查询的结果解析为列表，并添加到 responseDataList 中
                                            List<Double> dataRow = parseResponse(response);
                                            Log.d("YourAppTag", "Parsed Response: " + dataRow.toString());
                                            responseDataList.add(dataRow);
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
                                            // 处理 UI 更新，例如显示或使用 responseDataList 中的数据
                                        }
                                    });
                                }
                            }).start();

                        }
                    }
                }, hour, minute, true);

                timePickerDialog.show();
            }
        });

        findViewById(R.id.btn_wendu_tubiao).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(responseDataList.isEmpty()){
                    Log.d("YourAppTag", "请先选择时间日期 ");
                    Toast.makeText(TubiaoActivity.this, "请先选择时间日期", Toast.LENGTH_SHORT).show();
                }
                else{
                    Log.d("YourAppTag", "已经选择时间日期 ");
                    // 从responseDataList中提取温度和时间数据
                    List<Float> wendu_list = new ArrayList<>();
                    List<Integer> timeIds = new ArrayList<>();
                    for (List<Double> dataRow : responseDataList) {
                        // 数据格式：[温度, 湿度, 光照, CO2, 声响, 风力, 时间ID]
                        float wendu = dataRow.get(0).floatValue(); // 提取温度
                        int timeId = dataRow.get(6).intValue(); // 提取时间ID

                        // 将提取的数据添加到相应的列表中
                        wendu_list.add(wendu);
                        timeIds.add(timeId);
                    }
                    // 将温度和时间数据用作图表的数据
                    updateChart(wendu_list, timeIds);
                }
            }
        });
        findViewById(R.id.btn_shidu_tubiao).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(responseDataList.isEmpty()){
                    Log.d("YourAppTag", "请先选择时间日期 ");
                    Toast.makeText(TubiaoActivity.this, "请先选择时间日期", Toast.LENGTH_SHORT).show();
                }
                else{
                    // 从responseDataList中提取温度和时间数据
                    List<Float> shidu_list = new ArrayList<>();
                    List<Integer> timeIds = new ArrayList<>();
                    for (List<Double> dataRow : responseDataList) {
                        // 数据格式：[温度, 湿度, 光照, CO2, 声响, 风力, 时间ID]
                        float shidu = dataRow.get(1).floatValue(); // 提取温度
                        int timeId = dataRow.get(6).intValue(); // 提取时间ID

                        // 将提取的数据添加到相应的列表中
                        shidu_list.add(shidu);
                        timeIds.add(timeId);
                    }
                    // 将温度和时间数据用作图表的数据
                    updateChart(shidu_list, timeIds);
                }

            }
        });
        findViewById(R.id.btn_guangzhao_tubiao).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(responseDataList.isEmpty()){
                    Log.d("YourAppTag", "请先选择时间日期 ");
                    Toast.makeText(TubiaoActivity.this, "请先选择时间日期", Toast.LENGTH_SHORT).show();
                }
                else{
                    // 从responseDataList中提取温度和时间数据
                    List<Float> guangzhao_list = new ArrayList<>();
                    List<Integer> timeIds = new ArrayList<>();
                    for (List<Double> dataRow : responseDataList) {
                        // 数据格式：[温度, 湿度, 光照, CO2, 声响, 风力, 时间ID]
                        float guangzhao = dataRow.get(2).floatValue(); // 提取温度
                        int timeId = dataRow.get(6).intValue(); // 提取时间ID

                        // 将提取的数据添加到相应的列表中
                        guangzhao_list.add(guangzhao);
                        timeIds.add(timeId);
                    }
                    // 将温度和时间数据用作图表的数据
                    updateChart(guangzhao_list, timeIds);
                }
            }
        });
        findViewById(R.id.btn_co2_tubiao).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(responseDataList.isEmpty()){
                    Log.d("YourAppTag", "请先选择时间日期 ");
                    Toast.makeText(TubiaoActivity.this, "请先选择时间日期", Toast.LENGTH_SHORT).show();
                }
                else{
                    // 从responseDataList中提取温度和时间数据
                    List<Float> co2_list = new ArrayList<>();
                    List<Integer> timeIds = new ArrayList<>();
                    for (List<Double> dataRow : responseDataList) {
                        // 数据格式：[温度, 湿度, 光照, CO2, 声响, 风力, 时间ID]
                        float co2 = dataRow.get(3).floatValue(); // 提取温度
                        int timeId = dataRow.get(6).intValue(); // 提取时间ID
                        // 将提取的数据添加到相应的列表中
                        co2_list.add(co2);
                        timeIds.add(timeId);
                    }
                    // 将温度和时间数据用作图表的数据
                    updateChart(co2_list, timeIds);
                }
            }
        });

        findViewById(R.id.btn_shengxiang_tubiao).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(responseDataList.isEmpty()){
                    Log.d("YourAppTag", "请先选择时间日期 ");
                    Toast.makeText(TubiaoActivity.this, "请先选择时间日期", Toast.LENGTH_SHORT).show();
                }
                else{
                    // 从responseDataList中提取温度和时间数据
                    List<Float> shengxiang_list = new ArrayList<>();
                    List<Integer> timeIds = new ArrayList<>();
                    for (List<Double> dataRow : responseDataList) {
                        // 数据格式：[温度, 湿度, 光照, CO2, 声响, 风力, 时间ID]
                        float shengxiang = dataRow.get(4).floatValue(); // 提取温度
                        int timeId = dataRow.get(6).intValue(); // 提取时间ID
                        // 将提取的数据添加到相应的列表中
                        shengxiang_list.add(shengxiang);
                        timeIds.add(timeId);
                    }
                    // 将温度和时间数据用作图表的数据
                    updateChart(shengxiang_list, timeIds);
                }
            }
        });

        findViewById(R.id.btn_fengli_tubiao).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(responseDataList.isEmpty()){
                    Log.d("YourAppTag", "请先选择时间日期 ");
                    Toast.makeText(TubiaoActivity.this, "请先选择时间日期", Toast.LENGTH_SHORT).show();
                }
                else{
                    // 从responseDataList中提取温度和时间数据
                    List<Float> fengli_list = new ArrayList<>();
                    List<Integer> timeIds = new ArrayList<>();
                    for (List<Double> dataRow : responseDataList) {
                        // 数据格式：[温度, 湿度, 光照, CO2, 声响, 风力, 时间ID]
                        float fengli = dataRow.get(5).floatValue(); // 提取温度
                        int timeId = dataRow.get(6).intValue(); // 提取时间ID
                        // 将提取的数据添加到相应的列表中
                        fengli_list.add(fengli);
                        timeIds.add(timeId);
                    }
                    // 将温度和时间数据用作图表的数据
                    updateChart(fengli_list, timeIds);
                }
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
                if(switch_tubiao_state[0] == 1){
                    outputStream.write(("select_table&" + year + "&" + (month+1) + "&" + day + "&" + hour + "&" + minute + "&\n").getBytes("utf-8"));
                } else if (switch_tubiao_state[0] == 0) {
                    outputStream.write(("select_day&" + year + "&" + (month+1) + "&" + day + "&" + hour + "&" + minute + "&\n").getBytes("utf-8"));
                }
                outputStream.flush();

            } catch (IOException e) {
                e.printStackTrace();
                // Handle exception (e.g., show a toast indicating failure)
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(TubiaoActivity.this, "Failed to send data to server", Toast.LENGTH_SHORT).show();
                    }
                });
            }
        } else {
            // Handle case where the socket is not connected
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(TubiaoActivity.this, "Socket is not connected", Toast.LENGTH_SHORT).show();
                }
            });
        }
    }
    // 解析每次查询的结果，并返回以列表形式存储的数据行
    private List<Double> parseResponse(String response) {
        List<Double> dataRow = new ArrayList<>();
        String[] dataPairs = response.split("\\|\\|");

        for (String pair : dataPairs) {
            String[] keyValuePairs = pair.split("&");

            for (String keyValue : keyValuePairs) {
                String[] keyValueArray = keyValue.split("=");

                if (keyValueArray.length == 2) {
                    String value = keyValueArray[1];

                    // 将字符串转换为 Double，并添加到数据行中
                    dataRow.add(Double.parseDouble(value));
                }
            }
        }
        return dataRow;
    }
    // 更新图表的方法
    private void updateChart(List<Float> data_x, List<Integer> timeIds) {
        // 创建一个用于存储数据的条目列表
        List<Entry> entries_line = new ArrayList<>();

        // 将温度和时间数据添加到条目列表中
        for (int i = 0; i < data_x.size(); i++) {
            entries_line.add(new Entry(timeIds.get(i), data_x.get(i)));
        }
        // 创建带有标签和颜色的折线图数据集
        LineDataSet lineDataSet = new LineDataSet(entries_line, "折线图");
        lineDataSet.setColor(getResources().getColor(R.color.blue));
        // 创建带有数据集的折线图数据对象
        LineData lineData = new LineData(lineDataSet);
        // 将折线图数据设置到折线图中
        lineChart.setData(lineData);
        // 动画显示折线图
        lineChart.animateX(1000);

        // 创建一个用于存储数据的条目列表
        List<BarEntry> entries_bar = new ArrayList<>();

        // 将温度和时间数据添加到条目列表中
        for (int i = 0; i < data_x.size(); i++) {
            entries_bar.add(new BarEntry(timeIds.get(i), data_x.get(i)));
        }
        // 创建带有标签的条形图数据集
        BarDataSet barDataSet = new BarDataSet(entries_bar, "条形图");
        // 创建带有数据集的条形图数据对象
        BarData barData = new BarData(barDataSet);
        // 将条形图数据设置到条形图中
        barChart.setData(barData);
        // 动画显示条形图
        barChart.animateY(1000);
    }


}
