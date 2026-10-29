#include "settings.h"

/*
This is a function that sends data to the app.
Data packets are sent using OsmAnd-like protocol:
id=name&lat={0}&lon={1}&timestamp={2}&speed={3}&altitude={4}

The message framing and fields are shared with the app (client-app/lib/protocol).
*/
String sendToPhone(DATA *package) {

  String result;
  result += F("&dev_batt=");  //battery of your device
  result += F("100"); // not implemented: hardcoded 100, no voltage measurement yet

  result += F("&name=");  //other tracker's name
  result += package->name;  //NAME_LENGTH bytes

  result += F("&lat=");       // cordinates
  result += String(package->lat, 6);  // latitude
  result += F("&lon=");       // record separator
  result += String(package->lon, 6);  // longitute

  //Date and time format: 2023-06-07T15:21:00Z
  result += F("&timestamp=");      // record separator
  result += package->year + 2000;  // year
  result += F("-");                // data separator
  if (package->month < 10) result += F("0");
  result += package->month;        // month
  result += F("-");                // data separator
  if (package->day < 10) result += F("0");
  result += package->day;          // day
  result += F("T");                // record separator
  if (package->hour < 10) result += F("0");
  result += package->hour;         // hour
  result += F(":");                // time separator
  if (package->minute < 10) result += F("0");
  result += package->minute;       // minute
  result += F(":");                // time separator
  if (package->second < 10) result += F("0");
  result += package->second;       // second
  result += F("Z");                // UTC

  // Sensors and additional data
  result += F("&gpsValid="); 
  result += package->gpsValid;  // validity of coordinates  bool

  // Add more data here if you need ...
  // result += "&speed=";       
  // result += package->speed;   

  result += "&sat=";
  result += package->sats;

  result += "&alt=";
  result += String(package->altitude, 1);

  result += "&battery=";
  result += package->battery;

  // result += "&C-137-level=";  // data's name in app
  // result += package->sensor2; // value

  return result;
}


String sendBatteryToPhone() {

  String result;
  
  result += "&dev_bat=";
  //result += "&db=";   // short version example
  result += 100; // not implemented: hardcoded 100

  return result;
}

