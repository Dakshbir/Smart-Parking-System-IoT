package com.example.smartparking;

import static android.content.Context.MODE_PRIVATE;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import androidx.fragment.app.Fragment;


public class Home extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment

        View view =  inflater.inflate(R.layout.fragment_home, container, false);

        ImageButton parking, status, cancel, profile;
        TextView tv;

        tv = (TextView) view.findViewById(R.id.tv);

        SharedPreferences sp= this.getActivity().getSharedPreferences("credentials",MODE_PRIVATE);
        if(sp.contains("uname"))
            tv.setText(sp.getString("uname",""));

        parking = (ImageButton) view.findViewById(R.id.parking);
        parking.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                getFragmentManager().beginTransaction().replace(R.id.container,new Parking()).commit();
            }
        });

        status = (ImageButton) view.findViewById(R.id.status);
        status.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                getFragmentManager().beginTransaction().replace(R.id.container,new Status()).commit();
            }
        });

        cancel = (ImageButton) view.findViewById(R.id.cancle);
        cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                getFragmentManager().beginTransaction().replace(R.id.container, new CancelParking()).commit();
            }
        });

        profile = (ImageButton) view.findViewById(R.id.profile);
        profile.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                getFragmentManager().beginTransaction().replace(R.id.container, new Profile()).commit();
            }
        });
        return view;

    }
}