package com.example.myapplication;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;
import java.util.List;

public class JiajuListAdapter extends RecyclerView.Adapter<JiajuListAdapter.MyHolder> {
    private List<JiajuInfo> mJiajuInfos = new ArrayList<>();
    private static OnItemClickListener itemClickListener;

    public JiajuListAdapter(List<JiajuInfo> list) {
        this.mJiajuInfos = list;
    }

    @NonNull
    @Override
    public MyHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.jiaju_item, parent, false);
        return new MyHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MyHolder holder, int position) {
        JiajuInfo jiajuInfo = mJiajuInfos.get(position);

        holder.tv_title.setText(jiajuInfo.getProduct_title());
        holder.tv_style.setText("风格：" + jiajuInfo.getProduct_style());
        holder.tv_size.setText("尺寸：" + jiajuInfo.getProduct_size());
        holder.tv_price.setText("¥ " + jiajuInfo.getProduct_price());
        holder.img_product.setImageResource(jiajuInfo.getProduct_img());
    }

    @Override
    public int getItemCount() {
        return mJiajuInfos.size();
    }

    static class MyHolder extends RecyclerView.ViewHolder implements View.OnClickListener, View.OnLongClickListener {
        TextView tv_title;
        TextView tv_style;
        TextView tv_size;
        TextView tv_price;
        ImageView img_product;

        public MyHolder(@NonNull View itemView) {
            super(itemView);
            tv_title = itemView.findViewById(R.id.tv_title);
            tv_style = itemView.findViewById(R.id.tv_style);
            tv_size = itemView.findViewById(R.id.tv_size);
            tv_price = itemView.findViewById(R.id.tv_price);
            img_product = itemView.findViewById(R.id.img_product);

            itemView.setOnClickListener(this);
            itemView.setOnLongClickListener(this);
        }

        @Override
        public void onClick(View view) {
            if (itemClickListener != null) {
                itemClickListener.onItemClick(getAbsoluteAdapterPosition());
            }
        }

        @Override
        public boolean onLongClick(View view) {
            if (itemClickListener != null) {
                itemClickListener.onItemLongClick(getAbsoluteAdapterPosition());
                return true;
            }
            return false;
        }
    }

    public interface OnItemClickListener {
        void onItemClick(int pos);
        void onItemLongClick(int pos);
    }

    public void setOnItemClickListener(OnItemClickListener listener) {
        this.itemClickListener = listener;
    }
}
