package com.example.amartyabanerjee.bluetoothapplication;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {


    private final String DEVICE_ADDRESS="20.16.02.29.75.96";
    private final UUID PORT_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
    private BluetoothDevice device;
    private BluetoothSocket socket;
    private OutputStream outputStream;
    private InputStream inputStream;
    Button OFF;
    Button LIST;
    Button SCAN;
    ListView lv;
    TextView  date;
    ListView Lv1;
    private BluetoothAdapter Ba;
    private Set<BluetoothAdapter> pairedDevices;
    TextView textView;
    public static String EXTRA_ADDRESS ="device_address";
    ArrayList<String> stringArrayList = new ArrayList<String>();
    ArrayAdapter<String>arrayAdapter;
    BluetoothAdapter myAdapter= BluetoothAdapter.getDefaultAdapter();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        OFF = (Button) findViewById(R.id.button);
        LIST = (Button) findViewById(R.id.button2);
        SCAN = (Button) findViewById(R.id.button3);
        lv = (ListView) findViewById(R.id.ListView);

        Lv1 = (ListView) findViewById(R.id.ListView1);
        TextView date = (TextView) findViewById(R.id.textview);
        Calendar calendar = Calendar.getInstance();
        SimpleDateFormat sdf = new SimpleDateFormat("dd.MM.yyyy  'at' HH:mm:ss z");
        String cuurentdateandtime = sdf.format(calendar.getTime());
        date.setText(cuurentdateandtime);
        Ba = BluetoothAdapter.getDefaultAdapter();
        if (!Ba.isEnabled()) {
            Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnOn, 0);
            Toast.makeText(getApplicationContext(), "turned ON", Toast.LENGTH_LONG).show();


        } else {
            Toast.makeText(getApplicationContext(), "already ON", Toast.LENGTH_LONG).show();
        }

        LIST.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                pairedDevices();
            }
        });
        OFF.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                btnoff();
            }
        });
        SCAN.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                myAdapter.startDiscovery();

            }
        });
        IntentFilter intentFilter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(receiver,intentFilter);
        arrayAdapter = new ArrayAdapter<String>(getApplicationContext(),android.R.layout.simple_list_item_1,stringArrayList);
        Lv1.setAdapter(arrayAdapter);
        Lv1.setOnItemClickListener(myListclicklistener);
    }
    BroadcastReceiver receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                // Discovery has found a device. Get the BluetoothDevice
                // object and its info from the Intent.
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                stringArrayList.add(device.getName()+" "+device.getAddress());
                arrayAdapter.notifyDataSetChanged();// MAC address
            }


        }
    };


    private  void btnoff(){
        Ba.disable();
        Toast.makeText(getApplicationContext(),"Turned OFF",Toast.LENGTH_LONG).show();

    }

    private void pairedDevices() {
        Set<BluetoothDevice> pairedDevices = Ba.getBondedDevices();
        ArrayList list = new ArrayList();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice bt : pairedDevices) {
                list.add(bt.getName()+" "+bt.getAddress());

                Toast.makeText(getApplicationContext(), "showing paired devies", Toast.LENGTH_LONG).show();
            }
        } else {
            Toast.makeText(getApplicationContext(), "no pairing devices found", Toast.LENGTH_LONG).show();
        }
        final ArrayAdapter adapter = new ArrayAdapter(getApplicationContext(), android.R.layout.simple_list_item_1, list);
        lv.setAdapter(adapter);
        lv.setOnItemClickListener(myListclicklistener);
    }


    private AdapterView.OnItemClickListener myListclicklistener=new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            String info[] =((TextView)view ).getText().toString().split(" ");
            String address = info[1];
            Intent i=new Intent(MainActivity.this,btConnect.class);
            i.putExtra(EXTRA_ADDRESS,address);
            startActivity(i);



        }
    };
    }

