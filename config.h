#ifndef CONFIG_H
#define CONFIG_H

extern boolean AdminEnabled;

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

  void initializeConfig(void)
  {
      // DEFAULT CONFIG
      config.WiFissid = "MYSSID";
      config.WiFipass = "MYPASSWORD";
      config.dhcp = true;
      config.IP[0] = 192;config.IP[1] = 168;config.IP[2] = 1;config.IP[3] = 100;
      config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
      config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;config.Gateway[3] = 1;
      config.ntpServerName = "0.au.pool.ntp.org";
      config.Update_Time_Via_NTP_Every =  0;
      config.timezone = 0;
      config.daylight = true;
      config.DeviceName = "Not Named";
      config.AutoTurnOff = false;
      config.AutoTurnOn = false;
      config.TurnOffHour = 0;
      config.TurnOffMinute = 0;
      config.TurnOnHour = 0;
      config.TurnOnMinute = 0;
      config.MQTTBroker = "MYBROKER";                       // MQTT broker parameters URL
      config.MQTTport = 0;                                  // MQTT port
      config.MQTTuser = "MYMQTTUSER";                       // MQTT username
      config.MQTTpass = "MYMQTTPASS";                       // MQTT password
      config.clientID = "MYID";                             // MQTT client ID - this device
      WriteConfig();
      Serial.println("General config applied");

  }

  void setupConfig(void)
  {
  	if (!ReadConfig())
  	{
  	  initializeConfig();
  	  AdminEnabled = true;
  	}
  }

#endif
