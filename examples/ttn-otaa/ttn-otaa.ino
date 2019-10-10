/*******************************************************************************
 *
 * This example sends a valid LoRaWAN packet with payload containing temperature,
 * humidity and pressure, using frequency and encryption settings matching those 
 * of the The Things Network.
 *
 * This uses OTAA (Over-the-air activation), where where a DevEUI and
 * application key is configured, which are used in an over-the-air
 * activation procedure where a DevAddr and session keys are
 * assigned/generated for use with all further communication.
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
 * g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
 * violated by this sketch when left running for longer)!
 
 * To use this sketch, first register your application and device with
 * the things network, to set or generate an AppEUI, DevEUI and AppKey.
 * Multiple devices can use the same AppEUI, but each device has its own
 * DevEUI and AppKey.
 *
 * Do not forget to define the radio type correctly in config.h.
 *
 *******************************************************************************/
 
#include <lmic.h>
#include <hal/hal.h>
#include <xSW02.h>
#include <xOC03.h>
 
xSW02 SW02;
xOC03 OC03;

#ifdef ARDUINO_ZERO
#define Serial SerialUSB
#endif
// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
static const u1_t PROGMEM APPEUI[8]={ 0x0E, 0xFB, 0x01, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
 
// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8]={ 0xD8, 0x57, 0xC5, 0x9F, 0xE9, 0xD3, 0x81, 0x00 };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
 
// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
// The key shown here is the semtech default key.
static const u1_t PROGMEM APPKEY[16] = { 0xEB, 0x42, 0x5D, 0x09, 0xDD, 0x76, 0x88, 0xD3, 0xD6, 0x58, 0x15, 0x60, 0x7A, 0xA4, 0xAC, 0x87 };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}
 
static uint8_t mydata[] = "Hello, world!";
static osjob_t sendjob;
 
// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 60;
 
// Pin mapping
const lmic_pinmap lmic_pins = {
  .nss = 0,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 2,
  .dio = {LMIC_UNUSED_PIN, LMIC_UNUSED_PIN, LMIC_UNUSED_PIN}, //2,4,na
};
 
void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
 
            // Disable link check validation (automatically enabled
            // during join, but not supported by TTN at this time).
            LMIC_setLinkCheckMode(0);
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
              if (LMIC.dataLen == 1) {
                uint8_t result = LMIC.frame[LMIC.dataBeg + 0];
                if (result == 0x00)  {
                  Serial.println("CONTACT OPEN");
                  OC03.write(LOW);
                }              
                if (result == 0xFF)  {
                  Serial.println("CONTACT CLOSED");
                  OC03.write(HIGH);                 
                }                                            
              }
             Serial.println();
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:
            Serial.println(F("Unknown event"));
            break;
    }
}
 
void do_send(osjob_t* j) {
  SW02.poll();
  uint32_t temperature = SW02.getTempC() * 100;
  uint32_t humidity = SW02.getHumidity() * 100;
  uint32_t pressure = SW02.getPressure() * 100;
 
#define DATASIZE 8
  static uint8_t data[DATASIZE];
  data[0] = highByte(temperature);
  data[1] = lowByte(temperature);
  data[2] = highByte(humidity);
  data[3] = lowByte(humidity);
  data[4] = (pressure >> 24) & 0xFF;
  data[5] = (pressure >> 16) & 0xFF;
  data[6] = (pressure >> 8) & 0xFF;
  data[7] = pressure & 0xFF;

  
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
  } else {
    // Prepare transmission at the next possible time.
    LMIC_setTxData2(1, data, DATASIZE, 0);
  }
}
 
void setup() {
    Serial.begin(115200);
    Serial.println(F("Starting"));
    Wire.begin();
    SW02.begin();
    OC03.begin();
 
    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
//    pinMode(VCC_ENABLE, OUTPUT);
//    digitalWrite(VCC_ENABLE, HIGH);
//    delay(1000);
    #endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    LMIC_setClockError(MAX_CLOCK_ERROR * 45 / 100);
 
    // Start job (sending automatically starts OTAA too)
    do_send(&sendjob);
}
 
void loop() {
    os_runloop_once();
}
