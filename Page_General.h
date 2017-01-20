//

//  HTML PAGE
//
#ifndef PAGE_GENERAL_H
#define PAGE_GENERAL_H

const char PAGE_AdminGeneralSettings[] PROGMEM =  R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>General Settings</strong>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="10" cellpadding="3" >
<tr>
  <td align="right">Name of Device</td>
  <td><input type="text" id="devicename" name="devicename" value=""></td>
</tr>


<tr>
  <td align="left" colspan="2"><hr></td>
</tr>



<tr>
  <td align="left" colspan="2">Turn on at:</td>
</tr>

<tr>
  <td align="right"> Enabled:</td>
  <td><input type="checkbox" id="tonenabled" name="tonenabled"></td>
</tr>

<tr>
  <td align="right"> Time:</td>
  <td><input maxlength="2" type="text" id="tonhour" name="tonhour" size="2" value="00">:<input maxlength="2" type="text" id="tonminute" name="tonminute" size="2" value="00"></td>
</tr>



<tr>
  <td align="left" colspan="2">Turn off at:</td>
<tr>
  <td align="right"> Enabled:</td>
  <td><input type="checkbox" id="toffenabled" name="toffenabled"></td>
</tr>

<tr>
  <td align="right"> Time:</td>
  <td><input maxlength="2" type="text" id="toffhour" name="toffhour" size="2" value="00">:<input maxlength="2" type="text" id="toffminute" name="toffminute" size="2" value="00"></td>
</tr>



<tr>
  <td align="left" colspan="2">Bypass timer: </td>
<tr> 
  <td align="right"> Enabled:</td>
  <td><input type="checkbox" id="bypassonenabled" name="bypassonenabled"></td>
</tr>
    
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
        setValues("/admin/generalvalues");
    });
  });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";


// Functions for this Page
void send_devicename_value_html()
{
    
  String values ="";
  values += "devicename|" + (String) config.DeviceName + "|div\n";
  server.send ( 200, "text/plain", values);
  Serial.println(__FUNCTION__); 
  
}

void send_general_html()
{
  
  if (server.args() > 0 )  // Save Settings
  {
    config.AutoTurnOn = false;
    config.AutoTurnOff = false;
    BypassOn = false;
    String temp = "";
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "devicename") config.DeviceName = urldecode(server.arg(i)); 
      if (server.argName(i) == "tonenabled") config.AutoTurnOn = true; 
      if (server.argName(i) == "toffenabled") config.AutoTurnOff = true; 
      if (server.argName(i) == "tonhour") config.TurnOnHour =  server.arg(i).toInt(); 
      if (server.argName(i) == "tonminute") config.TurnOnMinute =  server.arg(i).toInt(); 
      if (server.argName(i) == "toffhour") config.TurnOffHour =  server.arg(i).toInt(); 
      if (server.argName(i) == "toffminute") config.TurnOffMinute =  server.arg(i).toInt();
      if (server.argName(i) == "bypassonenabled") BypassOn = true; 
    }
    WriteConfig();
    firstStart = true;
  }
  server.send ( 200, "text/html", PAGE_AdminGeneralSettings ); 
  Serial.println(__FUNCTION__); 
  
  
}

void send_general_configuration_values_html()
{
  String values ="";
  values += "devicename|" +  (String)  config.DeviceName +  "|input\n";
  char OnHourBuff[2];
  sprintf(OnHourBuff, "%02d", config.TurnOnHour);
  values += "tonhour|"  + (String) OnHourBuff +  "|input\n";
  char OnMinBuff[2];
  sprintf(OnMinBuff, "%02d", config.TurnOnMinute);
  values += "tonminute|" +   (String) OnMinBuff +  "|input\n";
  char OffHourBuff[2];
  sprintf(OffHourBuff, "%02d", config.TurnOffHour);
  values += "toffhour|" +  (String)  OffHourBuff +  "|input\n";
  char OffMinBuff[2];
  sprintf(OffMinBuff, "%02d", config.TurnOffMinute);
  values += "toffminute|" +   (String)  OffMinBuff +  "|input\n";
  values += "toffenabled|" +  (String) (config.AutoTurnOff ? "checked" : "") + "|chk\n";
  values += "tonenabled|" +  (String) (config.AutoTurnOn ? "checked" : "") + "|chk\n";
  values += "bypassonenabled|" +  (String) (BypassOn ? "checked" : "") + "|chk\n";
  server.send ( 200, "text/plain", values);
  Serial.println(__FUNCTION__); 
}
#endif
