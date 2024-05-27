package com.example.myapplication;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class WenduActivity extends AppCompatActivity {

    private EditText et_username;
    private EditText et_password;

    private SharedPreferences mSharedPreferences;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        //获取mSharedPreferences
        mSharedPreferences = getSharedPreferences("user",MODE_PRIVATE);

        et_username = findViewById(R.id.et_username);
        et_password = findViewById(R.id.et_password);
        //点击返回
        findViewById(R.id.toolbar).setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                //返回
                finish();
            }
        });

        findViewById(R.id.register).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String username = et_username.getText().toString();
                String password = et_password.getText().toString();

                if(TextUtils.isEmpty(username) || TextUtils.isEmpty(password)){
                    Toast.makeText(WenduActivity.this,"请输入用户名和密码",Toast.LENGTH_SHORT).show();
                }else{
                    SharedPreferences.Editor edit = mSharedPreferences.edit();
                    edit.putString("username",username);
                    edit.putString("password",password);
                    edit.commit();
                    Toast.makeText(WenduActivity.this,"注册成功，请登录",Toast.LENGTH_SHORT).show();
                    finish();
                }
            }
        });



    }

}
