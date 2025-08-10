/*
 * ESP32 + DHT11 → MQTT + Fan Control + WiFiManager Configuration
 * Có thể điều khiển quạt qua MQTT và nút bấm
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <WiFiManager.h>
#include <Preferences.h>

/* ====== CONFIG ====== */
#define DHTPIN 4
#define DHTTYPE DHT11
#define FAN_PIN 5
#define BUTTON_PIN 0
#define CONFIG_PIN 16

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
PubSubClient mqtt(client);
Preferences prefs;

String mqtt_server = "192.168.1.38";
int mqtt_port = 1883;
bool fanState = false;
bool lastBtn = HIGH, lastCfgBtn = HIGH;
bool btnPressed = false;
unsigned long lastRead = 0, lastDebounce = 0, lastCfgDebounce = 0;

const char* topics[] = {"/home/room1/temperature", "/home/room1/humidity", "/home/room1/fan"};
const char* fan_command_topic = "/home/room1/fan/set"; // Topic để nhận lệnh

/* ====== UTILS ====== */
void blink(int n) {
  pinMode(LED_BUILTIN, OUTPUT);
  for(int i=0; i<n; i++) {
    digitalWrite(LED_BUILTIN, HIGH); delay(100);
    digitalWrite(LED_BUILTIN, LOW); delay(100);
  }
}

/* ====== MQTT CALLBACK ====== */
void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  // Convert payload to string
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.println("MQTT received: " + String(topic) + " = " + message);
  
  // Xử lý lệnh điều khiển quạt
  if (String(topic) == fan_command_topic) {
    message.toUpperCase(); // Convert to uppercase for comparison
    
    if (message == "ON" || message == "1" || message == "TRUE") {
      setFan(true);
      Serial.println("Fan turned ON via MQTT");
    } 
    else if (message == "OFF" || message == "0" || message == "FALSE") {
      setFan(false);
      Serial.println("Fan turned OFF via MQTT");
    }
    else if (message == "TOGGLE") {
      setFan(!fanState);
      Serial.println("Fan toggled via MQTT");
    }
    else {
      Serial.println("Unknown fan command: " + message);
    }
  }
}

/* ====== CONFIG ====== */
void loadConfig() {
  prefs.begin("cfg", true);
  mqtt_server = prefs.getString("mqtt", "192.168.1.38");
  mqtt_port = prefs.getInt("port", 1883);
  prefs.end();
  Serial.println("MQTT: " + mqtt_server + ":" + String(mqtt_port));
}

void saveConfig() {
  prefs.begin("cfg", false);
  prefs.putString("mqtt", mqtt_server);
  prefs.putInt("port", mqtt_port);
  prefs.end();
  Serial.println("Config saved");
}

void startConfigPortal() {
  WiFiManager wm;
  
  // Custom parameters cho MQTT
  WiFiManagerParameter mqtt_ip("mqtt", "MQTT Server", mqtt_server.c_str(), 40);
  WiFiManagerParameter mqtt_p("port", "MQTT Port", String(mqtt_port).c_str(), 6);
  
  wm.addParameter(&mqtt_ip);
  wm.addParameter(&mqtt_p);
  
  // Callback để lưu config
  wm.setSaveParamsCallback([&](){
    mqtt_server = mqtt_ip.getValue();
    mqtt_port = String(mqtt_p.getValue()).toInt();
    saveConfig();
  });
  
  wm.setConfigPortalTimeout(300); // 5 phút timeout
  
  if (!wm.startConfigPortal("ESP32_Config")) {
    Serial.println("Config failed");
    delay(3000);
    ESP.restart();
  }
  
  Serial.println("Connected!");
  blink(3);
}

/* ====== BUTTON HANDLER ====== */
void handleFanButton() {
  bool reading = digitalRead(BUTTON_PIN);
  
  // Chống rung nút bấm
  if (reading != lastBtn) {
    lastDebounce = millis();
  }
  
  if ((millis() - lastDebounce) > 50) {
    if (reading != btnPressed) {
      btnPressed = reading;
      
      // Chỉ thực hiện khi nút được nhấn xuống (LOW)
      if (btnPressed == LOW) {
        setFan(!fanState);
        blink(1);
      }
    }
  }
  
  lastBtn = reading;
}

void setFan(bool state) {
  fanState = state;
  digitalWrite(FAN_PIN, state);
  if(mqtt.connected()) {
    mqtt.publish(topics[2], state ? "ON" : "OFF", true);
  }
  Serial.println("Fan: " + String(state ? "ON" : "OFF"));
}

/* ====== NETWORK ====== */
void mqttConnect() {
  if(WiFi.status() == WL_CONNECTED && !mqtt.connected()) {
    Serial.print("MQTT connecting...");
    if(mqtt.connect("esp32_dht")) {
      Serial.println("OK");
      
      // Subscribe to fan control topic
      mqtt.subscribe(fan_command_topic);
      Serial.println("Subscribed to: " + String(fan_command_topic));
      
      // Publish current fan state
      mqtt.publish(topics[2], fanState ? "ON" : "OFF", true);
      blink(2);
    } else {
      Serial.println("Failed");
    }
  }
}

/* ====== MAIN ====== */
void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ESP32 DHT11 MQTT with 2-Way Fan Control ===");
  
  pinMode(FAN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIG_PIN, INPUT_PULLUP);
  
  dht.begin();
  loadConfig();
  
  // Kiểm tra nút config khi khởi động
  if(digitalRead(CONFIG_PIN) == LOW) {
    Serial.println("Config mode");
    blink(5);
    startConfigPortal();
  }
  
  // Kết nối WiFi bình thường
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  
  unsigned long start = millis();
  while(WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(500);
    Serial.print(".");
  }
  
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWiFi failed, starting config portal");
    startConfigPortal();
  }
  
  Serial.println("\nWiFi OK: " + WiFi.localIP().toString());
  mqtt.setServer(mqtt_server.c_str(), mqtt_port);
  mqtt.setCallback(onMqttMessage); // Set callback function
  
  Serial.println("Ready!");
  Serial.println("- GPIO0: Fan button");
  Serial.println("- GPIO16: Config button (hold 3s)");
  Serial.println("- MQTT Control: " + String(fan_command_topic));
}

void loop() {
  // Config button (giữ 3 giây)
  static unsigned long cfgHoldStart = 0;
  bool cfgRead = digitalRead(CONFIG_PIN);
  
  if(cfgRead == LOW && lastCfgBtn == HIGH) {
    cfgHoldStart = millis();
  }
  if(cfgRead == LOW && millis() - cfgHoldStart > 3000) {
    Serial.println("Starting config portal...");
    startConfigPortal();
    cfgHoldStart = millis(); // Reset để tránh lặp
  }
  lastCfgBtn = cfgRead;
  
  mqttConnect();
  mqtt.loop(); // Quan trọng: xử lý MQTT messages
  
  // Fan button control
  handleFanButton();
  
  // Sensor reading
  if(millis() - lastRead > 10000) {
    lastRead = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    if(!isnan(t) && !isnan(h)) {
      Serial.printf("T:%.1f°C H:%.1f%%\n", t, h);
      
      if(mqtt.connected()) {
        char buf[8];
        dtostrf(t, 4, 1, buf);
        mqtt.publish(topics[0], buf, true);
        dtostrf(h, 4, 1, buf);  
        mqtt.publish(topics[1], buf, true);
      }
    }
  }
}