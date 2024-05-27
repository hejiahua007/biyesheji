package com.example.myapplication;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class LoginActivity extends AppCompatActivity {

    private EditText et_username;
    private EditText et_password;
    private CheckBox checkBox;
    private boolean is_login;
    private SharedPreferences mSharedPreferences;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        //初始化控件
        et_username = findViewById(R.id.et_username);
        et_password = findViewById(R.id.et_password);
        checkBox = findViewById(R.id.checkbox);

        //获取SharedPreferences
        mSharedPreferences = getSharedPreferences("user",MODE_PRIVATE);

        //是否勾选了记住密码
        is_login = mSharedPreferences.getBoolean("is_login",false);
        if (is_login){
            String username = mSharedPreferences.getString("username",null);
            String password = mSharedPreferences.getString("password",null);
            et_username.setText(username);
            et_password.setText(password);
            checkBox.setChecked(true);

        }




        //点击注册
        findViewById(R.id.register).setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                //跳转到注册页面
                Intent intent = new Intent(LoginActivity.this,RegisterActivity.class);
                startActivity(intent);
            }
        });


        findViewById(R.id.login).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String username = et_username.getText().toString();
                String password = et_password.getText().toString();
                if(TextUtils.isEmpty(username) && TextUtils.isEmpty(password)){
                    Toast.makeText(LoginActivity.this,"请输入用户名和密码",Toast.LENGTH_SHORT).show();
                }else{
                    String name = mSharedPreferences.getString("username",null);
                    String pwd = mSharedPreferences.getString("password",null);
                    if(username.equals(name) && password.equals(pwd)){
                        SharedPreferences.Editor edit = mSharedPreferences.edit();
                        edit.putBoolean("is_login",is_login);
                        edit.putString("username",username);
                        edit.putString("password",password);
                        edit.commit();

                        //登录成功
                        Intent intent = new Intent(LoginActivity.this,MainActivity.class);
                        startActivity(intent);
                    }else{
                        //用户名或密码错误
                        Toast.makeText(LoginActivity.this,"用户名或密码错误",Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });


        //checkBox的点击事件
        checkBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                is_login = isChecked;

            }
        });




    }
}