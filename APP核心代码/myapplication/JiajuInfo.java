package com.example.myapplication;

public class JiajuInfo{
    private int _id;
    private int product_img;
    private String product_title;
    private String product_style;
    private String product_size;
    private String product_price;

    public JiajuInfo(int _id,int product_img, String product_title, String product_style, String product_size, String product_price) {
        this._id = _id;
        this.product_title = product_title;
        this.product_style = product_style;
        this.product_size = product_size;
        this.product_price = product_price;
        this.product_img = product_img;
    }


    public int getProduct_img() {
        return product_img;
    }

    public void setProduct_img(int product_img) {
        this.product_img = product_img;
    }

    public int get_id() {
        return _id;
    }

    public void set_id(int _id) {
        this._id = _id;
    }

    public String getProduct_title() {
        return product_title;
    }

    public void setProduct_title(String product_title) {
        this.product_title = product_title;
    }

    public String getProduct_style() {
        return product_style;
    }

    public void setProduct_style(String product_style) {
        this.product_style = product_style;
    }

    public String getProduct_size() {
        return product_size;
    }

    public void setProduct_size(String product_size) {
        this.product_size = product_size;
    }

    public String getProduct_price() {
        return product_price;
    }

    public void setProduct_price(String product_price) {
        this.product_price = product_price;
    }
}

