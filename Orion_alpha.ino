/* ORion: OpenRelief Ion Chamber Radiation Detector 

  - http://openrelief.org

Copyright: 2012 Andrew Back
           2011 James Coliz (Pachube/Cosm Ethercard example)
           2011 Rufus Cable (Nanode MAC addr chip code)
          
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <EtherCard.h>

#define JFET A0                   // Nanode pin connected to JFET source
#define METER 3                   // Nanode pin connected to moving coil meter (not used at present)
#define HOLD_LOW 300              // Time in ms to hold the JFET source low
#define READ_DELAY 500            // Time in ms between taking voltage readings

// Set the Cosm feed ID and API key

#define FEED    "60745" 
#define APIKEY  "secretkey"

typedef byte bytes6_t[6];

int voltageDrift;
byte mymac[6];

// Static IP configuration to use if no DHCP found

static byte static_ip[] = { 0,0,0,0 };
static byte static_gw[] = { 0,0,0,0 };
static byte static_dns[] = { 0,0,0,0 };

char website[] PROGMEM = "api.cosm.com";

byte Ethernet::buffer[500];
uint32_t timer;
Stash stash;

void printf_begin(void);
void read_MAC(byte*);
void measure_drift(int);

void setup()
{
  Serial.begin(9600);
  printf_begin();
  printf_P(PSTR("OpenRelief Ion Chamber Radiation Detector v0.1\n\r"));
 
  read_MAC(mymac);  // Fetch the MAC address
  printf_P(PSTR("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n\r"),
      mymac[0],
      mymac[1],
      mymac[2],
      mymac[3],
      mymac[4],
      mymac[5]
  );

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    printf_P(PSTR( "Failed to access Ethernet controller\n\r"));
  if (!ether.dhcpSetup())
  {
    printf_P(PSTR("DHCP failed, using static configuration\n\r"));
    ether.staticSetup(static_ip, static_gw);
    ether.copyIp(ether.dnsip, static_dns);
  }

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

  if (!ether.dnsLookup(website))
    printf_P(PSTR("DNS failed\n\r"));
    
  ether.printIp("SRV: ", ether.hisip);
}

void loop() 
{
  if(Stash::freeCount()<5){ // reset EtherCard stash if running low
    Stash::initMap(56); 
  }
  voltageDrift = measure_drift();
  printf_P(PSTR("Level: "));
  Serial.println(voltageDrift);
   
  ether.packetLoop(ether.packetReceive());
  
  if (millis() > timer) {
    timer = millis() + 10000;

    printf_P(PSTR("Posting to Cosm feed...\n\r"));
  
    // generate two fake values as payload - by using a separate stash,
    // we can determine the size of the generated message ahead of time
    byte sd = stash.create();
    stash.print("radiation,");
    stash.println((word) voltageDrift);
    stash.save();
    
    // generate the header with payload - note that the stash size is used,
    // and that a "stash descriptor" is passed in as argument using "$H"
    Stash::prepare(PSTR("PUT http://$F/v2/feeds/$F.csv HTTP/1.0" "\r\n"
                        "Host: $F" "\r\n"
                        "X-ApiKey: $F" "\r\n"
                        "Content-Length: $D" "\r\n"
                        "\r\n"
                        "$H"),
            website, PSTR(FEED), website, PSTR(APIKEY), stash.size(), sd);

    // send the packet - this also releases all stash buffers once done
    ether.tcpSend();
  } 
}

// Functions below here

int serial_putc( char c, FILE * ) 
{
  Serial.write( c );

  return c;
} 

void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

// Measure JFET gate voltage drift over a fixed period

int measure_drift() {
  pinMode(JFET, OUTPUT);          // Hold JFET gate low for a period
  digitalWrite(JFET, LOW);
  delay(HOLD_LOW);
  
  pinMode(JFET, INPUT);           // Prepare to read JFET gate voltage
  int v1 = analogRead(JFET);      // Take first reading
  delay(READ_DELAY);              // Wait
  int v2 = analogRead(JFET);      // Take a second reading
  return (v2 - v1);               // Calculate drift
}

// Nanode_MAC
// Rufus Cable, June 2011 (threebytesfull)

// Sample code to read the MAC address from the 11AA02E48 on the
// back of the Nanode V5 board.

// This code is hacky and basic - it doesn't check for bus errors
// and will probably fail horribly if it's interrupted. It's best
// run in setup() - fetch the MAC address once and keep it. After
// the address is fetched, it puts the chip back in standby mode
// in which it apparently only consumes 1uA.

// Feel free to reuse this code - suggestions for improvement are
// welcome! :)

// http://ww1.microchip.com/downloads/en/DeviceDoc/DS-22067H.pdf
// http://ww1.microchip.com/downloads/en/devicedoc/22122a.pdf 

// Nanode has UNI/O SCIO on PD7 

#define D7_ON  _BV(7) 
#define D7_OFF (~D7_ON)

#define SCIO_HIGH PORTD |= D7_ON
#define SCIO_LOW  PORTD &= D7_OFF

#define SCIO_OUTPUT DDRD |= D7_ON
#define SCIO_INPUT  DDRD &= D7_OFF

#define SCIO_READ ((PIND & D7_ON) != 0)

#define WAIT_QUARTER_BIT delayMicroseconds(9);
#define WAIT_HALF_BIT delayMicroseconds(20);

#define NOP PORTD &= 0xff

// Fixed Timings
// standby pulse time (600us+)
#define UNIO_TSTBY_US 600
// start header setup time (10us+)
#define UNIO_TSS_US 10
// start header low pulse (5us+)
#define UNIO_THDR_US 6

// SCIO Manipulation macros
#define BIT0 SCIO_HIGH;WAIT_HALF_BIT;SCIO_LOW;WAIT_HALF_BIT;
#define BIT1 SCIO_LOW;WAIT_HALF_BIT;SCIO_HIGH;WAIT_HALF_BIT;

// 11AA02E48 defines
#define DEVICE_ADDRESS 0xA0
#define READ_INSTRUCTION 0x03

// Where on the chip is the MAC address located?
#define CHIP_ADDRESS 0xFA

inline bool unio_readBit()
{
  SCIO_INPUT;
  WAIT_QUARTER_BIT;
  bool value1 = SCIO_READ;
  WAIT_HALF_BIT;
  bool value2 = SCIO_READ;
  WAIT_QUARTER_BIT;
  return (value2 && !value1);
}

void unio_standby() {
  
  SCIO_OUTPUT;
  SCIO_HIGH;
  delayMicroseconds(UNIO_TSTBY_US);
}

void unio_sendByte(byte data) {
  
  SCIO_OUTPUT;
  for (int i=0; i<8; i++) {
    if (data & 0x80) {
      BIT1;
    } else {
      BIT0;
    }
    data <<= 1;
  }
  // MAK
  BIT1;
  // SAK?
  /*bool sak =*/ unio_readBit();
}

void unio_readBytes(byte *addr, int length) {
  for (int i=0; i<length; i++) {
    
    byte data = 0;
    for (int b=0; b<8; b++) {
      data = (data << 1) | (unio_readBit() ? 1 : 0);
    }
    SCIO_OUTPUT;
    if (i==length-1) {
      BIT0; // NoMAK
    } else {
      BIT1; // MAK
    }
    /*bool sak =*/ unio_readBit();
    addr[i] = data;
  }
}

void unio_start_header() {
  SCIO_LOW;
  delayMicroseconds(UNIO_THDR_US);
  unio_sendByte(B01010101);
}

void read_MAC(byte* mac_address) {

  // no interrupts!
  cli();

  // standby
  unio_standby();
  
  // start header
  unio_start_header();
  
  unio_sendByte(DEVICE_ADDRESS);
  unio_sendByte(READ_INSTRUCTION);
  unio_sendByte(CHIP_ADDRESS >> 8);
  unio_sendByte(CHIP_ADDRESS & 0xff);
  
  // read 6 bytes
  unio_readBytes(mac_address, 6);
 
  // back to standby
  unio_standby();

  // interrupts ok now
  sei();
}

