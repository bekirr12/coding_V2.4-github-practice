void writeMessage()
{
  if(flagGoingByte_u8[0] == 1)
  {    
    channel_st.sent.message_id_u8 = (UINT8)flagGoingByte_u8[1];
    //"channel_st.sent.message_length_u8" variable was set in protocol page in switch case

    if(flagBluetooth_b) {
      // Writing for Bluetooth
      
      _Bluetooth.write(0xAA);
      _Bluetooth.write(0x44);
      _Bluetooth.write(0x1C);
      _Bluetooth.write(channel_st.sent.message_id_u8);
      _Bluetooth.write(channel_st.sent.message_length_u8);
      
      for(UINT8 i = 0; i < channel_st.sent.message_length_u8; i++)
        _Bluetooth.write(bufferGoingByte_u8[i]);

      flagBluetooth_b = false;

    } else if(flagUSB_b) {
      // Writing for USB
      //_USB.write(0x95);
      _USB.write(0xAA);
      _USB.write(0x44);
      _USB.write(0x1C);
      _USB.write(channel_st.sent.message_id_u8);
      _USB.write(channel_st.sent.message_length_u8);
      
      for(UINT8 i = 0; i < channel_st.sent.message_length_u8; i++)
        _USB.write(bufferGoingByte_u8[i]);
    
      flagUSB_b = false;
    }

    flagGoingByte_u8[0] = 0;
  }  
}
