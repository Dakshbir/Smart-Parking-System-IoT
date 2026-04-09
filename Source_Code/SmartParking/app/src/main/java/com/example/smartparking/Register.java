package com.example.smartparking;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class Register extends AppCompatActivity {
    Button R_button;
    TextView textView3;
    EditText Name, Phone_no, Email_id, Number_plate, R_password;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);
        Name = (EditText) findViewById(R.id.Name);
        Phone_no = (EditText) findViewById(R.id.Phone_no);
        Email_id = (EditText) findViewById(R.id.Email_id);
        Number_plate = (EditText) findViewById(R.id.Number_plate);
        R_password = (EditText) findViewById(R.id.R_Password);
        R_button = (Button) findViewById(R.id.R_button);
        textView3 = (TextView) findViewById(R.id.textView3);

        R_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String username = Name.getText().toString();
                String phone_no = Phone_no.getText().toString();
                String email_id = Email_id.getText().toString();
                String number_plate = Number_plate.getText().toString();
                String password = R_password.getText().toString();

                if (!username.equals("") && !phone_no.equals("") && !email_id.equals("") && !number_plate.equals("") && !password.equals("")){
                    process(view);
                }
                else{
                    Toast.makeText(Register.this, "Please Enter all the details.", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    public void process(View view){
        Intent login = new Intent(Register.this, MainActivity.class);
        String R_name = Name.getText().toString();
        String R_Phone_no = Phone_no.getText().toString();
        String R_Email_id = Email_id.getText().toString();
        String R_Number_plate = Number_plate.getText().toString();
        String Pass = R_password.getText().toString();
        SharedPreferences sp = getSharedPreferences("credentials", MODE_PRIVATE);
        String ip = sp.getString("server_ip", "");
        String url = "http://" + ip + "/parking/LogIn-SignUp-master/register.php";

        Toast.makeText(getApplicationContext(), R_Email_id, Toast.LENGTH_SHORT).show();

        String qryString="?R_name="+R_name+"&R_Phone_no="+ R_Phone_no+"&R_Email_id="+R_Email_id+"&R_Number_plate="+R_Number_plate+"&Pass="+Pass;
        class dbclass extends AsyncTask<String,Void,String> {

            protected void onPostExecute(String data){
                Name.setText("");
                Phone_no.setText("");
                Email_id.setText("");
                Number_plate.setText("");
                R_password.setText("");
                textView3.setText(data);
                startActivity(new Intent(getApplicationContext(), MainActivity.class));
            }

            @Override
            protected String doInBackground(String... strings) {
                try{
                    URL url = new URL(strings[0]);
                    HttpURLConnection conn =(HttpURLConnection) url.openConnection();
                    BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                    return br.readLine();
                }catch(Exception ex)
                {
                    return ex.getMessage();
                }
            }
        }

        dbclass obj = new dbclass();
        obj.execute(url+qryString);
    }
}