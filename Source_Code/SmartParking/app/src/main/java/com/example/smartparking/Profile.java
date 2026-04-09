package com.example.smartparking;

import static android.content.Context.MODE_PRIVATE;

import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class Profile extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_profile, container, false);

        SharedPreferences sp = this.getActivity().getSharedPreferences("credentials", MODE_PRIVATE);
        String username = sp.getString("uname", "User");

        TextView profileName = view.findViewById(R.id.profile_name);
        TextView profileUsernameDetail = view.findViewById(R.id.profile_username_detail);

        profileName.setText(username);
        profileUsernameDetail.setText(username);

        return view;
    }
}
