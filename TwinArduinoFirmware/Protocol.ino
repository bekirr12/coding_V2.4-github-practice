void applyCommands()
{  
  if (flagAnalyzeByte_b == true)
  {
    // Reset coming byte flag
    flagAnalyzeByte_b = false;
    
    UINT8   message_ID    = channel_st.received.message_id_u8;
    flagGoingByte_u8[10]  = {0};  // Clean going bytes
    _USB.println(message_ID);
    
    switch (message_ID)
    {
      case (UINT8)_digitalWrite:
        digitalWrite_F();
        break;

      case (UINT8)_digitalRead:
        digitalRead_F();
        break;

      case (UINT8)_analogWrite:
        analogWrite_F();
        break;

      case (UINT8)_analogRead:
        analogRead_F();
        break;

      case (UINT8)_pianoButton:
        pianoButton_F();
        break;

      case (UINT8)_servo:
        servo_F();
        break;
        
      case (UINT8)_hcsr04:
        HCSR04_F();
        break;

      case (UINT8)_ws2812b:
        WS2812B_F();
        break;

      case (UINT8)_initTwin:
        initTwin();
        break;

      case (UINT8) _getFirmwareVersion:
        getFirmwareVersion();
        break;

      case (UINT8) _playNote:
        playNote();
        break;

      case (UINT8) _dcMotor:
        dcMotor();
        break;
      case (UINT8) _raspiComm:
        raspiComm();
        break; 
      case (UINT8)_dht11:
        DHT11_F();
        break;
      case (UINT8) _doubledc:
        double_dc_F();
        break; 
    } 
  }
}
 
