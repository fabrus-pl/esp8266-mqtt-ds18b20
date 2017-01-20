#ifndef GLOBALS_H
#define GLOBALS_H

ESP8266WebServer server(80);                        // Create web server instance
boolean firstStart = true;                          // On firststart = true, NTP will try to get a valid time
boolean AdminTimeOutCounter = 0;                    // Counter for disabling Admin mode
strDateTime DateTime;                               // Global DateTime structure, will be refreshed every Second
WiFiUDP UDPNTPClient;                               // NTP Client
unsigned long UnixTimestamp = 0;                    // GLOBALTIME  ( Will be set by NTP)
boolean Refresh = true;                             // For Main Loop, to refresh things like GPIO / WS2812
int cNTP_Update = 0;                                // Counter for Updating the time via NTP
Ticker tkSecond;                                    // Second - Timer for Updating Datetime Structure
boolean AdminEnabled = true;                        // Enable admin for a certain time
byte Minute_Old = 100;                              // Helpvariable for checking, when a new Minute comes up (for Auto Turn On / Off)
boolean BypassOn = false;                           // Toggle on/off via web interface
int count = 0;                                      // Push Button press time counter
const long connectionCheckInterval = 90000;         // MQTT connection checked every 90 seconds
long previousMillis = 0;
const int NTP_PACKET_SIZE = 48;                     // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE];                // Buffer to hold incoming and outgoing packets
int cb;                                             // NTP packet check store
unsigned int localNtpPort = 2390;                   // Set the incomming UDP port to listen to
boolean devStat = false;                            // Timer status condition
WiFiClient wclient;                                 // Create WiFi client instance



#define LED 14                                      // Device output pin definition, yep I know it's defined as LED, cause that's what I was using to test.

struct strConfig 
  {
    String WiFissid;
    String WiFipass;
    byte IP[4];
    byte Netmask[4];
    byte Gateway[4];
    boolean dhcp;
    String ntpServerName;
    long Update_Time_Via_NTP_Every;
    long timezone;
    boolean daylight;
    String DeviceName;
    boolean AutoTurnOff;
    boolean AutoTurnOn;
    byte TurnOnHour;
    byte TurnOnMinute;
    byte TurnOffHour;
    byte TurnOffMinute;
    String MQTTBroker;                  // MQTT broker parameters URL
    long MQTTport;                      // MQTT port
    String MQTTuser;                    // MQTT username
    String MQTTpass;                    // MQTT password
    String clientID;                    // MQTT client ID - this device
  } config;


PubSubClient client(wclient);            // Partially initialise PubSubClient we'll do the remainder in void setup

void WriteConfig()
{

  Serial.println("Writing Config");
  EEPROM.write(0,'C');
  EEPROM.write(1,'F');
  EEPROM.write(2,'G');

  EEPROM.write(16,config.dhcp);
  EEPROM.write(17,config.daylight);
  
  EEPROMWritelong(18,config.Update_Time_Via_NTP_Every); // 4 Byte

  EEPROMWritelong(22,config.timezone);  // 4 Byte
  EEPROMWritelong(26,config.MQTTport);  // 4 Byte
  
  EEPROM.write(32,config.IP[0]);
  EEPROM.write(33,config.IP[1]);
  EEPROM.write(34,config.IP[2]);
  EEPROM.write(35,config.IP[3]);

  EEPROM.write(36,config.Netmask[0]);
  EEPROM.write(37,config.Netmask[1]);
  EEPROM.write(38,config.Netmask[2]);
  EEPROM.write(39,config.Netmask[3]);

  EEPROM.write(40,config.Gateway[0]);
  EEPROM.write(41,config.Gateway[1]);
  EEPROM.write(42,config.Gateway[2]);
  EEPROM.write(43,config.Gateway[3]);

  WriteStringToEEPROM(64,config.WiFissid);
  WriteStringToEEPROM(96,config.WiFipass);
  WriteStringToEEPROM(128,config.ntpServerName);

  WriteStringToEEPROM(160,config.MQTTuser);
  WriteStringToEEPROM(192,config.MQTTpass);
  WriteStringToEEPROM(224,config.clientID);
  WriteStringToEEPROM(256,config.MQTTBroker);  

  EEPROM.write(384,config.AutoTurnOn);
  EEPROM.write(385,config.AutoTurnOff);
  EEPROM.write(386,config.TurnOnHour);
  EEPROM.write(387,config.TurnOnMinute);
  EEPROM.write(388,config.TurnOffHour);
  EEPROM.write(389,config.TurnOffMinute);
  WriteStringToEEPROM(390,config.DeviceName);
  
  EEPROM.commit();                             

}


boolean ReadConfig()
{

  Serial.println("Reading Configuration");
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
  {
    Serial.println("Configuration Found!");
    config.dhcp =   EEPROM.read(16);
    config.daylight = EEPROM.read(17);
    config.Update_Time_Via_NTP_Every = EEPROMReadlong(18); // 4 Byte

    config.timezone = EEPROMReadlong(22); // 4 Byte
    config.MQTTport = EEPROMReadlong(26); // 4 Byte
    
    config.IP[0] = EEPROM.read(32);
    config.IP[1] = EEPROM.read(33);
    config.IP[2] = EEPROM.read(34);
    config.IP[3] = EEPROM.read(35);
    config.Netmask[0] = EEPROM.read(36);
    config.Netmask[1] = EEPROM.read(37);
    config.Netmask[2] = EEPROM.read(38);
    config.Netmask[3] = EEPROM.read(39);
    config.Gateway[0] = EEPROM.read(40);
    config.Gateway[1] = EEPROM.read(41);
    config.Gateway[2] = EEPROM.read(42);
    config.Gateway[3] = EEPROM.read(43);
    config.WiFissid = ReadStringFromEEPROM(64);
    config.WiFipass = ReadStringFromEEPROM(96);
    config.ntpServerName = ReadStringFromEEPROM(128);
    config.MQTTuser = ReadStringFromEEPROM(160);
    config.MQTTpass = ReadStringFromEEPROM(192);
    config.clientID = ReadStringFromEEPROM(224);
    config.MQTTBroker = ReadStringFromEEPROM(256);    
    
    config.AutoTurnOn = EEPROM.read(384);
    config.AutoTurnOff = EEPROM.read(385);
    config.TurnOnHour = EEPROM.read(386);
    config.TurnOnMinute = EEPROM.read(387);
    config.TurnOffHour = EEPROM.read(388);
    config.TurnOffMinute = EEPROM.read(389);
    config.DeviceName= ReadStringFromEEPROM(390);
    return true;
    
  }
  else
  {
    Serial.println("Configuration NOT FOUND!!!!");
    return false;
  }
}


void ConfigureWifi()
  {
  Serial.println("Configuring WiFi "); //Serial.print(config.WiFissid);
  WiFi.begin (config.WiFissid.c_str(), config.WiFipass.c_str());
    if (!config.dhcp)
       {
        WiFi.config(IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3] ),  IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ) , IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
       }
     int retries = 10;
       while ((WiFi.status() != WL_CONNECTED) && retries--) 
       {
       delay(1000);
       Serial.print(".");
       }
       if (WiFi.status() != WL_CONNECTED)
       {
         Serial.println("Unable to connect to WiFi network....!");
       }
     Serial.println("");
     Serial.print("WiFi connected to IP Address: "); Serial.println(WiFi.localIP());
  }
  

  
void ConnectMQTT()
 {
   if (WiFi.status() == WL_CONNECTED)
    {
    Serial.println("");
    Serial.print("Connecting to MQTT Broker:"); //Serial.print(config.MQTTBroker); 
    int retries = 4;
      while (!client.connect(config.clientID.c_str(), config.MQTTuser.c_str(), config.MQTTpass.c_str()) && retries-- )
      {
      delay(500);
      Serial.print(".");
      }
        if(client.connected())                   // Connected to MQTT server
        {
        Serial.println("");
        Serial.println("");
        Serial.print("MQTT Client: ");
        Serial.print(config.clientID);
        Serial.print(": is connected to MQTT server: ");
        Serial.println(config.MQTTBroker);
        client.publish("feedback", "hello from esp8266");
        Serial.println("");
        client.subscribe("device");
        Serial.println("Subscribed to: 'device'");
        Serial.println("");
        } 
        else    
        {
        Serial.println("\nfatal: MQTT server Connection failed");
        }
     }
 }



void NTPRefresh() 
  {
  if (WiFi.status() == WL_CONNECTED)
    {
      IPAddress timeServerIP; 
      WiFi.hostByName(config.ntpServerName.c_str(), timeServerIP);
      Serial.println("");
      Serial.print("sending NTP packet to  "); Serial.print(timeServerIP);
      memset(packetBuffer, 0, NTP_PACKET_SIZE);
      packetBuffer[0] = 0b11100011;                 // LI, Version, Mode
      packetBuffer[1] = 0;                          // Stratum, or type of clock
      packetBuffer[2] = 6;                          // Polling Interval
      packetBuffer[3] = 0xEC;                       // Peer Clock Precision
      packetBuffer[12]  = 49;
      packetBuffer[13]  = 0x4E;
      packetBuffer[14]  = 49;
      packetBuffer[15]  = 52;
      int retries = 5;
       do
        { 
        UDPNTPClient.beginPacket(timeServerIP, 123); 
        UDPNTPClient.write(packetBuffer, NTP_PACKET_SIZE);
        UDPNTPClient.endPacket();
        delay(1000);
        Serial.print(".");
        cb = UDPNTPClient.parsePacket();
        if (!cb)                                            // I dont know why but sometimes we have to restart UDPNTP again and re-write the port
            {
            UDPNTPClient.begin(localNtpPort);
            } 
        } while (!cb && retries--);
          if (!cb)
            {
            Serial.println("");
            Serial.println("Unable to get NTP packet.....!");
            }
        else
        {
        Serial.println("");
        Serial.print("NTP packet received, length = ");
        Serial.println(cb);
        UDPNTPClient.read(packetBuffer, NTP_PACKET_SIZE);                   // read the packet into the buffer
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        const unsigned long seventyYears = 2208988800UL;
        unsigned long epoch = secsSince1900 - seventyYears;
        UnixTimestamp = epoch;
        }
     }    
}


// Main internal clock called every second by Ticker.h

void Second_Tick()
{
  strDateTime tempDateTime;
  AdminTimeOutCounter++;
  cNTP_Update++;
  UnixTimestamp++;
  ConvertUnixTimeStamp(UnixTimestamp +  (config.timezone *  360) , &tempDateTime);
  if (config.daylight) 
    if (summertime(tempDateTime.year,tempDateTime.month,tempDateTime.day,tempDateTime.hour,0))
      {
      ConvertUnixTimeStamp(UnixTimestamp +  (config.timezone *  360) + 3600, &DateTime);
      }
        else
        {
        DateTime = tempDateTime;
        }
    else
    {
      DateTime = tempDateTime;
    }
  Refresh = true; 
}

#endif
