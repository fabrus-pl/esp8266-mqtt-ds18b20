#ifndef WEBSERVER_H
#define WEBSERVER_H

/*
Include the HTML, STYLE and Script "Pages"
*/
#include "Page_Root.h"
#include "Page_Admin.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_NTPSettings.h"
#include "Page_Information.h"
#include "Page_General.h"
#include "PAGE_NetworkConfiguration.h"
#include "PAGE_MQTTConfiguration.h"

extern ESP8266WebServer server;                        // Create web server instance

void startWebServer (void)
{
	  server.on ( "/", []() { server.send ( 200, "text/html", reinterpret_cast<const __FlashStringHelper *>(PAGE_AdminMainPage ));   }  );
	  server.on ( "/index.html", []() { server.send ( 200, "text/html", reinterpret_cast<const __FlashStringHelper *>(PAGE_AdminMainPage ));   }  );
	  server.on ( "/admin.html", []() { server.send ( 200, "text/html", reinterpret_cast<const __FlashStringHelper *>(PAGE_AdminMainPage ));   }  );
	  server.on ( "/config.html", send_network_configuration_html );
	  server.on ( "/mqttconfig.html", send_mqtt_configuration_html );
	  server.on ( "/info.html", []() { server.send ( 200, "text/html", reinterpret_cast<const __FlashStringHelper *>(PAGE_Information ));   }  );
	  server.on ( "/ntp.html", send_NTP_configuration_html  );
	  server.on ( "/general.html", send_general_html  );
	  server.on ( "/style.css", []() { server.send ( 200, "text/plain", reinterpret_cast<const __FlashStringHelper *>(PAGE_Style_css ));  } );
	  server.on ( "/microajax.js", []() { server.send ( 200, "text/plain", reinterpret_cast<const __FlashStringHelper *>(PAGE_microajax_js ));  } );
	  server.on ( "/admin/values", send_network_configuration_values_html );
	  server.on ( "/admin/mqttvalues", send_mqtt_configuration_values_html );
	  server.on ( "/admin/connectionstate", send_connection_state_values_html );
	  server.on ( "/admin/infovalues", send_information_values_html );
	  server.on ( "/admin/ntpvalues", send_NTP_configuration_values_html );
	  server.on ( "/admin/generalvalues", send_general_configuration_values_html);
	  server.on ( "/admin/devicename",     send_devicename_value_html);
	  server.onNotFound ( []() { Serial.println("Page Not Found"); server.send ( 400, "text/html", "Page not Found" );   }  );
	  server.begin();
	  Serial.println( "HTTP server started" );
	  Serial.println("");
}

void handleWebServer(void)
{
	server.handleClient();
}

#endif
