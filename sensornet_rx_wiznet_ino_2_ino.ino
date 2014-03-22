/*
 Copyright (C) 2013 Grzegorz Szczechowiak <gszczechowiak@gmail.com>
 Based on sketch of James Coliz, Jr. <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Simplest possible example of using RF24Network and DHT22 sensor
 *
 * Receiver NODE
 * Node listening for a payload and printing them on the web server.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Ethernet.h>


// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(6,7);
RF24Network network(radio);                                 // Network uses that radio

#define CMS_HOST "emoncms.org"                              // Host name of the emoncms web server
#define CMS_PORT 80                                         
#define CMS_NODEID_PREFIX "0"                               // EmonCMS NodeId (x part on xY)
#define CMS_APIKEY "c7c030fd2d1648cb44384b4907572ded"       // EmonCMS API KEY

const uint16_t channel = 90;                                 // Channel of our node
const uint16_t this_node = 0;                                // Address of our node

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 2, 177);                              // fixed ip address

EthernetServer server(80);                                  // Initialize the Ethernet server library

// Structure of our payload
struct sensor_data
{
  float value1;
  float value2;
  byte nodeNo;
};

void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24 Network Start");
 
  SPI.begin();
  radio.begin();                                          // RF24 network initialized
  network.begin(channel, this_node);
  
  Ethernet.begin(mac, ip);                                // Ethernet network initialized
  server.begin();
//  Serial.print("Server is at ");
//  Serial.println(Ethernet.localIP());
}

void loop(void)
{
  network.update();                                       // Pump the network regularly
  while ( network.available() )                           // Is there anything ready for us?
  {
    
    RF24NetworkHeader header;                            // If so, grab it and print it out
    sensor_data payload;
    network.read(header,&payload,sizeof(payload));
    Serial.print("value_1 ");
    Serial.print(payload.value1);
    Serial.print(" , value_2 ");
    Serial.print(payload.value2);
    Serial.print(" Node Id: ");
    Serial.println(payload.nodeNo);

/*  }
}
void sendToWebserwer()
{ */

  EthernetClient client = server.available();
  
  
 // if (client.connect(server, CMS_PORT)) {
    // Make a HTTP request:
    //http://emon.server.com/input/bulk.json?data=[[0,1,10,20,30,40,50,60]]
    client.print("GET http://emoncms.org/input/bulk.json?data=[[0,humidity]]");
  
  /*
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 10");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("Temperatura zdalna ");
          client.print(payload.temp);
          client.println(" Wilgotnosc ");
          client.print(payload.humidity);
          client.println(" Numer noda ");
          client.print(payload.remoteNodeNumber); 
          client.println("<br />");
          client.println("</html>");
*/          
   delay(1000);
    // close the connection:
  client.stop();
  }
}

