package com.example.smartparking;

import static android.content.Context.MODE_PRIVATE;

import android.content.SharedPreferences;
import android.media.Image;
import android.os.AsyncTask;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class Parking extends Fragment {
    private static final String url_slot1 = "http://10.100.70.36/parking/LogIn-SignUp-master/Slot_1.php";
    private static final String url_slot2 = "http://10.100.70.36/parking/LogIn-SignUp-master/Slot_2.php";
    private static final String url_slot3 = "http://10.100.70.36/parking/LogIn-SignUp-master/Slot_3.php";
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment

        SharedPreferences sp= this.getActivity().getSharedPreferences("credentials",MODE_PRIVATE);
        if(sp.contains("uname"))
            Toast.makeText(getActivity(), sp.getString("uname",""), Toast.LENGTH_SHORT).show();

        View view =inflater.inflate(R.layout.fragment_parking, container, false);
        ImageButton slot1,slot2,slot3;

        slot1 = (ImageButton) view.findViewById(R.id.slot1);
        slot1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                slot1(slot1);
            }
        });

        slot2 = (ImageButton) view.findViewById(R.id.slot2);
        slot2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                slot2(slot2);
            }
        });
        slot3 = (ImageButton) view.findViewById(R.id.slot3);
        slot3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                slot3(slot3);
            }
        });

        return view;
    }

    public void slot1(View view) {
        /*String n1 = t1.getText().toString();
        String n2 = t2.getText().toString();*/
        SharedPreferences sp= this.getActivity().getSharedPreferences("credentials",MODE_PRIVATE);
        String name = sp.getString("uname","");
        String n2 = name;
        String qryString = "?n1=" + n2;
        class dbclass extends AsyncTask<String, Void, String> {

            protected void onPostExecute(String data) {
                /*t1.setText("");
                t2.setText("");
                textView.setText(data);*/
                if(data.equals("full")) {
                    Toast.makeText(getActivity(), data, Toast.LENGTH_SHORT).show();
                    //getData();
                }
                else{
                    Toast.makeText(getActivity(), data, Toast.LENGTH_SHORT).show();
                }
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
        obj.execute(url_slot1 + qryString);
    }

    public void slot2(View view) {
        /*String n1 = t1.getText().toString();
        String n2 = t2.getText().toString();*/
        SharedPreferences sp= this.getActivity().getSharedPreferences("credentials",MODE_PRIVATE);
        String name = sp.getString("uname","");
        String n2 = name;
        String qryString = "?n1=" + n2;
        class dbclass extends AsyncTask<String, Void, String> {

            protected void onPostExecute(String data) {
                /*t1.setText("");
                t2.setText("");
                textView.setText(data);*/
                if(data.equals("full")) {
                    Toast.makeText(getActivity(), data, Toast.LENGTH_SHORT).show();
                    //getData();
                }
                else{
                    Toast.makeText(getActivity(), data, Toast.LENGTH_SHORT).show();
                }
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
        obj.execute(url_slot2 + qryString);
    }

    public void slot3(View view) {
        /*String n1 = t1.getText().toString();
        String n2 = t2.getText().toString();*/
        SharedPreferences sp= this.getActivity().getSharedPreferences("credentials",MODE_PRIVATE);
        String name = sp.getString("uname","");
        String n2 = name;
        String qryString = "?n1=" + n2;
        class dbclass extends AsyncTask<String, Void, String> {

            protected void onPostExecute(String data) {
                /*t1.setText("");
                t2.setText("");
                textView.setText(data);*/
                if(data.equals("full")) {
                    Toast.makeText(getActivity(), data, Toast.LENGTH_SHORT).show();
                    //getData();
                }
                else{
                    Toast.makeText(getActivity(), data, Toast.LENGTH_SHORT).show();
                }
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
        obj.execute(url_slot3 + qryString);
    }
}