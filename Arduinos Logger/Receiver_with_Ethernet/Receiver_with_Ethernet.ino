#include <UIPEthernet.h> // Used for Ethernet
#include <Wire.h>

// **** ETHERNET SETTING ****
// Arduino Uno pins: 10 = CS, 11 = MOSI, 12 = MISO, 13 = SCK
// Ethernet MAC address - must be unique on your network - MAC Reads T4A001 in hex (unique in your network)
byte mac[] = { 0x14, 0x2D, 0x27, 0xA5, 0xE4, 0xD5 };                                       
// For the rest we use DHCP (IP address and such)

EthernetClient client;
char server[] = "192.168.2.8"; // IP Address (or name) of server to dump data to
int  interval = 5000; // Wait between dumps
IPAddress myIP(192,168,2,8); // client Adress

int x,i,k;
long first, second;
char pathToPHPFile[] = "/RfidLogger/logger.php?";


void setup() {

  Serial.begin(9600);
  Ethernet.begin(mac,myIP);
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  
  x = -1; i = 0; k = 1; first = 0; second = 0;

  Serial.println("ifconfig - Schaool Logger - v1.1");
  Serial.println("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  Serial.print("IP Address        : ");
  Serial.println(Ethernet.localIP());
  Serial.print("Subnet Mask       : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Default Gateway IP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS Server IP     : ");
  Serial.println(Ethernet.dnsServerIP());
  
  client.connect(server, 80); // without this cannot capture first use log!!!

}

void loop() {
  // if you get a connection, report back via serial:
  if(x > 0)
  {
    delay(2);
   // Serial.print(x);
    if(k<6)
      first = first * 10 + x;
    else
      second = second * 10 + x;
    if(k > 10) 
    {
      k = 0;
      Serial.print(first); 
      Serial.println(second);
      
      if (1) {
        // Make a HTTP request:
        client.print( "GET ");
        client.print( pathToPHPFile);
        client.print("first=");
        client.print( first );
        client.print("&&");
        client.print("second=");
        client.print( second );
        client.println( " HTTP/1.1");
        client.print( "Host: " );
        client.println(server);
        client.println( "Connection: close" );
        client.println();
        client.println();
        client.stop();
      }
      if(client.connect(server, 80))
        Serial.println("-> Connected");
      else {
        // you didn't get a connection to the server:
        Serial.println("--> connection failed/n");
      }
      first = 0; second = 0;
        
    }
    x = -1;
    k++;
    
  }
}

void receiveEvent(int howMany) {
  x = Wire.read();    // receive byte as an integer
}
