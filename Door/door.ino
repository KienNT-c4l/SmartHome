#include <Arduino.h>

#include <LiquidCrystal_I2C.h> //Thu vien lcd
#include <Keypad.h>
#include <ESP32Servo.h>
#include <EEPROM.h>
#include <WiFi.h>           // Thư viện WiFi cho ESP32
#include <PubSubClient.h>   // Thư viện MQTT

#define PIN_SG90 16 // Output pin used

const char* ssid = "SSIoT-02";
const char* password_wifi = "SSIoT-02";

const char* mqtt_server = "192.168.72.193"; // Địa chỉ IP Mosquitto broker
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned char index_t = 0;
unsigned char error_in = 0;

// init keypad
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns

char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {14, 27, 26, 25}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {17, 32, 18, 19}; // connect to the column pinouts of the keypad

int addr = 0;
char password[6] = "12345";
char pass_def[6] = "12345";
char mode_changePass[6] = "*#01#";
char mode_resetPass[6] = "*#02#";

char data_input[6];
char new_pass1[6];
char new_pass2[6];

unsigned char in_num = 0, error_pass = 0, isMode = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
Servo sg90;

void writeEpprom(char data[])
{
    unsigned char i = 0;
    for (i = 0; i < 5; i++)
    {
        EEPROM.write(i, data[i]);
    }
    EEPROM.commit();
}

void readEpprom()
{
    unsigned char i = 0;
    for (i = 0; i < 5; i++)
    {
        password[i] = EEPROM.read(i);
    }
}

void clear_data_input() // xoa gia tri nhap vao hien tai
{
    int i = 0;
    for (i = 0; i < 6; i++)
    {
        data_input[i] = '\0';
    }
}

unsigned char isBufferdata(char data[]) // Kiem tra buffer da co gia tri chua
{
    unsigned char i = 0;
    for (i = 0; i < 5; i++)
    {
        if (data[i] == '\0')
        {
            return 0;
        }
    }
    return 1;
}

bool compareData(char data1[], char data2[]) // Kiem tra 2 cai buffer co giong nhau hay khong
{
    unsigned char i = 0;
    for (i = 0; i < 5; i++)
    {
        if (data1[i] != data2[i])
        {
            return false;
        }
    }
    return true;
}

void insertData(char data1[], char data2[]) // Gan buffer 2 cho buffer 1
{
    unsigned char i = 0;
    for (i = 0; i < 5; i++)
    {
        data1[i] = data2[i];
    }
}

void getData() // Nhan buffer tu ban phim
{
    char key = keypad.getKey(); // Doc gia tri ban phim
    if (key)
    {
        // Serial.println("key != 0");
        if (in_num == 0)
        {
            data_input[0] = key;
            lcd.setCursor(5, 1);
            lcd.print(data_input[0]);
            delay(200);
            lcd.setCursor(5, 1);
            lcd.print("*");
        }
        if (in_num == 1)
        {
            data_input[1] = key;
            lcd.setCursor(6, 1);
            lcd.print(data_input[1]);
            delay(200);
            lcd.setCursor(6, 1);
            lcd.print("*");
        }
        if (in_num == 2)
        {
            data_input[2] = key;
            lcd.setCursor(7, 1);
            lcd.print(data_input[2]);
            delay(200);
            lcd.setCursor(7, 1);
            lcd.print("*");
        }
        if (in_num == 3)
        {
            data_input[3] = key;
            lcd.setCursor(8, 1);
            lcd.print(data_input[3]);
            delay(200);
            lcd.setCursor(8, 1);
            lcd.print("*");
        }
        if (in_num == 4)
        {
            data_input[4] = key;
            lcd.setCursor(9, 1);
            lcd.print(data_input[4]);
            delay(200);
            lcd.setCursor(9, 1);
            lcd.print("*");
        }
        if (in_num == 4)
        {
            Serial.println(data_input);
            in_num = 0;
        }
        else
        {
            in_num++;
        }
    }
}

void checkPass() // kiem tra password
{
    getData();
    if (isBufferdata(data_input))
    {
        if (compareData(data_input, password)) // Dung pass
        {
            lcd.clear();
            clear_data_input();
            index_t = 3;
        }
        else if (compareData(data_input, mode_changePass))
        {
            // Serial.print("mode_changePass");
            lcd.clear();
            clear_data_input();
            index_t = 1;
        }
        else if (compareData(data_input, mode_resetPass))
        {
            // Serial.print("mode_resetPass");
            lcd.clear();
            clear_data_input();
            index_t = 2;
        }
        else
        {
            if (error_pass == 2)
            {
                clear_data_input();
                lcd.clear();
                index_t = 4;
            }
            Serial.print("Error");
            lcd.clear();
            lcd.setCursor(1, 1);
            lcd.print("WRONG PASSWORD");
            clear_data_input();
            error_pass++;
            delay(1000);
            lcd.clear();
        }
    }
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password_wifi);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void reconnect_mqtt() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("MQTT connected");
    } else {
      Serial.print("Failed MQTT, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void openDoor()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("---OPENDOOR---");
    sg90.write(180);

    unsigned long startAttemptTime = millis();
    bool mqttConnected = false;

    // Thử kết nối MQTT nhưng giới hạn thời gian 5 giây
    while (!client.connected() && millis() - startAttemptTime < 5000) {
        if (client.connect("ESP32Client")) {
            mqttConnected = true;
            break;
        }
        delay(500); // đợi nửa giây rồi thử lại
    }

    if (mqttConnected) {
        client.loop();  
        client.publish("/home/door/timelog", "open door");
        Serial.println("MQTT: open door");
    } else {
        Serial.println("MQTT: không kết nối được, vẫn tiếp tục đóng cửa");
    }

    delay(5000);
    sg90.write(0);
    lcd.clear();
    index_t = 0;
}

void error()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("WRONG 3 TIME");
    lcd.setCursor(1, 1);
    lcd.print("Wait 1 minutes");
    unsigned char minute = 0;
    unsigned char i = 30;
    while (i > 0)
    {
        if (i == 1 && minute > 0)
        {
            minute--;
            i = 59;
        }
        if (i == 1 && minute == 0)
        {
            break;
        }
        i--;
        delay(1000);
    }
    lcd.clear();
    index_t = 0;
}

void changePass() // Thay doi pass
{
    lcd.setCursor(0, 0);
    lcd.print("-- Change Pass --");
    delay(3000);
    lcd.setCursor(0, 0);
    lcd.print("--- New Pass ---");
    while (1)
    {
        getData();
        if (isBufferdata(data_input))
        {
            insertData(new_pass1, data_input);
            clear_data_input();
            break;
        }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("---- AGAIN ----");
    while (1)
    {
        getData();
        if (isBufferdata(data_input))
        {
            insertData(new_pass2, data_input);
            clear_data_input();
            break;
        }
    }
    delay(1000);
    if (compareData(new_pass1, new_pass2))
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("--- Success ---");
        delay(1000);
        writeEpprom(new_pass2);
        insertData(password, new_pass2);
        lcd.clear();
        index_t = 0;
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("-- Mismatched --");
        delay(1000);
        lcd.clear();
        index_t = 0;
    }
}

void resetPass()
{
    unsigned char choise = 0;
    // Serial.println("Pass reset");
    lcd.setCursor(0, 0);
    lcd.print("---Reset Pass---");
    getData();
    if (isBufferdata(data_input))
    {
        if (compareData(data_input, password))
        {
            lcd.clear();
            clear_data_input();
            while (1)
            {
                lcd.setCursor(0, 0);
                lcd.print("---Reset Pass---");
                char key = keypad.getKey();
                if (choise == 0)
                {
                    lcd.setCursor(0, 1);
                    lcd.print(">");
                    lcd.setCursor(2, 1);
                    lcd.print("YES");
                    lcd.setCursor(9, 1);
                    lcd.print(" ");
                    lcd.setCursor(11, 1);
                    lcd.print("NO");
                }
                if (choise == 1)
                {
                    lcd.setCursor(0, 1);
                    lcd.print(" ");
                    lcd.setCursor(2, 1);
                    lcd.print("YES");
                    lcd.setCursor(9, 1);
                    lcd.print(">");
                    lcd.setCursor(11, 1);
                    lcd.print("NO");
                }
                if (key == '*')
                {
                    if (choise == 1)
                    {
                        choise = 0;
                    }
                    else
                    {
                        choise++;
                    }
                }
                if (key == '#' && choise == 0)
                {
                    lcd.clear();
                    delay(1000);
                    writeEpprom(pass_def);
                    insertData(password, pass_def);
                    lcd.setCursor(0, 0);
                    lcd.print("---Reset ok---");
                    delay(1000);
                    lcd.clear();
                    break;
                }
                if (key == '#' && choise == 1)
                {
                    lcd.clear();
                    break;
                }
            }
            index_t = 0;
        }
        else
        {
            index_t = 0;
            lcd.clear();
        }
    }
}

void setup()
{
    Serial.begin(9600);
    EEPROM.begin(64);
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);




    
    // sg90.setPeriodHertz(50); 
    // sg90.attach(PIN_SG90, 500, 2400);
    sg90.attach(PIN_SG90);
    lcd.init();
    lcd.backlight();
    lcd.print("   SYSTEM INIT   ");
    delay(2000);
    lcd.clear();

    Serial.print("PASSWORD: ");
    Serial.println(password);
}

void loop()
{
    client.loop(); //  duy trì kết nối MQTT
    lcd.setCursor(1, 0);
    lcd.print("Enter Password");
    checkPass();

    while (index_t == 1)
    {
        changePass();
    }

    while (index_t == 2)
    {
        resetPass();
    }

    while (index_t == 3)
    {
        openDoor();
        error_pass = 0;
    }

    while (index_t == 4)
    {
        error();
        error_pass = 0;
    }
}