#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H


// handle incoming messages

void callback(const MQTT::Publish& pub)             
{
  Serial.print("Received from Topic: "); Serial.print(pub.topic()); Serial.print("=> "); Serial.print(pub.payload_string()); Serial.print("  Length: "); Serial.println(pub.payload_len(),DEC);
   
    if ((pub.payload_string().equalsIgnoreCase("on:")))
    {
    BypassOn = true;
    client.publish("feedback", "Device <ON>");
    }
  
      else if ((pub.payload_string().equalsIgnoreCase("off:")))  
        {
        BypassOn = false;
        client.publish("feedback", "Device <OFF>");
        }


      else if ((pub.payload_string().equalsIgnoreCase("time:"))) 
        {
        char timeBuff[25];
        sprintf(timeBuff, "%02d:%02d - Date is %02d/%02d/%02d", DateTime.hour,DateTime.minute,DateTime.day,DateTime.month,DateTime.year);
        String MQTTTime = timeBuff;
        client.publish("feedback", "Time is: "+ MQTTTime);
        Serial.println("feedback Device Current Time is " + MQTTTime);
        }

      else if((pub.payload_string().startsWith("on time:"))) 
        {
        String OnTime = pub.payload_string();
            if (OnTime.length()==14) 
            {
            OnTime.remove(0,9);
            Serial.print("Device on time is: "); Serial.println(OnTime);
            config.TurnOnHour = OnTime.substring(0,3).toInt();
            config.TurnOnMinute = OnTime.substring(3,5).toInt();
            char minBuff[5];                                                                // All this just to print leading zero's!
            sprintf(minBuff, "%02d:%02d", config.TurnOnHour, config.TurnOnMinute);
            String pubOnTime = minBuff;
            client.publish("feedback", "On time set to: " + pubOnTime);
            WriteConfig();
            }
             else 
               {
               Serial.println("");
               Serial.println("Time must be entered in the following format:- on time: 12:00 or on time: 06:30 etc.");
               client.publish("feedback", "Incorrectly formatted message...!");
               }
         }

      else if((pub.payload_string().startsWith("off time:"))) 
        {
        String OffTime = pub.payload_string();
          if (OffTime.length()==15) 
            {
            OffTime.remove(0,10);
            Serial.print("Device off time is: "); Serial.println(OffTime);
            config.TurnOffHour = OffTime.substring(0,3).toInt();
            config.TurnOffMinute = OffTime.substring(3,5).toInt();
            char minBuff[5];                                                                  // All this just to print leading zero's!
            sprintf(minBuff, "%02d:%02d",config.TurnOffHour, config.TurnOffMinute);
            String pubOffTime = minBuff;
            client.publish("feedback", "Off time set to: " + pubOffTime);
            WriteConfig();
            }
              else 
              {
               Serial.println("");
               Serial.println("Time must be entered in the following format:- off time: 12:00 or off time: 06:30 etc.");
               client.publish("feedback", "Incorrectly formatted message...!");
              }
         }


      else if((pub.payload_string().startsWith("auto turn on:"))) 
        {
        String MQTTAutoOffOn = pub.payload_string();
        MQTTAutoOffOn.remove(0,14);
           if (MQTTAutoOffOn == "enable")
              {
              config.AutoTurnOn = true;
              Serial.println("Device automatic turn on enabled: ");
              client.publish("feedback", "Automatic turn on is enabled");
              WriteConfig();
              }
           else if (MQTTAutoOffOn == "disable")
              {
               config.AutoTurnOn = false; 
               Serial.println("Device automatic turn on disabled: ");
               client.publish("feedback", "Automatic turn on is disabled");
               WriteConfig();
              }           
                else
                 {
                 Serial.println("");
                 Serial.println("Time must be entered in the following format:- auto turn on: enable or auto turn on: disable.");
                 client.publish("feedback", "?? " + MQTTAutoOffOn);
                 }
         }

      else if((pub.payload_string().startsWith("auto turn off:"))) 
        {
        String MQTTAutoOffOn = pub.payload_string();
        MQTTAutoOffOn.remove(0,15);
           if (MQTTAutoOffOn == "enable")
              {
              config.AutoTurnOff = true;
              Serial.println("Device automatic turn off enabled: ");
              client.publish("feedback", "Automatic turn off is enabled");
              WriteConfig();
              }
           else if (MQTTAutoOffOn == "disable")
              {
               config.AutoTurnOff = false; 
               Serial.println("Device automatic turn off disabled: ");
               client.publish("feedback", "Automatic turn off is disabled");
               WriteConfig();
              }           
                else
                  {
                 Serial.println("");
                 Serial.println("Time must be entered in the following format:- auto turn off: enable or auto turn off: disable.");
                 client.publish("feedback", "?? " + MQTTAutoOffOn);
                 }
         }


     else if ((pub.payload_string().equalsIgnoreCase("status:")))
       {
        String statTurnOn;
        String statTurnOff;
        String bypassStat;
        String timerStat;
        if (config.AutoTurnOn)
            {
            statTurnOn = "Enabled";
            } 
            else
            {
            statTurnOn = "Disabled";
            }
  
        if (config.AutoTurnOff)
            {
            statTurnOff = "Enabled"; 
            }
            else
            {
            statTurnOff = "Disabled";
            }
        if (BypassOn)
            {
            bypassStat = "Enabled";    
            }
            else
            {
            bypassStat = "Disabled";
            }
        if (devStat)
            {
            timerStat = "Enabled";
            }
            else 
            {
            timerStat = "Disabled";
            }
        char onBuff[5];
        sprintf(onBuff, "%02d:%02d", config.TurnOnHour, config.TurnOnMinute);
        String pubOnBuff = onBuff;
        char offBuff[5];
        sprintf(offBuff, "%02d:%02d",config.TurnOffHour, config.TurnOffMinute);
        String pubOffBuff = offBuff;
        client.publish("feedback", "Current setup status: ");
        delay(100);
        client.publish("feedback", "Auto Turn on is: " + statTurnOn);
        delay(100);
        client.publish("feedback", "Auto Turn 0ff is: " + statTurnOff);
        delay(100);
        client.publish("feedback", "Turn on time is: " + pubOnBuff);
        delay(100);
        client.publish("feedback", "Turn off time is: " + pubOffBuff);
        delay(100);
        client.publish("feedback", "Bypass is: " + bypassStat);
        delay(100);
        client.publish("feedback", "Timer status is: " + timerStat);
      }
    else  
    {
    client.publish("feedback", "?? " + pub.payload_string());
    Serial.print(" \tsent: feedback=> ???? '"); Serial.print(pub.payload_string()); Serial.println("'");
    }
}

#endif      
    
