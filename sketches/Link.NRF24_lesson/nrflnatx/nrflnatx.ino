void setup()
{
  SPI_DIR = ( CE + SCK + CSN + MOSI);
  SPI_DIR &=~ ( IRQ + MISO);
  //  attachInterrupt(1, _ISR, LOW);// interrupt enable
  Serial.begin(9600);
  init_io();                        // Initialize IO port
  unsigned char status=SPI_Read(STATUS);
  Serial.print("status = ");
  Serial.println(status,HEX);      // read the mode’s status register, the default value should be ‘E’
  Serial.println("*******************TX_Mode Start****************************");
  TX_Mode();                       // set TX mode
}
void loop()
{
  int k = 0;
  for(;;)
  {
    for(int i=0; i<32; i++)
        tx_buf[i] = k++;
    unsigned char status = SPI_Read(STATUS);                // read register STATUS's value
    if(status&TX_DS)                                        // if receive data ready (TX_DS) interrupt
    {
      SPI_RW_Reg(FLUSH_TX,0);
      SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);     // write playload to TX_FIFO
    }
    if(status&MAX_RT)                                       // this is retransmit than  SETUP_RETR
    {
      SPI_RW_Reg(FLUSH_TX,0);
      SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);     // disable standy-mode
    }
    SPI_RW_Reg(WRITE_REG+STATUS,status);                    // clear RX_DR or TX_DS or MAX_RT interrupt flag
    delay(1000);
  }
}
