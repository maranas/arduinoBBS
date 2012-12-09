#include <Ethernet.h>
#include <SD.h>
#include <SPI.h>

#define FILENAME_MAX 13 // 8.3

EthernetServer server(23);
EthernetClient client;
IPAddress ip(192,168,1,105);

// TODO: Single connections only
int is_connected = 0;
String inputString;

byte mac[] = {0x98, 0x76, 0x54, 0x32, 0x10, 0xAB}; // Replace with your shield's mac address

void setup() {
  // Initialize
  is_connected = 0;
  inputString = String("");
  
  Ethernet.begin(mac,ip);
  server.begin();
  Serial.begin(9600);
  
  // SD reader setup
  pinMode(10, OUTPUT);
  if (!SD.begin(4)) {
    Serial.println("Can't initialize SD card.");
    return; 
  }
  
}

void cmd_quit(EthernetClient client) {
    client.flush();
    client.print("Disconnecting...");
    client.stop();
    is_connected = 0;
}

void connectClient() {
    client = server.available();
    client.flush();
    if (client) {
      is_connected = 1;
      // Do first connect stuff here
      cmd_printfile("motd", client);
    }
}

void loop() {
  // TODO: Single connections only; support multiple in the
  // future.
  if (is_connected != 1) {
    connectClient();
  }
  if (client) {
    if (client.available()) {
      char in = client.read();
      switch (in) {
        case 'g':
        case 'G':
          // Goodbye!
          cmd_quit(client);
          break;
        case '?':
          client.flush();
          cmd_printfile("help", client);
          break;
        case '\n':
        case '\r':
          // Try opening that file
          client.flush();
          inputString = inputString + "\0";
          char filename[13];
          inputString.toCharArray(filename, FILENAME_MAX);
          cmd_printfile(filename, client);
          inputString = "";
          break;
        default:
          // collect input here while length < filename max
          if (inputString.length() < FILENAME_MAX) {
            inputString = inputString + in;
          }
          break;
      }
    }
  }
}
