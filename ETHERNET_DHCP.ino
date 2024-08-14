#include <SoftwareSerial.h>
#include <fpm.h>
#include <Arduino.h>
#include <Ethernet2.h>
#include <SPI.h>
#include <base64.h>

SoftwareSerial fserial(4, 2);

FPM finger(&fserial);
FPMSystemParams params;

// var
byte mac[] = { 0x2C, 0xF7, 0xF1, 0x08, 0x00, 0x9A };
char server[] = "192.168.1.4";
EthernetClient client;
#define MAX_UID 5
String readString;
unsigned int tcpServerPort = 9999;
String idfile;
bool konektoserver=false;
/* for convenience */
#define PRINTF_BUF_SZ  60
char printfBuf[PRINTF_BUF_SZ];

#define IMAGE_SZ	36864UL
uint8_t imageBuffer[IMAGE_SZ];
String imgfilenm="";
void setup() {
  Serial.begin(57600);
  Serial.println("Connecting...\n");
  fserial.begin(57600);
  delay(1);
  Ethernet.init(5);
  delay(100);
  int i = 0;
  int DHCP = 0;
  DHCP =Ethernet.begin(mac);
  if (DHCP == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for (;;)
      ;
    while( DHCP == 0 && i < 30){
      delay(1000);
      DHCP = Ethernet.begin(mac);
      i++;
    }
    if(!DHCP){
      Serial.println("DHCP FAILED");
      for(;;); //Infinite loop because DHCP Failed
    }
    Serial.println("DHCP Success");
    // no point in carrying on, so do nothing forevermore:
  }
  else {
      // print your local IP address:
      Serial.print("IP assigned Ethernet ");
      Serial.println("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
      Serial.print("IP Address        : ");
      Serial.println(Ethernet.localIP());
      Serial.print("Subnet Mask       : ");
      Serial.println(Ethernet.subnetMask());
      Serial.print("Default Gateway IP: ");
      Serial.println(Ethernet.gatewayIP());
      Serial.print("DNS Server IP     : ");
      Serial.println(Ethernet.dnsServerIP());
      Serial.println();
      imgfilenm=(Ethernet.localIP().toString());
      imgfilenm.replace(".","");
      imgfilenm.replace(".","");
      imgfilenm.replace(".","");
      imgfilenm=imgfilenm+'_'+GenTXT();
      Serial.print("Nama file : ");
      Serial.println(imgfilenm);
      delay(1000);
      /*
      uint8_t *imageData;
      size_t len = file.size();

      imageData = alloca(len);
      file.readBytes(imageData, len);

      client.write(imageData, len);
      */
      // post data 
      if (client.connect(server, 8888)) {
        String data = "[{'json':'data'}]";
        
        // Make a HTTP request:   
        client.println("POST /matchingfinger HTTP/1.1"); 
        //client.println("Host: "+server);
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(data.length());
        client.println("");
        client.println(data);
        client.println();

        while(client.connected() && !client.available()) delay(1); //waits for data
        while (client.connected() || client.available()) { //connected or data available
            char c = client.read(); //gets byte from ethernet buffer
            Serial.print(c); //prints byte to serial monitor
            delay(1);
        }
        
        Serial.println();
        Serial.println("disconnecting.");
        Serial.println("==================");
        Serial.println();
        //client.stop();
      }

    }

}

void loop() {
  imageToBuffer();
  while (1) yield();
}

uint32_t imageToBuffer(void)
{
    FPMStatus status;
    
    /* Take a snapshot of the finger */
    Serial.println("\r\nPlace a finger.");
    
    do {
        status = finger.getImage();
        
        switch (status) 
        {
            case FPMStatus::OK:
                Serial.println("Image taken.");
                break;
                
            case FPMStatus::NOFINGER:
                Serial.println(".");
                break;
                
            default:
                /* allow retries even when an error happens */
                snprintf(printfBuf, PRINTF_BUF_SZ, "getImage(): error 0x%X", static_cast<uint16_t>(status));
                Serial.println(printfBuf);
                break;
        }
        
        yield();
    }
    while (status != FPMStatus::OK);
    
    /* Initiate the image transfer */
    status = finger.downloadImage();
    
    switch (status) 
    {
        case FPMStatus::OK:
            Serial.println("Starting image stream...");
            break;
            
        default:
            snprintf(printfBuf, PRINTF_BUF_SZ, "downloadImage(): error 0x%X", static_cast<uint16_t>(status));
            Serial.println(printfBuf);
            return 0;
    }
    
    uint32_t totalRead = 0;
    uint16_t readLen = IMAGE_SZ;
    
    /* Now, the sensor will send us the image from its image buffer, one packet at a time */
    bool readComplete = false;

    while (!readComplete) 
    {
        bool ret = finger.readDataPacket(imageBuffer + totalRead, NULL, &readLen, &readComplete);
        
        if (!ret)
        {
            snprintf_P(printfBuf, PRINTF_BUF_SZ, PSTR("readDataPacket(): failed after reading %u bytes"), totalRead);
            Serial.println(printfBuf);
            return 0;
        }
        
        totalRead += readLen;
        readLen = IMAGE_SZ - totalRead;
        
        yield();
    }
    
    Serial.println();
    Serial.print(totalRead); 
    Serial.println(" bytes transferred.");
    String serverName="192.168.1.11";
    String serverPath="/matchingfinger";
    String filenm=serverName;
    filenm.replace(".","");
    filenm.replace(".","");
    filenm.replace(".","");
    filenm=filenm+'_'+GenTXT();
    Serial.println("connected");
    String response = "POST " + serverPath + " HTTP/1.1\r\n";
    response += "Content-Type: image/bmp\r\n";
    response += "Content-Length: " + String(totalRead) + "\r\n";
    client.println("POST /matchingfinger HTTP/1.1"); 
        //client.println("Host: "+server);
    client.println("Content-Type: image/bmp");
    client.print("Content-Length: ");
    client.println(totalRead);
    client.println("");
    //client.println(data);
    client.println();
    Serial.println("conected to the server");
    //client.println(response);
    client.println();

    client.write((uint8_t*)(imageBuffer+totalRead),totalRead);
    client.println();
    delay(100);
    while(client.connected() && !client.available()) delay(1); //waits for data
    while (client.connected() || client.available()) { //connected or data available
      char c = client.read(); //gets byte from ethernet buffer
      Serial.print(c); //prints byte to serial monitor
      delay(1);
    }
    return totalRead;
}
const char * GenTXT(){
  /* Change to allowable characters */
  const char possible[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  static char uid[MAX_UID + 1];
  for(int p = 0, i = 0; i < MAX_UID; i++){
    int r = random(0, strlen(possible));
    uid[p++] = possible[r];
  }
  uid[MAX_UID] = '\0';
  return uid;
}