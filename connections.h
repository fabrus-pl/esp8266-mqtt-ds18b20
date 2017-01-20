#ifndef CONNECTIONS_H
#define CONNECTIONS_H




void checkConnection()  
{
  if ((WiFi.status() == WL_CONNECTED) && !AdminEnabled)
  {
  static long previousMillis = 0;                             
  long currentMillis = millis();
  boolean isConnected = true;
  
  if(currentMillis - previousMillis > connectionCheckInterval) 
    {
     previousMillis = currentMillis; 
  
        if (client.connected()) 
        {
        Serial.println("MQTT connection OK");
        } 
        else 
        {
        Serial.println("MQTT NOT connected! - Starting MQTT Client. ");
        ConnectMQTT();
        //UDPNTPClient.begin(localNtpPort);                                     // May need to uncomment this if having trouble with NTP starting up again after a failure
        }  
     }
  }
}

#endif
