
// Include the RTC library
#include "RTC.h"

// Include the NTP library
#include <NTPClient.h>


#include <WiFiS3.h>

#include <WiFiUdp.h>

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int wifiStatus = WL_IDLE_STATUS;
WiFiUDP Udp; // a UDP instance to let us send and recieve packets over UDP
NTPClient timeClient(Udp);

void get_network_time() {
  connectToWiFi();
  

  Serial.println("\nStarting connection to server...");
  timeClient.begin();
  timeClient.update();

  // Get the current date/time from an NTP server and convert to appropriate time zone
  auto timeZoneOffsetHours = 10.5; 
  // This needs replacing. 
  /* 
  1. On intial run, set region (like when installing an OS). (This can be hard coded until there's an interface to handle that input)
  2. Based on region, retrieve the DST start/end rules (tz database?)
  3. Get real UTC fron NTP, and compare with DST start/end rules to determine current timezone offset for display purpose.
  */
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
  Serial.print("Unix time = ");
  Serial.println(unixTime);

  RTC.begin();

  RTCTime timeToSet = RTCTime(unixTime);
  RTC.setTime(timeToSet);
  delay(50);
  // Retrieve the date/time from RTC and print
  RTCTime currentTime;
  RTC.getTime(currentTime);
  Serial.println(currentTime);
  Serial.println("The RTC was just set to: " + String(currentTime));
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connectToWiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (wifiStatus != WL_CONNECTED) {
    Serial.print("Attempting to connnect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    wifiStatus = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to WiFi");
  printWifiStatus();
}
