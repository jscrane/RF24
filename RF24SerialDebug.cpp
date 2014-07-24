#include "RF24_config.h"
#include "RF24.h"
#include "RF24SerialDebug.h"

/****************************************************************************/

void SerialDebug::print_byte_register(const char* name, uint8_t reg, uint8_t qty)
{
  char extra_tab = strlen_P(name) < 8 ? '\t' : 0;
  printf_P(PSTR(PRIPSTR"\t%c ="),name,extra_tab);
  while (qty--)
    printf_P(PSTR(" 0x%02x"), r.read_register(reg++));
  printf_P(PSTR("\r\n"));
}

/****************************************************************************/

void SerialDebug::print_address_register(const char* name, uint8_t reg, uint8_t qty)
{
  char extra_tab = strlen_P(name) < 8 ? '\t' : 0;
  printf_P(PSTR(PRIPSTR"\t%c ="),name,extra_tab);

  while (qty--)
  {
    uint8_t buffer[5];
    r.read_register(reg++,buffer,sizeof buffer);

    printf_P(PSTR(" 0x"));
    uint8_t* bufptr = buffer + sizeof buffer;
    while( --bufptr >= buffer )
      printf_P(PSTR("%02x"),*bufptr);
  }

  printf_P(PSTR("\r\n"));
}

/****************************************************************************/

void SerialDebug::print_status(uint8_t status)
{
  printf_P(PSTR("STATUS\t\t = 0x%02x RX_DR=%x TX_DS=%x MAX_RT=%x RX_P_NO=%x TX_FULL=%x\r\n"),
           status,
           (status & _BV(RX_DR))?1:0,
           (status & _BV(TX_DS))?1:0,
           (status & _BV(MAX_RT))?1:0,
           ((status >> RX_P_NO) & B111),
           (status & _BV(TX_FULL))?1:0
          );
}

/****************************************************************************/

static const char rf24_datarate_e_str_0[] PROGMEM = "1MBPS";
static const char rf24_datarate_e_str_1[] PROGMEM = "2MBPS";
static const char rf24_datarate_e_str_2[] PROGMEM = "250KBPS";
static const char * const rf24_datarate_e_str_P[] PROGMEM = {
  rf24_datarate_e_str_0,
  rf24_datarate_e_str_1,
  rf24_datarate_e_str_2,
};
static const char rf24_model_e_str_0[] PROGMEM = "nRF24L01";
static const char rf24_model_e_str_1[] PROGMEM = "nRF24L01+";
static const char * const rf24_model_e_str_P[] PROGMEM = {
  rf24_model_e_str_0,
  rf24_model_e_str_1,
};
static const char rf24_crclength_e_str_0[] PROGMEM = "Disabled";
static const char rf24_crclength_e_str_1[] PROGMEM = "8 bits";
static const char rf24_crclength_e_str_2[] PROGMEM = "16 bits" ;
static const char * const rf24_crclength_e_str_P[] PROGMEM = {
  rf24_crclength_e_str_0,
  rf24_crclength_e_str_1,
  rf24_crclength_e_str_2,
};
static const char rf24_pa_dbm_e_str_0[] PROGMEM = "PA_MIN";
static const char rf24_pa_dbm_e_str_1[] PROGMEM = "PA_LOW";
static const char rf24_pa_dbm_e_str_2[] PROGMEM = "PA_HIGH";
static const char rf24_pa_dbm_e_str_3[] PROGMEM = "PA_MAX";
static const char * const rf24_pa_dbm_e_str_P[] PROGMEM = { 
  rf24_pa_dbm_e_str_0,
  rf24_pa_dbm_e_str_1,
  rf24_pa_dbm_e_str_2,
  rf24_pa_dbm_e_str_3,
};

void SerialDebug::printDetails(void)
{
  print_status(r.get_status());

  print_address_register(PSTR("RX_ADDR_P0-1"),RX_ADDR_P0,2);
  print_byte_register(PSTR("RX_ADDR_P2-5"),RX_ADDR_P2,4);
  print_address_register(PSTR("TX_ADDR"),TX_ADDR);

  print_byte_register(PSTR("RX_PW_P0-6"),RX_PW_P0,6);
  print_byte_register(PSTR("EN_AA"),EN_AA);
  print_byte_register(PSTR("EN_RXADDR"),EN_RXADDR);
  print_byte_register(PSTR("RF_CH"),RF_CH);
  print_byte_register(PSTR("RF_SETUP"),RF_SETUP);
  print_byte_register(PSTR("CONFIG"),CONFIG);
  print_byte_register(PSTR("DYNPD/FEATURE"),DYNPD,2);

  printf_P(PSTR("Data Rate\t = " PRIPSTR "\r\n"), pgm_read_word(&rf24_datarate_e_str_P[r.getDataRate()]));
  printf_P(PSTR("Model\t\t = " PRIPSTR "\r\n"), pgm_read_word(&rf24_model_e_str_P[r.isPVariant()]));
  printf_P(PSTR("CRC Length\t = " PRIPSTR "\r\n"), pgm_read_word(&rf24_crclength_e_str_P[r.getCRCLength()]));
  printf_P(PSTR("PA Power\t = " PRIPSTR "\r\n"), pgm_read_word(&rf24_pa_dbm_e_str_P[r.getPALevel()]));
}

void SerialDebug::on_write_register(uint8_t reg, uint8_t value)
{
  printf_P(PSTR("write_register(%02x,%02x)\r\n"),reg,value);
}

void SerialDebug::observe_tx(uint8_t value)
{
  printf_P(PSTR("OBSERVE_TX=%02x: PLOS_CNT=%x ARC_CNT=%x\r\n"),
           value,
           (value >> PLOS_CNT) & B1111,
           (value >> ARC_CNT) & B1111);
}

void SerialDebug::on_status(uint8_t status)
{
  uint8_t result = status & _BV(TX_DS);
  Serial.println(result?"...OK.":"...Failed");
}

void SerialDebug::on_ack(uint8_t ack_payload_length)
{
    Serial.print("[AckPacket]/");
    Serial.println(ack_payload_length,DEC);
}

void SerialDebug::on_write_payload(uint8_t data_len, uint8_t blank_len)
{
  printf_P(PSTR("[Writing %u bytes %u blanks]"),data_len,blank_len);
}

void SerialDebug::on_read_payload(uint8_t data_len, uint8_t blank_len)
{
  printf_P(PSTR("[Reading %u bytes %u blanks]"),data_len,blank_len);
}

SerialDebug::SerialDebug(RF24 &radio): r(radio) 
{ 
  r.dbg = this; 
}
