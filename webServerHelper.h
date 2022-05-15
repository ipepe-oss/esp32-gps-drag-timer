
void setupWebserver() {
  server = new AsyncWebServer(80);

  server->on("/file", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasParam("name") && request->hasParam("action")) {
      const char *fileName = request->getParam("name")->value().c_str();
      const char *fileAction = request->getParam("action")->value().c_str();

      if (!SPIFFS.exists(fileName)) {
        Serial.println(" ERROR: file does not exist");
        request->send(400, "text/plain", "ERROR: file does not exist");
      }
      else {
        Serial.println(" file exists");
        if (strcmp(fileAction, "show") == 0) {
          Serial.println(" show");
          SpiffsFile = SPIFFS.open(fileName, "r");
          String response = "";
          while (SpiffsFile.available()) {
            response += ((char)SpiffsFile.read());
          }
          request->send(200, "text/html", response);
        } else if (strcmp(fileAction, "delete") == 0) {
          Serial.println(" deleted");
          SPIFFS.remove(fileName);
          request->redirect("/");
        } else {
          Serial.println(" ERROR: invalid action param supplied");
          request->send(400, "text/plain", "ERROR: invalid action param supplied");
        }
      }
    } else {
      request->send(400, "text/plain", "ERROR: name and action params required");
    }
  });

  server->on("/", HTTP_GET, [](AsyncWebServerRequest * request)  {
    String returnText = "";
    Serial.println("Listing files stored on SPIFFS");
    File root = SPIFFS.open("/");
    File foundfile = root.openNextFile();
    returnText += "<center><table>";
    returnText += "<tr><th>Name</th><th></th></tr>";
    returnText += ("<tr><td>USEDSPIFFS:</td><td>");
    returnText += String(SPIFFS.usedBytes());
    returnText += ("</td><tr><td>TOTALSPIFFS:</td><td>");
    returnText += String(SPIFFS.totalBytes());
    returnText += "</td></tr>";

    while (foundfile) {
      returnText += "<tr><td><a href=\"/file?action=show&name=" + String(foundfile.name()) + "\">" + String(foundfile.name()) + "</a></td>";
      returnText += "<td><a onclick=\"return confirm('Are you sure?')\" href=\"/file?action=delete&name=" + String(foundfile.name()) + "\"><button>Delete</button></a></tr>";
      foundfile = root.openNextFile();
    }
    returnText += "</table><center>";
    root.close();
    foundfile.close();

    request->send(200, "text/html", returnText);
  });

  Serial.println("Starting Webserver ...");
  server->begin();
}
