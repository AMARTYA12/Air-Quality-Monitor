package com.example.amartyabanerjee.bluetoothapplication;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.UUID;

public class btConnect extends AppCompatActivity {

    Button SEND;
    TextView  date,textview1;

    BluetoothAdapter myBluetooth;
    BluetoothSocket btSocket;
    String Address = null;
    String Receive;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
    private boolean isBtConnected = false;
    private OutputStream outputStream;
    private InputStream inputStream;
    private ProgressDialog progress;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bt_connect);
        TextView date =(TextView)findViewById(R.id.textView);
        Calendar calendar= Calendar.getInstance();
        SimpleDateFormat sdf= new SimpleDateFormat("dd.MM.yyyy  'at' HH:mm:ss z");
        String cuurentdateandtime= sdf.format(calendar.getTime());
        date.setText(cuurentdateandtime);
        Intent newint = getIntent();
        Address = newint.getStringExtra(MainActivity.EXTRA_ADDRESS);
        SEND = (Button) findViewById(R.id.button4);
        TextView textView=(TextView)findViewById(R.id.textView1);
        new ConnectBT().execute();


        SEND.setOnClickListener(new View.OnClickListener() {
                                    @Override
                                    public void onClick(View v) {
                                        send();
                                    }
                                }
        );
    }



    private void send() {
        Calendar calendar= Calendar.getInstance();
        SimpleDateFormat sdf= new SimpleDateFormat("yyyy.MM.dd  'at' HH:mm:ss z");
        String cuurentdateandtime= sdf.format(calendar.getTime());
        date.setText(cuurentdateandtime);
        if (btSocket != null) {
            try {
                btSocket.getOutputStream().write(cuurentdateandtime.toString().getBytes());
            } catch (IOException e) {
                message("error");
            }

        }
    }

    public void message(String s) {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG).show();
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void> {
        private boolean ConnectSuccess = true;

        @Override
        protected Void doInBackground(Void... voids) {
            try {
                if (btSocket == null || !isBtConnected) {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(Address);
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();
                }
            } catch (IOException e) {
                ConnectSuccess = false;
            }
            return null;
        }

        @Override
        protected void onPreExecute() {
            progress = ProgressDialog.show(btConnect.this, "connecting....", "Please Wait!!");

        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            if (!ConnectSuccess) {
                message("connection failed");
                finish();
            } else {
                message("connected");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }
}

