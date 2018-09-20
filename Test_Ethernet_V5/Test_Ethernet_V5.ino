/*
  NECESARIO MODIFICAR ETHERNET.H Y ETHERNET.CPP PARA OBTENER IP DEL SERVIDOR DHCP. SOLO HAY QUE AÑADIR LO NECESARIO PARA PODER OBTENER ESA INFORMACIÓN.
  O adjuntar Librería Ethernet_Mod ya preparadas para tal fin.
  Para la Pantalla necesaria librería Adafruit_TFTLCD y Adafruit_GFX.
  Para el test ICMP es usada la librería ICMP_Ping https://github.com/BlakeFoster/Arduino-Ping
  Probado en Arduino Uno con Shiel Ethernet W5100 compatible, recomendada placa Arduino Ethernet, por tamaño y ergonomía.
  Pantalla usada 2.8 Inch TFT LCD HX8347 Shield 
  https://es.aliexpress.com/item/WAVGAT-2-8-Inch-3-3V-300mA-TFT-LCD-Shield-Touch-Display-Module-For-Arduino-UNO/32907271642.html
  o
  https://www.adafruit.com/product/376
  Módulo batería Lipo -> https://es.aliexpress.com/item/Elecrow-LiPower-Shield-for-Arduino-Portable-Device-2-In-1-Development-Board-3-7V-LiPo-Battery/32278066863.html
  Probado y funcional el 06/09/2018 con Arduino IDE 1.8.6 y las librerías modificadas basadas en Ethernet Library 1.1.2. 
  IDE configurado => https://1drv.ms/u/s!At1AwrxoRqguw-Y5gXUSvRbdpju0hA
  Posibilidad de añadir más funciones, pero prima la estabilidad y la rapidez de los resultados.

  De momento solo funciona con versiones de librería ethernet 1.1.2 o inferiores, librerías 2.0.0 o superior aún en pruebas
  (falta como hacer ICMP con la nueva librerías. Disponible la librería ethernet 2.0.0 modificada y funcional por si alguien se atreve o en un futuro se puede implementar ICMP echoReply).

*/

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Dns.h>
#include <ICMPPing.h>
#include <Adafruit_GFX.h>    // Libreria de graficos
#include <Adafruit_TFTLCD.h> // Libreria de LCD 
#include <utility/w5100.h>

// Pines de conexion del LCD
#define LCD_CS A3 // Chip Select - Pin analogico 3
#define LCD_CD A2 // Command/Data - Pin Analogico 2
#define LCD_WR A1 // LCD Write - Pin Analogico 1
#define LCD_RD A0 // LCD Read - Pin Analogico 0
#define LCD_RESET A4 // LCD Reset - Pin Analogico 4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET); // Instancia del LCD

//---------------------------------------------------------------------
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
////////////////////////////////////////////////////////////////////

// SI HAY QUE CAMBIAR ALGO, SOLO HAY QUE TOCAR AQUÍ.

#define TIEMPO_RESET 30                 // Tiempo de espera antes volver a realizar la prueba tras no encontrar servidor DHCP.
#define hostname "www.google.com"        // Host to resolve DNS sever default.

byte mac[] {0x90, 0xA2, 0xDA, 0x00, 0x68, 0xBE};


////////////////////////////////////////////////////////////////////

SOCKET pingSocket = 0;

ICMPEchoReply echoReply;
char pingbuffer[20];
static char buff[20];
unsigned int localPort = 8888;
IPAddress remote_addr;
byte ping_addr[4];

boolean test = true;
boolean pint = false;
// Temporizadores
unsigned long previousMillis = 0;

EthernetUDP Udp;
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));

byte destMac[6];

//
void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
  asm volatile ("  jmp 0");
}

// Create a Text Box
/*
  void text_box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, char* str)
  {
  tft.fillRoundRect(x1, y1, x2, y2, radius, BLACK);
  tft.setCursor(x1 + 1, y1 + 1);
  tft.print(str);
  }
*/

void setup()
{
  // Inicializar LCD y escribir mensaje de bienvenida.
  tft.reset();
  tft.begin(0x7575); // Iniciamos el LCD especificando el controlador de nuestro LC. En este caso el 0x7575 que es igual que HX8347.
  // Otros controladores: 0x9325, 0x9328, 0x7575, 0x9341, 0x8357, 0x9341.
  delay(100);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.setCursor(0, 286);
  tft.println(F("........................................"));
  tft.setTextSize(2);
  tft.setCursor(10, 300);
  tft.print(F("Final result:"));
  tft.setCursor( 0, 0);
  tft.print(F("Discovering DHCP..."));
  // Iniciar adquisición de IP por DHCP y comenzar contador.
  delay(50); // esperar a que la shield ethernet sincronice.
  unsigned long start;
  start = millis();
  if (Ethernet.begin(mac) == 0) {
    // Si hay error o no hay IP mostrar mensaje de error.
    tft.setTextColor(RED);
    tft.setCursor( 0, 18);
    tft.print(F("DHCP NOT FOUND"));
    tft.setCursor( 160, 300);
    tft.print(F(" FAIL"));
    tft.setTextColor(CYAN);
    for (byte i = TIEMPO_RESET; i > 0; i--) {
      if (i <= 15) {
        if (i == 15) {
          //text_box(0, 255, 190, 20, 0, "Restart Test in");
          tft.fillRoundRect(0, 255, 190, 20, 0, BLACK);
          tft.setCursor(0 + 1, 255 + 1);
          tft.print(F("Restart Test in"));
          //text_box(200, 255, 30, 20, 0, "15");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("15"));
        }
        else if (i == 14) { // text_box(200, 255, 30, 20, 0, "14");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("14"));
        }
        else if (i == 13) { //text_box(200, 255, 30, 20, 0, "13");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("13"));
        }
        else if (i == 12) { //text_box(200, 255, 30, 20, 0, "12");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("12"));
        }
        else if (i == 11) { //text_box(200, 255, 30, 20, 0, "11");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("11"));
        }
        else if (i == 10) { //text_box(200, 255, 30, 20, 0, "10");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("10"));
        }
        else if (i == 9) { //text_box(200, 255, 30, 20, 0, "9");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("9"));
        }
        else if (i == 8) { //text_box(200, 255, 30, 20, 0, "8");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("8"));
        }
        else if (i == 7) { //text_box(200, 255, 30, 20, 0, "7");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("7"));
        }
        else if (i == 6) { //text_box(200, 255, 30, 20, 0, "6");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("6"));
        }
        else if (i == 5) { //text_box(200, 255, 30, 20, 0, "5");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("5"));
        }
        else if (i == 4) { //text_box(200, 255, 30, 20, 0, "4");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("4"));
        }
        else if (i == 3) { //text_box(200, 255, 30, 20, 0, "3");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("3"));
        }
        else if (i == 2) { //text_box(200, 255, 30, 20, 0, "2");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("2"));
        }
        else if (i == 1) { //text_box(200, 255, 30, 20, 0, "1");
          tft.fillRoundRect(200, 255, 30, 20, 0, BLACK);
          tft.setCursor(200 + 1, 255 + 1);
          tft.print(F("1"));
        }
      }
      delay(1000);
    }
    software_Reset();
  }
  unsigned long Time = millis() - start;
  // Si todo vá bien y adquiere IP iniciar UDP (se usará más adelante)
  Udp.begin(localPort);
  // Mostrar mensaje de obtención de IP correctamente. Ahora queda probar la conexión.
  tft.fillScreen(BLACK);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.setCursor(0, 286);
  tft.println(F("........................................"));
  tft.setTextSize(2);
  tft.setCursor( 0, 0);
  tft.setTextColor(GREEN);
  tft.print(F("DHCP OK "));
  tft.setTextColor(WHITE);
  tft.print(F("in "));
  tft.setTextColor(YELLOW);
  tft.print(Time);
  tft.print(F("ms"));
  // Obtener y mostrar la IP que ha sido asignada.
  sprintf(buff, "%d.%d.%d.%d\0", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);
  tft.setCursor( 0, 18);
  tft.setTextColor(WHITE);
  tft.print(F("IPv4 CIDR obtained:"));
  tft.setTextColor(YELLOW);
  tft.setCursor( 0, 36);
  tft.print(buff);
  tft.print(F("/"));
  // Obtener y mostrar la máscara de red adquirida formato CIDR
  byte CIDR = 0;
  for (byte q = 0; q < 4; q++) {
    byte octeto = Ethernet.subnetMask()[q];
    for (byte w = 0; w < 8; w++) {
      if (bitRead(octeto, w))
        CIDR++;
    }
  }
  tft.print(CIDR);
  // Obtener y mostrar tiempo de arrendamiendo de IP
  tft.setCursor( 0, 54);
  tft.setTextColor(WHITE);
  tft.print(F("DHCP lease time:"));
  tft.setTextColor(YELLOW);
  tft.setCursor( 0, 72);
  tft.print(Ethernet.dhcpLeaseTime());
  tft.print(F("s"));
  // Obtener y mostrar la puerta de enlace.
  sprintf(buff, "%d.%d.%d.%d\0", Ethernet.gatewayIP()[0], Ethernet.gatewayIP()[1], Ethernet.gatewayIP()[2], Ethernet.gatewayIP()[3]);
  tft.setCursor( 0, 90);
  tft.setTextColor(WHITE);
  tft.print(F("Gateway IPv4 address"));
  tft.setTextColor(YELLOW);
  tft.setCursor( 0, 108);
  tft.print(buff);
  // Hacer ping a la puerta de enlace y mostrar resultado
  byte pingAddr[] = {
    Ethernet.gatewayIP()[0], Ethernet.gatewayIP()[1], Ethernet.gatewayIP()[2], Ethernet.gatewayIP()[3]
  };
  echoReply = ping(pingAddr, 1);
  if (echoReply.status == SUCCESS) {
    tft.setTextColor(GREEN);
    tft.print(F(" OK"));
  }
  else {
    tft.setTextColor(RED);
    tft.print(F(" FAIL"));
  }

  // Obtenet y mostrar la IP del servidor DNS adquirida
  sprintf(buff, "%d.%d.%d.%d\0", Ethernet.dnsServerIP()[0], Ethernet.dnsServerIP()[1], Ethernet.dnsServerIP()[2], Ethernet.dnsServerIP()[3]);
  tft.setCursor( 0, 126);
  tft.setTextColor(WHITE);
  tft.print(F("DNS IPv4 address:"));
  tft.setTextColor(YELLOW);
  tft.setCursor( 0, 144);
  tft.print(buff);
  // Hacer ping al servidor DNS y mostrar resultado
  byte pingDNSAddr[] = {
    Ethernet.dnsServerIP()[0], Ethernet.dnsServerIP()[1], Ethernet.dnsServerIP()[2], Ethernet.dnsServerIP()[3]
  };
  echoReply = ping(pingDNSAddr, 1);
  if (echoReply.status == SUCCESS) {
    tft.setTextColor(GREEN);
    tft.print(F(" OK"));
  }
  else {
    tft.setTextColor(RED);
    tft.print(F(" FAIL"));
  }

  // Resolver a IP la dirección hostname and PING.
  tft.setCursor( 0, 162);
  tft.setTextColor(WHITE);
  tft.print(F("Resolve hostname ->"));
  tft.setTextColor(MAGENTA);
  tft.setCursor( 0, 180);
  tft.print(hostname);
  tft.setTextColor(WHITE);
  // Inicializar DNSClient
  DNSClient dns;
  dns.begin(Ethernet.dnsServerIP());
  // Si obtiene IP correctamente mostrar resultado.
  if (dns.getHostByName(hostname, remote_addr)) {
    // Mostrar la IP obtenida.
    sprintf(buff, "%d.%d.%d.%d\0", remote_addr[0], remote_addr[1], remote_addr[2], remote_addr[3]);
    tft.setCursor( 0, 198);
    tft.setTextColor(YELLOW);
    tft.print(buff);
    byte InternetIpAddr[] = {
      remote_addr[0], remote_addr[1], remote_addr[2], remote_addr[3]
    };
    for (byte i = 0; i < 4; i++) {
      ping_addr[i] = remote_addr[i];
    }

    echoReply = ping(InternetIpAddr, 1);
    if (echoReply.status == SUCCESS) {
      tft.setTextColor(GREEN);
      tft.print(F(" OK"));
    }
    else {
      tft.setTextColor(RED);
      tft.print(F(" FAIL"));
      test = false;
    }
  }
  else {
    // Si no obtiene IP mostrar error.
    tft.setTextColor(RED);
    tft.setCursor( 0, 198);
    tft.print(F("FAIL"));
    test = false;
  }
  // Obtener y mostrar la IP del servidor DHCP que ha dado la IP
  sprintf(buff, "%d.%d.%d.%d\0", Ethernet.dhcpServerIP()[0], Ethernet.dhcpServerIP()[1], Ethernet.dhcpServerIP()[2], Ethernet.dhcpServerIP()[3]);
  tft.setCursor( 0, 216);
  tft.setTextColor(WHITE);
  tft.print(F("IPv4 DHCP server:"));
  tft.setTextColor(YELLOW);
  tft.setCursor( 0, 234);
  tft.print(buff);
  // Para obtenet la MAC del Servidor DHCP es necesario hacer un ping al destino
  byte pingDHCPAddr[] = {
    Ethernet.dhcpServerIP()[0], Ethernet.dhcpServerIP()[1], Ethernet.dhcpServerIP()[2], Ethernet.dhcpServerIP()[3]
  };

  echoReply = ping(pingDHCPAddr, 1);
  if (echoReply.status == SUCCESS) {
    tft.setTextColor(GREEN);
    tft.print(F(" OK"));
  }
  else {
    tft.setTextColor(RED);
    tft.print(F(" FAIL"));
  }

  // obtener y mostrar MAC del servidor DHCP que ha dado la IP (muy útil para buscar luego servidores DHCP no autorizados)
  tft.setCursor( 0, 252);
  tft.setTextColor(WHITE);
  tft.print(F("MAC DHCP server:"));
  tft.setTextColor(YELLOW);
  tft.setCursor( 0, 270);
  // Para obtenet la MAC del Servidor DHCP es necesario hacer un ping al destino
  // ahora obtenemos la mac de destino del socket creado por el ping anteriormente realizado
  W5100.readSnDHAR(pingSocket, destMac);
  delay(50);
  //Mostramos la MAC del servidor DHCP.
  for (int j = 0; j < 6; j++) {
    if (destMac[j] < 16) tft.print(F("0"));
    tft.print(destMac[j], HEX);
    if (j < 5) tft.print(F(":"));
  }
  delay(50);
}
void loop()
{
  // Hacer ping a la IP resuelta cada segundo y montrar resultados.
  if (millis() - previousMillis >= 1000 && test) {
    previousMillis = millis();
    echoReply = ping(ping_addr, 1);
    if (echoReply.status == SUCCESS)
    {
      sprintf(buff,
              "Time=%ldms TTL=%d",
              millis() - echoReply.data.time,
              echoReply.ttl);
    }
    else
    {
      sprintf(buff, "Request Timed Out");
    }
    tft.setTextColor(CYAN);
    tft.setTextSize(2);
    //text_box(0, 300, 320, 20, 0, buff);
    tft.fillRoundRect(0, 300, 240, 20, 0, BLACK);
    tft.setCursor(0 + 1, 300 + 1);
    tft.print(buff);
  }

  if (!test && !pint) {
    tft.setCursor(10, 300);
    tft.setTextColor(WHITE);
    tft.print(F("Final result:"));
    tft.setTextColor(RED);
    tft.setCursor( 160, 300);
    tft.print(F(" FAIL"));
    pint = true;
  }

  // De momento no hay que hacer más que una sola vez mostrar el resultado y hacer pines a la IP de internet.
}
