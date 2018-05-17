#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H


void callback(char* topic_str, byte* payload_str, unsigned int length)
{
	String topic = String(topic_str);
	String payload = String((char*)payload_str);

    Serial.print("Received from Topic: "); Serial.print(topic); Serial.print("=> "); Serial.print(payload); Serial.print("  Length: "); Serial.println(length,DEC);
   
     if ((payload.equalsIgnoreCase("status:")))
       {
        String statTurnOn;
        String statTurnOff;
        String bypassStat;
        String timerStat;
        if (config.AutoTurnOn)
            {
            statTurnOn = "Auto Turn on is: Enabled";
            } 
            else
            {
            statTurnOn = "Auto Turn on is: Disabled";
            }
  
        if (config.AutoTurnOff)
            {
            statTurnOff = "Auto Turn 0ff is: Enabled";
            }
            else
            {
            statTurnOff = "Auto Turn 0ff is: Disabled";
            }
        if (BypassOn)
            {
            bypassStat = "Bypass is: Enabled";
            }
            else
            {
            bypassStat = "Bypass is: Disabled";
            }
        if (devStat)
            {
            timerStat = "Timer status is: Enabled";
            }
            else 
            {
            timerStat = "Timer status is: Disabled";
            }
        char onBuff[5];
        sprintf(onBuff, "%02d:%02d", config.TurnOnHour, config.TurnOnMinute);
        String pubOnBuff = "Turn on time is: ";
        pubOnBuff.concat(onBuff);
        char offBuff[5];
        sprintf(offBuff, "%02d:%02d",config.TurnOffHour, config.TurnOffMinute);
        String pubOffBuff = "Turn off time is: ";
        pubOffBuff.concat(offBuff);
        client.publish(mqttFeedbackTopic.c_str(), "Current setup status: ");
        delay(100);
        client.publish(mqttFeedbackTopic.c_str(), statTurnOn.c_str());
        delay(100);
        client.publish(mqttFeedbackTopic.c_str(), statTurnOff.c_str());
        delay(100);
        client.publish(mqttFeedbackTopic.c_str(), pubOnBuff.c_str());
        delay(100);
        client.publish(mqttFeedbackTopic.c_str(),  + pubOffBuff.c_str());
        delay(100);
        client.publish(mqttFeedbackTopic.c_str(), bypassStat.c_str());
        delay(100);
        client.publish(mqttFeedbackTopic.c_str(), timerStat.c_str());
      }
    else  
    {
		String errorString = "?? ";
		errorString.concat(payload);
		client.publish("feedback", errorString.c_str());
		Serial.print(" \tsent: feedback=> ???? '"); Serial.print(payload); Serial.println("'");
    }
}

void setupMQTTTopics(void)
{
	mqttFeedbackTopic = String("/ha/")+String(config.DeviceName)+String("/feedback");
	mqttTemperatureTopic = String("/ha/")+String(config.DeviceName)+String("/temperature");
}

void setupMQTT(void)
{
	setupMQTTTopics();
	client.setCallback(callback);                          // Complete PubSubClient initalisation after client instance creation in globals
	client.setServer(config.MQTTBroker, config.MQTTport);
}

void ConnectMQTT()
 {
   if (WiFi.status() == WL_CONNECTED)
    {
    Serial.println("");
    Serial.print("Connecting to MQTT Broker:"); //Serial.print(config.MQTTBroker);
    int retries = 4;
      while (!client.connect(config.clientID, config.MQTTuser, config.MQTTpass) && retries-- )
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
        client.publish(mqttFeedbackTopic.c_str(), "hello from esp8266");
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

#endif      
    
