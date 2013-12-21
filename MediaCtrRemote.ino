#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <IRremote.h>
#include <util.h>

// #define DEBUG_SERIAL

#define PORT 8888
#define NODHCP

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x3F, 0x5A };

#ifdef NODHCP
IPAddress ip(192, 168, 2, 88);
#endif

EthernetUDP Udp;

IRsend ir_send;

struct IrCode
{
  int type;
  int nbits;
  unsigned long ex1;
  unsigned long ex2;
  unsigned long long value;
};

// #define NEC 1
// #define SONY 2
// #define RC5 3
// #define RC6 4
// #define DISH 5
// #define SHARP 6
// #define PANASONIC 7
// #define JVC 8
// #define SANYO 9
// #define MITSUBISHI 10
// #define UNKNOWN -1

#define BLINK_DELAY 25
#define BLINK_PIN A0

void BlinkStatus()
{
  digitalWrite(BLINK_PIN, HIGH);
  delay(BLINK_DELAY);
  digitalWrite(BLINK_PIN, LOW);
  delay(BLINK_DELAY);
}

IrCode packet;
char * rawBuffer;

void setup()
{
  pinMode(BLINK_PIN, OUTPUT);

  #ifdef DEBUG_SERIAL
  Serial.begin(9600);

  while (Serial.available() <= 0 && Serial.read() != 'g' && Serial.read() != 'o')
  delay(100);

  Serial.println(F("Go Code Received"));
  #endif

  rawBuffer = static_cast<char*>((void*)(&packet));
  pinMode(SS, OUTPUT);

  #ifdef NODHCP
  Ethernet.begin(mac, ip);
  #else
  if (Ethernet.begin(mac) == 0)
  {
    while(true);
  }
  #endif

  Udp.begin(PORT);
}

void loop()
{
  size_t packet_size = Udp.parsePacket();

  if (packet_size && sizeof(packet) == packet_size) {
    BlinkStatus();
    BlinkStatus();
    BlinkStatus();
    BlinkStatus();


    Udp.read(rawBuffer, packet_size);

    switch (packet.type) {
      case NEC:
      ir_send.sendNEC((long unsigned int)packet.value, packet.nbits);
      break;
      case SONY:
      ir_send.sendSony((long unsigned int)packet.value, packet.nbits);
      break;
      case RC5:
      ir_send.sendRC5((long unsigned int)packet.value, packet.nbits);
      break;
      case RC6:
      ir_send.sendRC6((long unsigned int)packet.value, packet.nbits);
      break;
      case DISH:
      ir_send.sendDISH((long unsigned int)packet.value, packet.nbits);
      break;
      case SHARP:
      ir_send.sendSharp((long unsigned int)packet.value, packet.nbits);
      break;
      case PANASONIC:
      ir_send.sendPanasonic(packet.ex1, (long unsigned int)packet.value);
      break;
      case JVC:
      // ir_send.sendJVC(packet);
      break;
      default:
      break;
    }


    #ifdef DEBUG_SERIAL
    delay(100);
    Serial.println(packet_size);
    printBigInt(&Serial, packet.type);
    printBigInt(&Serial, packet.nbits);
    printBigInt(&Serial, packet.ex1);
    printBigInt(&Serial, packet.ex2);
    printBigInt(&Serial, packet.value);
    Serial.flush();
    delay(100);
    #endif
  }

  // Wait a bit before parsing the next packet
  delay(10);
}