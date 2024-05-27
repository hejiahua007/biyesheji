package com.example.myapplication;

public class UserModel {
    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public String getWendu() {
        return wendu;
    }

    public void setWendu(String wendu) {
        this.wendu = wendu;
    }

    public String getShidu() {
        return shidu;
    }

    public void setShidu(String shidu) {
        this.shidu = shidu;
    }

    public String getGuangzhao() {
        return guangzhao;
    }

    public void setGuangzhao(String guangzhao) {
        this.guangzhao = guangzhao;
    }

    public String getCo2() {
        return co2;
    }

    public void setCo2(String co2) {
        this.co2 = co2;
    }

    public String getShengxiang() {
        return shengxiang;
    }

    public void setShengxiang(String shengxiang) {
        this.shengxiang = shengxiang;
    }

    public String getFengli() {
        return fengli;
    }

    public void setFengli(String fengli) {
        this.fengli = fengli;
    }

    private String time;
    private String wendu;
    private String shidu;
    private String guangzhao;
    private String co2;
    private String shengxiang;
    private String fengli;
    public UserModel() {
        this.time = "时间";
        this.wendu = "温度";
        this.shidu = "湿度";
        this.guangzhao = "光照";
        this.co2 = "气体";
        this.shengxiang = "声响";
        this.fengli = "风力";
    }
    public UserModel(String time, String wendu, String shidu, String guangzhao, String co2, String shengxiang, String fengli) {
        this.time = time;
        this.wendu = wendu;
        this.shidu = shidu;
        this.guangzhao = guangzhao;
        this.co2 = co2;
        this.shengxiang = shengxiang;
        this.fengli = fengli;
    }
}
