package com.example.smartparking;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class MainActivity extends AppCompatActivity {
    Button b_login, b_register;
    EditText Username, Password, ServerIP;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.login);
        ServerIP = (EditText) findViewById(R.id.ServerIP);
        Username = (EditText) findViewById(R.id.Username);
        Password = (EditText) findViewById(R.id.Password);

        // Load previously saved IP
        SharedPreferences sp = getSharedPreferences("credentials", MODE_PRIVATE);
        String savedIP = sp.getString("server_ip", "");
        if (!savedIP.isEmpty()) {
            ServerIP.setText(savedIP);
        }
        b_login = (Button) findViewById(R.id.b_login);
        b_login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                login_process(b_login);
            }
        });

        b_register = (Button) findViewById(R.id.b_register);
        b_register.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String ip = ServerIP.getText().toString().trim();
                if (ip.isEmpty()) {
                    Toast.makeText(MainActivity.this, "Please enter Server IP first", Toast.LENGTH_SHORT).show();
                    return;
                }
                // Save IP before going to Register so it can use it
                SharedPreferences sp = getSharedPreferences("credentials", MODE_PRIVATE);
                sp.edit().putString("server_ip", ip).commit();
                Intent intent = new Intent(MainActivity.this, Register.class);
                startActivity(intent);
            }
        });
    }

    public void login_process(View view)
    {
        Username=(EditText)findViewById(R.id.Username);
        Password=(EditText)findViewById(R.id.Password);
        ServerIP=(EditText)findViewById(R.id.ServerIP);

        String ip = ServerIP.getText().toString().trim();
        if (ip.isEmpty()) {
            Toast.makeText(MainActivity.this, "Please enter Server IP", Toast.LENGTH_SHORT).show();
            return;
        }

        String apiurl = "http://" + ip + "/parking/android_db_pool/login_maker.php";
        String qry="?t1="+Username.getText().toString().trim()+"&t2="+Password.getText().toString().trim();

        class dbprocess extends AsyncTask<String,Void,String>
        {
            @Override
            protected  void onPostExecute(String data)
            {
                Toast.makeText(MainActivity.this, data, Toast.LENGTH_SHORT).show();
                if(data.equals("found"))
                {
                    SharedPreferences sp=getSharedPreferences("credentials",MODE_PRIVATE);
                    SharedPreferences.Editor editor=sp.edit();
                    editor.putString("uname",Username.getText().toString());
                    editor.putString("server_ip", ip);
                    editor.commit();
                    startActivity(new Intent(getApplicationContext(), Dashboard.class));
                }
                else
                {
                    Username.setText("");
                    Password.setText("");
                    Toast.makeText(MainActivity.this, "Wrong Username or Password", Toast.LENGTH_SHORT).show();
                }
            }
            @Override
            protected String doInBackground(String... params)
            {
                String furl=params[0];

                try
                {
                    URL url=new URL(furl);
                    HttpURLConnection conn=(HttpURLConnection)url.openConnection();
                    BufferedReader br=new BufferedReader(new InputStreamReader(conn.getInputStream()));

                    return br.readLine();

                }catch (Exception ex)
                {
                    return ex.getMessage();
                }
            }
        }

        dbprocess obj=new dbprocess();
        obj.execute(apiurl+qry);

    }

    public void checklogoutmsg(View view)
    {

        SharedPreferences sp=getSharedPreferences("credentials",MODE_PRIVATE);
        if(sp.contains("msg"))
        {
            Toast.makeText(getApplicationContext(), (sp.getString("msg", "")), Toast.LENGTH_SHORT).show();
            SharedPreferences.Editor ed=sp.edit();
            ed.remove("msg");
            ed.commit();
        }
    }
}