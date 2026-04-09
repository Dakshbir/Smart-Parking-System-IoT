package com.example.smartparking;

import static android.content.Context.MODE_PRIVATE;

import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class Status extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_status, container, false);
        Button button2;
        button2 = (Button) view.findViewById(R.id.button2);
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                cancle(button2);
            }
        });

        return view;
    }

    public void cancle(View view) {
        SharedPreferences sp= this.getActivity().getSharedPreferences("credentials",MODE_PRIVATE);
        String name = sp.getString("uname","");
        String ip = sp.getString("server_ip","");
        String url = "http://" + ip + "/parking/android_db_pool/status.php";
        String n2 = name;
        String qryString = "?n1=" + n2;
        class dbclass extends AsyncTask<String, Void, String> {

            protected void onPostExecute(String data) {
                Toast.makeText(getActivity(), data, Toast.LENGTH_SHORT).show();
            }

            @Override
            protected String doInBackground(String... strings) {
                try {
                    URL url = new URL(strings[0]);
                    HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                    BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                    return br.readLine();
                } catch (Exception ex) {
                    return ex.getMessage();
                }
            }
        }

        dbclass obj = new dbclass();
        obj.execute(url + qryString);
    }
}