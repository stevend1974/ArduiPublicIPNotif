// (c) Copyright 2015 Steven Decruynaere
// 
// Sketch Demonstrating the Arduino obtaining its network public IP address and
// sending it to a notification platform.
//
// sketch loosely based on example from 2010-2012 MCQN Ltd.
//     Simple example to show how to use the HttpClient library
//     Get's the web page given at http://<kHostname><kPath>//
//
//
//  Variables to modify
//  const char *kPushingBoxToken = "please set your scenario value"
//  
// Hardware needed
// - arduino + NIC interface
//  - DHCP enabled on your network
//  
//This sketch will 
//  - setup and initialize the ethernet connection
//  - obtain a DHCP address from its local network
//  - check every 30 minutes the public IP address
//      - Connect to curlmyip.com to obtain its public IP address
//      - check if the obtained ip address differs from the previously obtained
//      - if different call the notification api from pushingbox.com
//      - set the token of the service and provided the variable arduidata in the url parameters
      
// include libraries 

#include <SPI.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>


// server that will return the public ip address 
const char kHostname[] = "curlmyip.com";
// no arguments on the path
const char kPath[] = "/";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

String ipString;
String oldipString;

 const char *kHostnameNotification = "api.pushingbox.com";

 const char *kPushingBoxToken = "your scenario token here";

String notifPath;

void setup()
{

  Serial.begin(115200); 

  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }  

  ipString.reserve(20);
  notifPath.reserve(200);

  oldipString.reserve(20);
  oldipString="";
  
}

void loop()
{
  int err =0;
  EthernetClient c;
  err = getPublicIP( c, err);
  // check every 30 min (1000*60*30) minutes for updated IP
  delay(18000000);
}

int getPublicIP(EthernetClient c, int err)  {
    HttpClient http(c);
  
  err = http.get(kHostname, kPath);
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      Serial.println(" response is "+err);
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
        
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        ipString = "";
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout) )
        {
            if (http.available())
            {
                c = http.read();
                // Print out this character
                Serial.print(c);
                ipString += c;
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                delay(kNetworkDelay);
            }
        }
        Serial.println();
        Serial.println("X"+ipString+"X");
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();

  if (ipString !="")  {
    ipString.remove(14); // we only need the first 15 chars
    
    if (ipString != oldipString)  {
    Serial.println("sending *"+ipString+"* to notification service ");
    notifPath="/pushingbox?devid=";
    notifPath+=kPushingBoxToken;
    notifPath+="&arduidata=";
    notifPath+=ipString;
    Serial.println(" notifpath"+notifPath);
    informNotificationService(c,0,notifPath);
    oldipString = ipString;
    }
  }

  
  return err;
  
  }

int informNotificationService(EthernetClient c, int err,String notifPath )  {
   HttpClient http(c);
   const char *C = notifPath.c_str();
  err = http.get(kHostnameNotification, C);
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
        
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        ipString = "";
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout) )
        {
            if (http.available())
            {
                c = http.read();
                // Print out this character
                Serial.print(c);
                ipString += c;
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                delay(kNetworkDelay);
            }
        }
        Serial.println();
        Serial.println("X"+ipString+"X");
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();
  return err;
  
}















