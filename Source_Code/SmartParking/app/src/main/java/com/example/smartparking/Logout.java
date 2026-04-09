package com.example.smartparking;

import static android.content.Context.MODE_PRIVATE;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

public class Logout extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_logout, container, false);

        view.findViewById(R.id.btn_logout).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                logout_process(v);
            }
        });

        return view;
    }
    public void logout_process(View view)
    {
        SharedPreferences sp= this.getActivity().getSharedPreferences("credentials",MODE_PRIVATE);
        if(sp.contains("uname"))
        {
            SharedPreferences.Editor editor=sp.edit();
            editor.remove("uname");

            editor.putString("msg","Logged out Successfully");
            editor.commit();
            Intent intent = new Intent(getActivity(),MainActivity.class);
            startActivity(intent);
        }

    }
}