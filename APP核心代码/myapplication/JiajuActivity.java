package com.example.myapplication;

import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Toast;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

public class JiajuActivity extends AppCompatActivity implements JiajuListAdapter.OnItemClickListener{
    String kongtiao,zhiwuqi,chuanglian,chuanghu,tishi,fengshan,led;
    byte ledOn = 0,kongtiaoOn = 0,zhiwuqiOn = 0,chuanglianOn = 0,chuanghuOn = 0,tishiOn = 0,fengshanOn = 0;
    private RecyclerView recyclerView;
    private List<JiajuInfo> mJiajuInfoList = new ArrayList<>();
    private JiajuListAdapter jiajuListAdapter;
    private InputStream is;
    private InputStreamReader isr;
    private BufferedReader br;
    private String response,response_data;
    private Socket socket;
    private OutputStream outputStream;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jiaju);

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
                                Toast.makeText(JiajuActivity.this, "TCP状态：True", Toast.LENGTH_SHORT).show();
                            }
                        });
                    } catch (UnknownHostException e) {
                        e.printStackTrace();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(JiajuActivity.this, "未知主机异常", Toast.LENGTH_SHORT).show();
                            }
                        });
                    } catch (IOException e) {
                        e.printStackTrace();
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Toast.makeText(JiajuActivity.this, "IO异常", Toast.LENGTH_SHORT).show();
                            }
                        });
                    }
                }
            }
        }).start();

//        //初始化控件
//        recyclerView = findViewById(R.id.jiaju_recyclerview);
//        //模拟数据
//        mJiajuInfoList.add(new JiajuInfo(0,R.mipmap.ic_launcher,"First Ring 灰色/棕色V领修身针织马甲背心复古叠穿百搭马夹上衣","韩版","M","180.90"));
//        mJiajuInfoList.add(new JiajuInfo(0,R.mipmap.ic_launcher,"First Ring 灰色/棕色V领修身针织马甲背心复古叠穿百搭马夹上衣","韩版","M","180.90"));
//        mJiajuInfoList.add(new JiajuInfo(0,R.mipmap.ic_launcher,"First Ring 灰色/棕色V领修身针织马甲背心复古叠穿百搭马夹上衣","韩版","M","180.90"));
//
//        //创建适配器
//        jiajuListAdapter = new JiajuListAdapter(mJiajuInfoList);
//        //设置适配器
//        recyclerView.setAdapter(jiajuListAdapter);
        //recyclerView.setLayoutManager(new LinearLayoutManager(JiajuActivity.this));

//        jiajuListAdapter.setOnItemClickListener(this);

        // 点击返回
        findViewById(R.id.image_icon_jiaju).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 返回
                disconnect_tcp();
                ConnectionInfo.setReconnectNeeded(true);
                finish();
            }
        });
        // 点击集群模式
        findViewById(R.id.switch1).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(JiajuActivity.this, "尚未开发，敬请期待", Toast.LENGTH_SHORT).show();
            }
        });
        // 点击刷新
        findViewById(R.id.image_refresh_jiaju).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                recyclerView = findViewById(R.id.jiaju_recyclerview);


                // 在新线程中处理网络请求和数据读取
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        // 先发送请求
                        askstates();
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
                                    Log.d("YourResponse", "response: "+ key + value);
                                    //kongtiao=0&zhiwuqi=0&chuanglian=0&chuanghu=0&tishi=0&fengshan=1&led=1\n
                                    // 处理收到的数据，根据需要更新 UI
                                    if (key.equals("kongtiao")) {
                                        kongtiao = value;
                                    } else if (key.equals("zhiwuqi")) {
                                        zhiwuqi = value;
                                    } else if (key.equals("chuanglian")) {
                                        chuanglian = value;
                                    } else if (key.equals("chuanghu")) {
                                        chuanghu = value;
                                    } else if (key.equals("tishi")) {
                                        tishi = value;
                                    } else if (key.equals("fengshan")) {
                                        fengshan = value;
                                    }
                                    else if (key.equals("led")) {
                                        led = value;
                                    }
                                }
                            }

                            // 在主线程中更新 UI
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    // 清空数据列表
                                    mJiajuInfoList.clear();

                                    if (kongtiao.equals("1")) {
                                        mJiajuInfoList.add(new JiajuInfo(0, R.mipmap.ic_launcher, "模块：空调", "状态", "运行时间：", "其他"));
                                    }
                                    if (zhiwuqi.equals("1")) {
                                        mJiajuInfoList.add(new JiajuInfo(0, R.mipmap.ic_launcher, "模块：制雾器", "状态", "运行时间：", "其他"));
                                    }
                                    if (chuanglian.equals("1")) {
                                        mJiajuInfoList.add(new JiajuInfo(0, R.mipmap.ic_launcher, "模块：窗帘", "状态", "运行时间：", "其他"));
                                    }
                                    if (chuanghu.equals("1")) {
                                        mJiajuInfoList.add(new JiajuInfo(0, R.mipmap.ic_launcher, "模块：窗户", "状态", "运行时间：", "其他"));
                                    }
                                    if (tishi.equals("1")) {
                                        mJiajuInfoList.add(new JiajuInfo(0, R.mipmap.ic_launcher, "模块：提示", "状态", "运行时间：", "其他"));
                                    }if (fengshan.equals("1")) {
                                        mJiajuInfoList.add(new JiajuInfo(0, R.mipmap.ic_launcher, "模块：风扇", "状态", "运行时间：", "其他"));
                                    }if (led.equals("1")) {
                                        mJiajuInfoList.add(new JiajuInfo(0, R.mipmap.ic_launcher, "模块：led", "状态", "运行时间：", "其他"));
                                    }
                                    // ... （以此类推）

                                    //创建适配器
                                    jiajuListAdapter = new JiajuListAdapter(mJiajuInfoList);
                                    //设置适配器
                                    recyclerView.setAdapter(jiajuListAdapter);
                                    //recyclerView.setLayoutManager(new LinearLayoutManager(JiajuActivity.this));

                                    jiajuListAdapter.setOnItemClickListener(JiajuActivity.this);
                                    Toast.makeText(JiajuActivity.this, "刷新已完成", Toast.LENGTH_SHORT).show();
                                }
                            });
                        } catch (IOException e) {
                            e.printStackTrace();
                        }

                    }
                }).start();
            }
        });
    }
    @Override
    public void onItemClick(int pos) {
        Toast.makeText(this, "单击位置 " + pos, Toast.LENGTH_SHORT).show();
        // 根据 pos 参数，获取对应的 item 数据，比如 JiajuInfo 对象
        JiajuInfo jiajuInfo = mJiajuInfoList.get(pos);
        // 根据 item 数据的某个属性，比如 product_title，判断是否是你想要的模块
        if (jiajuInfo.getProduct_title().equals("模块：led")) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        if (ledOn == 0) {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_ledon\n").getBytes("utf-8"));
                            outputStream.flush();
                            ledOn = 1;
                            Log.d("YourResponse", "send:app_ledon ");
                        } else {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_ledoff\n").getBytes("utf-8"));
                            outputStream.flush();
                            ledOn = 0;
                            Log.d("YourResponse", "send:app_ledoff ");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }

        if (jiajuInfo.getProduct_title().equals("模块：空调")) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        if (kongtiaoOn==0) {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_kongtiaoon\n").getBytes("utf-8"));
                            outputStream.flush();
                            kongtiaoOn = 1;
                            Log.d("YourResponse", "send:app_kongtiaoon ");
                        } else {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_kongtiaooff\n").getBytes("utf-8"));
                            outputStream.flush();
                            kongtiaoOn = 0;
                            Log.d("YourResponse", "send:app_kongtiaooff ");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }

        if (jiajuInfo.getProduct_title().equals("模块：制雾器")) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        if (zhiwuqiOn==0) {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_zhiwuqion\n").getBytes("utf-8"));
                            outputStream.flush();
                            zhiwuqiOn = 1;
                            Log.d("YourResponse", "send:app_zhiwuqion ");
                        } else {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_zhiwuqioff\n").getBytes("utf-8"));
                            outputStream.flush();
                            zhiwuqiOn = 0;
                            Log.d("YourResponse", "send:app_zhiwuqioff ");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }

        if (jiajuInfo.getProduct_title().equals("模块：窗户")) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        if (chuanghuOn==0) {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_chuanghuon\n").getBytes("utf-8"));
                            outputStream.flush();
                            chuanghuOn = 1;
                            Log.d("YourResponse", "send:app_chuanghuon ");
                        } else {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_chuanghuoff\n").getBytes("utf-8"));
                            outputStream.flush();
                            chuanghuOn = 0;
                            Log.d("YourResponse", "send:app_chuanghuoff ");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }

        if (jiajuInfo.getProduct_title().equals("模块：提示")) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        if (tishiOn==0) {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_tishion\n").getBytes("utf-8"));
                            outputStream.flush();
                            tishiOn = 1;
                            Log.d("YourResponse", "send:app_tishion ");
                        } else {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_tishioff\n").getBytes("utf-8"));
                            outputStream.flush();
                            tishiOn = 0;
                            Log.d("YourResponse", "send:app_tishioff ");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }

        if (jiajuInfo.getProduct_title().equals("模块：风扇")) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        if (fengshanOn==0) {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_fengshanon1\n").getBytes("utf-8"));
                            outputStream.flush();
                            fengshanOn = 1;
                            Log.d("YourResponse", "send:app_fengshanon1 ");
                        } else if (fengshanOn==1) {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_fengshanon2\n").getBytes("utf-8"));
                            outputStream.flush();
                            fengshanOn = 2;
                            Log.d("YourResponse", "send:app_fengshanon2 ");
                        } else {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_fengshanoff\n").getBytes("utf-8"));
                            outputStream.flush();
                            fengshanOn = 0;
                            Log.d("YourResponse", "send:app_fengshanoff ");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }
        if (jiajuInfo.getProduct_title().equals("模块：窗帘")) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        if (chuanglianOn==0) {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_chuanglianon1\n").getBytes("utf-8"));
                            outputStream.flush();
                            chuanglianOn = 1;
                            Log.d("YourResponse", "send:app_chuanglianon1 ");
                        } else if (chuanglianOn==1) {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_chuanglianon2\n").getBytes("utf-8"));
                            outputStream.flush();
                            chuanglianOn = 2;
                            Log.d("YourResponse", "send:app_chuanglianon2 ");
                        } else {
                            outputStream = socket.getOutputStream();
                            outputStream.write(("app_chuanglianoff\n").getBytes("utf-8"));
                            outputStream.flush();
                            chuanglianOn = 0;
                            Log.d("YourResponse", "send:app_chuanglianoff ");
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        }

    }

    @Override
    public void onItemLongClick(int pos) {
        showDeleteDialog(pos);
        Toast.makeText(this, "长按位置 " + pos, Toast.LENGTH_SHORT).show();
    }

    private void showDeleteDialog(final int pos) {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("确认删除");
        builder.setMessage("是否删除该项？");
        builder.setPositiveButton("是", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                // 用户点击是，执行删除逻辑
                deleteItem(pos);
            }
        });
        builder.setNegativeButton("否", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                // 用户点击否，关闭对话框
                dialogInterface.dismiss();
            }
        });
        builder.create().show();
    }

    private void deleteItem(int position) {
        // 从数据列表中移除相应位置的项
        mJiajuInfoList.remove(position);
        // 通知适配器有项被移除
        jiajuListAdapter.notifyItemRemoved(position);
        // 如果不是最后一项，需要更新被移除项后面的所有项的位置
        if (position != mJiajuInfoList.size()) {
            jiajuListAdapter.notifyItemRangeChanged(position, mJiajuInfoList.size() - position);
        }
        // 提示删除成功
        Toast.makeText(this, "删除成功", Toast.LENGTH_SHORT).show();
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
    private void askstates() {
        if (socket != null && socket.isConnected()) {
            try {
                outputStream = socket.getOutputStream();
                outputStream.write(("askstates\n").getBytes("utf-8"));
                outputStream.flush();
            } catch (IOException e) {
                e.printStackTrace();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(JiajuActivity.this, "正在刷新", Toast.LENGTH_SHORT).show();
                    }
                });
            }
        } else {
            // Handle case where the socket is not connected
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(JiajuActivity.this, "未连接", Toast.LENGTH_SHORT).show();
                }
            });
        }
    }

}
