package com.example.myapplication;

import android.app.Application;

public class ConnectionInfo extends Application {
    private static ConnectionInfo instance;
    private boolean isConnected;
    private String ipAddress;
    private int port;
    private static boolean reconnectNeeded = false;

    // 默认构造函数
    public ConnectionInfo() {
        // 默认值或空实现
    }

    // 带参数的构造函数
    public ConnectionInfo(boolean isConnected, String ipAddress, int port) {
        // 使用传递的参数来初始化成员变量
        this.isConnected = isConnected;
        this.ipAddress = ipAddress;
        this.port = port;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
        // 在这里初始化连接信息（默认值）
        isConnected = false;
        ipAddress = "192.168.43.112";
        port = 8888;
    }

    public static ConnectionInfo getInstance() {
        return instance;
    }

    public boolean isConnected() {
        return isConnected;
    }

    public String getIpAddress() {
        return ipAddress;
    }

    public int getPort() {
        return port;
    }

    public void setConnectionInfo(boolean isConnected, String ipAddress, int port) {
        this.isConnected = isConnected;
        this.ipAddress = ipAddress;
        this.port = port;
    }
    public static boolean isReconnectNeeded() {
        return reconnectNeeded;
    }

    public static void setReconnectNeeded(boolean reconnectNeeded) {
        ConnectionInfo.reconnectNeeded = reconnectNeeded;
    }
}
