// File-related commands

// Load file and show it to the user. Used for reading bulletins.
void cmd_printfile(char* fname, EthernetClient client) {
    File f = SD.open(fname, FILE_READ);
    if (f) {
      while (f.available()) {
        char c = f.read();
        client.print(c);
      }
      f.close();
    } else {
      client.print("Can't recognize command: \"");
      client.print(fname);
      client.print("\"");
    }
    client.print("\n? ");
}

// Create/edit bulletins from user's real-time input.
void cmd_edit(char *fname, EthernetClient client) {
  
}

// Check for new bulletins.
// TODO: Timer??
void cmd_check(EthernetClient) {
  
}
