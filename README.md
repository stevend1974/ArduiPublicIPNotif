ArduiPublicIPNotif
======================

Sample Sketches on Arduino with Ethernet Shield to obtain the public IP of its network 
and sending it to a notification service API.

To access your home network you have multiple options.
One option is to use dynamicDNS services to access from internet your home network.
This maps your domain name to your home network public IP address (on your router).
This home router obtains a public IP address lease that may change over time.
If you know your public IP address you can directly query your public IP-address.
This replaces the need for a DNS service.
To know your public IP-address you can have your Arduino notify you of its address.
The arduino first gets the IP address from the curlmyip.com and sends that to the
notification service of pushingbox.com.

Inspiration comes from http://makezine.com/projects/notifying-doorbell-with-pushingbox/

Prerequisits
============
Create a notification service with pushingbox.com

  - Go to http://www.pushingbox.com.
  - Login with your Google account.
  - Go to the My Services page.
  - Click on Add a service.
  - Click on the service by which you want be notified.
  - Enter the name of your new service and fill in the API that this application gave you when you installed it.
  - Click Submit.
  - Go to the My Scenarios page.
  - Create a scenario called "Somebody's at the door".
  - Click on Create scenario.
  - Click on Add an Action.
  - First, add an action using the Email Service.
  - Fill in the mail Subject and the Body.
      - the mail body contains the variable for the provided IP-address
      - example : Info obtained is $arduidata$
      - the $arduidata$ variable will be sent to the pushingbox api by the arduino
  - Save your changes    

These are sample Arduino Sketches that use the official Ethernet shield. 
It has been tested with Arduino Uno (v1.0) but it should also work with other boards.

Arduino Sketches
================

ArduiPublicIPNotif
------------------
Sketch Demonstrating the Arduino obtaining its network public IP address and
sending it to a notification platform.
  
Variables to modify
  const char *kPushingBoxToken = "please set your scenario value"
  
Hardware needed
  - arduino + NIC interface
  - DHCP enabled on your network
  
This sketch will 
  - setup and initialize the ethernet connection
  - obtain a DHCP address from its local network
  - check every 30 minutes the public IP address
      - Connect to curlmyip.com to obtain its public IP address
      - check if the obtained ip address differs from the previously obtained
      - if different call the notification api from pushingbox.com
      - set the token of the service and provided the variable arduidata in the url parameters
      
Licence
=======
      
Copyright 2015 Steven D.

The ArduiPublicIPNotif software is licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

