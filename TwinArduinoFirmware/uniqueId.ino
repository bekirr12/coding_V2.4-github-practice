String generateIdFromMac() {
    _Bluetooth.print("AT+ADDR?");
    delay(100);

    String macResponse = "";
    while (_Bluetooth.available()) {
        macResponse += (char)_Bluetooth.read();
    }
    macResponse.trim();

    Serial.println("MAC Address Response: " + macResponse);

    int addrIndex = macResponse.indexOf("OK+ADDR:");
    if (addrIndex == -1) {
        return "";
    }
    String macAddress = macResponse.substring(addrIndex + 8);
    macAddress.replace(":", "");

    int macSum = 0;
    for (int i = 0; i < macAddress.length(); i += 2) {
        macSum += (int)strtol(macAddress.substring(i, i + 2).c_str(), NULL, 16);
    }
    return String(macSum & 0x1FF);
}

String mapRgbToId(String id) {
    int uniqueId = id.toInt();

    uint8_t colors[8][3] = {
        {255, 0, 0},     // Red
        {0, 255, 0},     // Green
        {0, 0, 255},     // Blue
        {255, 255, 255}, // White
        {255, 120, 0},   // Orange
        {128, 0, 128},   // Purple
        {255, 255, 50},  // Yellow
        {64, 224, 208}   // Turquoise
    };

    String colorNames[8] = {"R", "G", "B", "W", "O", "P", "Y", "T"};

    int colorIndex1 = (uniqueId >> 6) & 0x07;
    int colorIndex2 = (uniqueId >> 3) & 0x07;
    int colorIndex3 = uniqueId & 0x07;

    twinRGB.setPixelColor(0, colors[colorIndex1][0], colors[colorIndex1][1], colors[colorIndex1][2]);
    twinRGB.setPixelColor(1, colors[colorIndex2][0], colors[colorIndex2][1], colors[colorIndex2][2]);
    twinRGB.setPixelColor(2, colors[colorIndex3][0], colors[colorIndex3][1], colors[colorIndex3][2]);
    twinRGB.show();

    Serial.println("RGB LEDs mapped to Device ID: " + id);

    return "Twin " + colorNames[colorIndex1] + colorNames[colorIndex2] + colorNames[colorIndex3];
}

void enableNotificationsOnce() {
    if (isNotificationsEnabled) {
        return;
    }

    _Bluetooth.print("AT+NOTI1");
    delay(100);

    if (_Bluetooth.available()) {
        String response = _Bluetooth.readString();
        Serial.println("Notification Response: " + response);

        if (response == "OK+Set:1") {
            Serial.println("Notifications enabled successfully.");
            isNotificationsEnabled = true;
        } else {
            Serial.println("Failed to enable notifications. Response: " + response);
        }
    } else {
        Serial.println("No response from Bluetooth module.");
    }
}

void bleSetAndCheckName(){
  if(bleNameIsCorrect == 0){
    static String deviceName = "";
    if (deviceName == "") {
        deviceName = mapRgbToId(deviceId);
    }
    _Bluetooth.print("AT+NAME" + deviceName);
    delay(100); 
    if(_Bluetooth.available()){
      bleName = _Bluetooth.readString();
      _USB.println(bleName);
      if (bleName == "OK+Set:" + deviceName) {
          bleNameIsCorrect = 1;
          _Bluetooth.print("AT+RESET");
      }  
    }
  }
}

void handleBluetoothState() {
    enableNotificationsOnce();
    bleSetAndCheckName();

    if (bluetoothState == "Disconnected") {
        if (!isLedSet && deviceId != "") {
            mapRgbToId(deviceId);
            isLedSet = true;
        }
        ledTurnOff = false;
    } else if (bluetoothState == "Connected") {
        if (!ledTurnOff) {
            twinRGB.setPixelColor(0, 0, 0, 0);
            twinRGB.setPixelColor(1, 0, 0, 0);
            twinRGB.setPixelColor(2, 0, 0, 0);
            twinRGB.show();
            ledTurnOff = true;
        }
        isLedSet = false;
    }
}

