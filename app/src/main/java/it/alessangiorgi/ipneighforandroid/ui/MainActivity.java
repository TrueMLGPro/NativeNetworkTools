package it.alessangiorgi.ipneighforandroid.ui;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import it.alessangiorgi.ipneigh30.ArpNDK;
import it.alessangiorgi.ipneighforandroid.R;

public class MainActivity extends AppCompatActivity {
	TextView arp_table_textview;
//	TextView mac_address_textview;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		arp_table_textview = findViewById(R.id.arp_table);
//		mac_address_textview = findViewById(R.id.mac_addr);
		arp_table_textview.setText(ArpNDK.getARP());
		// mac_address_textview.setText("MAC Address Java: " + getMacAddressJava() + "\n" + "MAC Address NDK: " + ArpNDK.getMacAddrNew());
	}

/*
	public static String getMacAddressJava() {
        try {
            List<NetworkInterface> all = Collections.list(NetworkInterface.getNetworkInterfaces());
            for (NetworkInterface nif : all) {
                if (!nif.getName().equalsIgnoreCase("wlan0")) continue;

                byte[] macBytes = nif.getHardwareAddress();
                if (macBytes == null) {
                    return "";
                }

                StringBuilder res1 = new StringBuilder();
                for (byte b : macBytes) {
                    res1.append(String.format("%02X:",b));
                }

                if (res1.length() > 0) {
                    res1.deleteCharAt(res1.length() - 1);
                }
                return res1.toString();
            }
        } catch (Exception ignored) {
        }
        return "02:00:00:00:00:00";
    }
*/
}