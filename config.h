#ifndef CONFIG_H
#define CONFIG_H

extern boolean AdminEnabled;
#define CONFIG_STRLEN   32

#pragma pack(push)  // push current alignment to stack
#pragma pack(1)     // set alignment to 1 byte boundary

typedef struct
{
	char WiFissid[CONFIG_STRLEN];
	char WiFipass[CONFIG_STRLEN];
	byte IP[4];
    byte Netmask[4];
    byte Gateway[4];
    boolean dhcp;
    char ntpServerName[CONFIG_STRLEN];
    long Update_Time_Via_NTP_Every;
    long timezone;
    boolean daylight;
    char DeviceName[CONFIG_STRLEN];
    boolean AutoTurnOff;
    boolean AutoTurnOn;
    byte TurnOnHour;
    byte TurnOnMinute;
    byte TurnOffHour;
    byte TurnOffMinute;
    char MQTTBroker[CONFIG_STRLEN];                  // MQTT broker parameters URL
    long MQTTport;                                   // MQTT port
    char MQTTuser[CONFIG_STRLEN];                    // MQTT username
    char MQTTpass[CONFIG_STRLEN];                    // MQTT password
    char clientID[CONFIG_STRLEN];                    // MQTT client ID - this device
    uint8_t filler[222];
    uint16_t crc;
}_Config;
#pragma pack(pop)

_Config config;

uint16_t crc16Update(uint16_t crc, uint8_t a)
{
	int i;
	crc ^= a;
	for (i = 0; i < 8; ++i)
	{
	  if (crc & 1)
		crc = (crc >> 1) ^ 0xA001;
	  else
		crc = (crc >> 1);
	}
	return crc;
}
bool ReadConfig ()
{
    uint16_t crc = ~0;
    uint8_t * pconfig = (uint8_t *) &config ;
    uint8_t data ;

    // For whole size of config structure
    for (uint16_t i = 0; i < sizeof(_Config); ++i) {
        // read data
        data = EEPROM.read(i);

        // save into struct
        *pconfig++ = data ;

        // calc CRC
        crc = crc16Update(crc, data);
    }

    // CRC Error ?
    if (crc != 0) {
        return false;
    }

    return true ;
}

bool WriteConfig (void)
{
  uint8_t * pconfig ;
  bool ret_code;

  // Init pointer
  pconfig = (uint8_t *) &config ;

    // Init CRC
  config.crc = ~0;

    // For whole size of config structure, pre-calculate CRC
  for (uint16_t i = 0; i < sizeof (_Config) - 2; ++i)
    config.crc = crc16Update(config.crc, *pconfig++);

    // Re init pointer
  pconfig = (uint8_t *) &config ;

  // For whole size of config structure, write to EEP
  for (uint16_t i = 0; i < sizeof(_Config); ++i)
    EEPROM.write(i, *pconfig++);

  // Physically save
  EEPROM.commit();

  // Read Again to see if saved ok, but do
  // not clear if error this avoid clearing
  // default config and breaks OTA
  ret_code = ReadConfig();

  Serial.print(F("Write config"));

  if (ret_code)
    Serial.println(F("OK!"));
  else
    Serial.println(F("Error!"));

  // return result
  return (ret_code);
}

void initializeConfig(void)
{
	// Clear config if wanted
	memset(&config, 0, sizeof( _Config ));

	// DEFAULT CONFIG
	String("MYSSID").toCharArray(config.WiFissid, sizeof(config.WiFissid));
	String("MYPASSWORD").toCharArray(config.WiFipass, sizeof(config.WiFipass));
	config.dhcp = true;
	config.IP[0] = 192;config.IP[1] = 168;config.IP[2] = 1;config.IP[3] = 100;
	config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
	config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;config.Gateway[3] = 1;
	String("pl.au.pool.ntp.org").toCharArray(config.ntpServerName, sizeof(config.ntpServerName));
	config.Update_Time_Via_NTP_Every =  0;
	config.timezone = 0;
	config.daylight = true;
	String("Not Named").toCharArray(config.DeviceName, sizeof(config.DeviceName));
	config.AutoTurnOff = false;
	config.AutoTurnOn = false;
	config.TurnOffHour = 0;
	config.TurnOffMinute = 0;
	config.TurnOnHour = 0;
	config.TurnOnMinute = 0;
	String("MYBROKER").toCharArray(config.MQTTBroker, sizeof(config.MQTTBroker)); // MQTT broker parameters URL
	config.MQTTport = 0;                                                          // MQTT port
	String("MYMQTTUSER").toCharArray(config.MQTTuser, sizeof(config.MQTTuser));   // MQTT username
	String("MYMQTTPASS").toCharArray(config.MQTTpass, sizeof(config.MQTTpass));   // MQTT password
	String("MYID").toCharArray(config.clientID, sizeof(config.clientID));         // MQTT client ID - this device
	WriteConfig();
	Serial.println("Initial config applied");

}

  void setupConfig(void)
  {
	EEPROM.begin(sizeof(config));

  	if (!ReadConfig())
  	{
  	  initializeConfig();
      Serial.println("Enabling admin mode");
  	  AdminEnabled = true;
  	}
    Serial.print("Config set up. Size: ");Serial.println(sizeof(config));
    Serial.print("New Config set up. Size: ");Serial.println(sizeof(_Config));

  }
#endif
