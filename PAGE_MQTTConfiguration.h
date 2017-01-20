

//
//  HTML PAGE
//
const char PAGE_MQTTConfiguration[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Broker Configuration</strong>
<hr>
Connect to the Mqtt Broker with these settings:<br>
<form action="" method="get">
<table border="0"  cellspacing="0" cellpadding="3" style="width:360px" >
<tr><td align="right">ClientID:</td><td><input type="text" id="clientid" name="clientid" value=""></td></tr>
<tr><td align="right">UserName:</td><td><input type="text" id="username" name="username" value=""></td></tr>
<tr><td align="right">Password:</td><td><input type="text" id="passwd" name="passwd" value=""></td></tr>
<tr><td align="right">Port:</td><td><input type="text" id="mqttport" name="mqttport" value=""></td></tr>
<tr><td align="right">Broker URL:</td><td><input type="text" id="broker" name="broker" style="width:200px" value=""></td></tr>
<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
</form>



<script>

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
					setValues("/admin/mqttvalues");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>


)=====";

const char PAGE_Reloading[] PROGMEM = R"=====(
<meta http-equiv="refresh" content="10; URL=mqttconfig.html">
Please Wait....Saving MQTTConfiguration!
)=====";


//
//  SEND HTML PAGE OR IF A FORM SUMBITTED VALUES, PROCESS THESE VALUES
// 

void send_mqtt_configuration_html()
{
	
	if (server.args() > 0 )  // Save Settings
	{
		String temp = "";
		for ( uint8_t i = 0; i < server.args(); i++ ) {
			if (server.argName(i) == "clientid") config.clientID =   urldecode(server.arg(i));
      if (server.argName(i) == "username") config.MQTTuser =    urldecode(server.arg(i));
			if (server.argName(i) == "passwd") config.MQTTpass =    urldecode(server.arg(i));
      if (server.argName(i) == "mqttport") config.MQTTport =  server.arg(i).toInt(); 
      if (server.argName(i) == "broker") config.MQTTBroker =    urldecode(server.arg(i)); 			
		}
		server.send ( 200, "text/html", reinterpret_cast<const __FlashStringHelper *>(PAGE_Reloading ));
		WriteConfig();
    client.set_server(config.MQTTBroker, config.MQTTport);
    ConnectMQTT();
    firstStart = true;
		AdminTimeOutCounter=0;
		
	}
	else
	{
    server.send ( 200, "text/html", reinterpret_cast<const __FlashStringHelper *>(PAGE_MQTTConfiguration) ); 
	}
	Serial.println(__FUNCTION__); 
}



//
//   FILL THE PAGE WITH VALUES
//

void send_mqtt_configuration_values_html()
{

	String values ="";

	values += "clientid|" + (String) config.clientID + "|input\n";
  values += "username|" +  (String) config.MQTTuser + "|input\n";
	values += "passwd|" +  (String) config.MQTTpass + "|input\n";
  values += "mqttport|" +  (String) config.MQTTport + "|input\n";
  values += "broker|" +  (String) config.MQTTBroker + "|input\n";
	server.send ( 200, "text/plain", values);
	Serial.println(__FUNCTION__); 
	
}


