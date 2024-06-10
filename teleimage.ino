#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

const char* ssid = "namawifimu";
const char* password = "passwordwifimu";

// inisialisasi Bot Token
#define BOTtoken "token botnya"  // Bot Token dari BotFather

// chat id dari @myidbot
#define CHAT_ID "chatidlu"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

Servo myServo1;
Servo myServo2;
int servoPin1 = 23;
int servoPin2 = 26;
int StatusServo1 = 90;
int StatusServo2 = 90;

const int Pinled1 = 21;
bool Statusled1 = LOW;

const int Pinled2 = 2;
bool Statusled2 = LOW;

String imageWelcome = "https://i.pinimg.com/564x/f7/fb/93/f7fb93ba21fcbfdd51f85038bcff43d9.jpg";
String imageChatId = "https://i.pinimg.com/564x/ef/7c/a1/ef7ca12755bf9abb38cdb0c17010870e.jpg";
String imageTutupPintu = "https://i.pinimg.com/736x/2c/b5/34/2cb53497ad128e68b16bb6a1b1adc35a.jpg";
String imageBukaPintu = "https://i.pinimg.com/564x/37/f9/44/37f944424813f3aa7dfa1380421b23a4.jpg";
String imageStatus = "https://i.pinimg.com/564x/e0/d9/b9/e0d9b927139f5ef2ab8ac25c09767c92.jpg";
String imageLampuMerahNyala = "https://raw.githubusercontent.com/Jauhariq/lemari-iot/main/image/merah.jpg";
String imageLampuBiruNyala = "https://raw.githubusercontent.com/Jauhariq/lemari-iot/main/image/biru.jpg";
String imageSemuaLampuNyala = "https://raw.githubusercontent.com/Jauhariq/lemari-iot/main/image/merah-biru.jpg";
String imageLampuMati = "https://i.pinimg.com/564x/6b/53/af/6b53af6be67e47c70cdce335dd125b07.jpg";

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String from_name = bot.messages[i].from_name;
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      String kata = "Halo " + from_name + ", kamu tidak diizinkan untuk menggunakan bot ini!";
      //bot.sendMessage(chat_id, "Halo " + from_name + ", Kamu tidak diizinkan untuk menggunakan bot ini!", "");
      bot.sendPhoto(chat_id, imageChatId, kata);
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    if (text == "/start") {
      String control = "Selamat Datang, " + from_name + ".\n";
      control += "Gunakan Commands Di Bawah Untuk Control.\n\n";
      control += "/buka_pintu \nUntuk Membuka Pintu\n\n";
      control += "/tutup_pintu \nUntuk Menutup Pintu\n\n";
      control += "/lampumerah_nyala \nUntuk Menyalakan Lampu Merah\n\n";
      control += "/lampumerah_mati \nUntuk Matikan Lampu Merah\n\n";
      control += "/lampubiru_nyala \nUntuk Menyalakan Lampu Biru\n\n";
      control += "/lampubiru_mati \nUntuk Matikan Lampu Biru\n\n";
      control += "/semualampu_nyala \nUntuk Menyalakan Semua Lampu\n\n";
      control += "/semualampu_mati \nUntuk Matikan Semua Lampu\n\n";
      control += "/status \nUntuk Cek Status Saat Ini \n";
      //bot.sendMessage(chat_id, control, "");
      bot.sendPhoto(chat_id, imageWelcome, control);
    }

    if (text == "/buka_pintu") {
      //bot.sendMessage(chat_id, "Pintu Terbuka", "");
      bot.sendPhoto(chat_id, imageBukaPintu, "Pintu Terbuka");
      StatusServo1 = 180;
      myServo1.write(StatusServo1);
      delay(500);
      StatusServo2 = 0;
      myServo2.write(StatusServo2);
    }

    if (text == "/tutup_pintu") {
      //bot.sendMessage(chat_id, "Pintu Tertutup", "");
      bot.sendPhoto(chat_id, imageTutupPintu, "Pintu Tertutup");
      Statusled1 = LOW;
      Statusled2 = LOW;
      digitalWrite(Pinled1, Statusled1);
      digitalWrite(Pinled2, Statusled2);
      StatusServo2 = 90;
      myServo2.write(StatusServo2);
      delay(500);
      StatusServo1 = 90;
      myServo1.write(StatusServo1);
    }

    if (text == "/lampumerah_nyala") {
      //bot.sendMessage(chat_id, "Lampu Merah Menyala Abangkuh", "");
      bot.sendPhoto(chat_id, imageLampuMerahNyala, "Lampu Merah Menyala Abangkuh");
      Statusled1 = HIGH;
      digitalWrite(Pinled1, Statusled1);
    }
    
    if (text == "/lampumerah_mati") {
      //bot.sendMessage(chat_id, "Lampu Merah Padam Kanda", "");
      bot.sendPhoto(chat_id, imageLampuMati, "Lampu Merah Padam Kanda");
      Statusled1 = LOW;
      digitalWrite(Pinled1, Statusled1);
    }

    if (text == "/lampubiru_nyala") {
      //bot.sendMessage(chat_id, "Lampu Biru Menyala Abangkuh", "");
      bot.sendPhoto(chat_id, imageLampuBiruNyala, "Lampu Biru Menyala Abangkuh");
      Statusled2 = HIGH;
      digitalWrite(Pinled2, Statusled2);
    }
    
    if (text == "/lampubiru_mati") {
      //bot.sendMessage(chat_id, "Lampu Biru Padam Kanda", "");
      bot.sendPhoto(chat_id, imageLampuMati, "Lampu Biru Padam Kanda");
      Statusled2 = LOW;
      digitalWrite(Pinled2, Statusled2);
    }

    if (text == "/semualampu_nyala") {
      //bot.sendMessage(chat_id, "Semua Lampu Menyala Abangkuh", "");
      bot.sendPhoto(chat_id, imageSemuaLampuNyala, "Semua Lampu Menyala Abangkuh");
      Statusled1 = HIGH;
      Statusled2 = HIGH;
      digitalWrite(Pinled1, Statusled1);
      digitalWrite(Pinled2, Statusled2);
    }
    
    if (text == "/semualampu_mati") {
      //bot.sendMessage(chat_id, "Semua Lampu Padam Kanda", "");
      bot.sendPhoto(chat_id, imageLampuMati, "Semua Lampu Padam Kanda");
      Statusled1 = LOW;
      Statusled2 = LOW;
      digitalWrite(Pinled1, Statusled1);
      digitalWrite(Pinled2, Statusled2);
    }
    
    if (text == "/status") {
      bot.sendPhoto(chat_id, imageStatus, "Mengecek status saat ini...");
      if (digitalRead(Pinled1)){
        bot.sendMessage(chat_id, "Lampu Merah Nyala", "");
      }
      else{
        bot.sendMessage(chat_id, "Lampu Merah Mati", "");
      }
      if (digitalRead(Pinled2)){
        bot.sendMessage(chat_id, "Lampu Biru Nyala", "");
      }
      else{
        bot.sendMessage(chat_id, "Lampu Biru Mati", "");
      }
      if (StatusServo1 == 90 && StatusServo2 == 90){
        bot.sendMessage(chat_id, "Pintu Tertutup", "");
      }
      else{
        bot.sendMessage(chat_id, "Pintu Terbuka", "");
      } 
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(Pinled1, OUTPUT);
  digitalWrite(Pinled1, Statusled1);

  pinMode(Pinled2, OUTPUT);
  digitalWrite(Pinled2, Statusled2);
  
  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2);
  myServo1.write(StatusServo1);
  myServo2.write(StatusServo2);
  
  // Koneksi Ke Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
