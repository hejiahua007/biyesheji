package com.example.myapplication;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public class RecyclerViewAdapter extends RecyclerView.Adapter<RecyclerViewAdapter.MyViewHolder> {
    Context context;
    ArrayList<UserModel> userList;
    public RecyclerViewAdapter(Context context, ArrayList<UserModel> userList) {
        this.context=context;
        this.userList=userList;
    }

    @NonNull
    @Override
    public RecyclerViewAdapter.MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view= LayoutInflater.from(this.context).inflate(R.layout.recyclerview_item,parent,false);
        return new MyViewHolder(view);

    }

    @Override
    public void onBindViewHolder(@NonNull RecyclerViewAdapter.MyViewHolder holder, int position) {

        // 更新MyViewHolder中的TextView以显示相应的信息
        holder.timeTextView.setText(userList.get(position).getTime());
        holder.wenduTextView.setText(userList.get(position).getWendu());
        holder.shiduTextView.setText(userList.get(position).getShidu());
        holder.guangzhaoTextView.setText(userList.get(position).getGuangzhao());
        holder.co2TextView.setText(userList.get(position).getCo2());
        holder.shengxiangTextView.setText(userList.get(position).getShengxiang());
        holder.fengliTextView.setText(userList.get(position).getFengli());
    }

    @Override
    public int getItemCount() {
        return this.userList.size();
    }

    public static class MyViewHolder extends RecyclerView.ViewHolder{
        TextView timeTextView;
        TextView wenduTextView;
        TextView shiduTextView;
        TextView guangzhaoTextView;
        TextView co2TextView;
        TextView shengxiangTextView;
        TextView fengliTextView;
        public MyViewHolder(@NonNull View itemView) {
            super(itemView);
            timeTextView = itemView.findViewById(R.id.itemtext_time);
            wenduTextView = itemView.findViewById(R.id.itemtext_wendu);
            shiduTextView = itemView.findViewById(R.id.itemtext_shidu);
            guangzhaoTextView = itemView.findViewById(R.id.itemtext_guangzhao);
            co2TextView = itemView.findViewById(R.id.itemtext_co2);
            shengxiangTextView = itemView.findViewById(R.id.itemtext_shengxiang);
            fengliTextView = itemView.findViewById(R.id.itemtext_fengli);
        }
    }
}
