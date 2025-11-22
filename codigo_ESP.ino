#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// -------------------- PINAGEM --------------------
#define TRIGGER_PIN  5
#define ECHO_PIN     33

#define DHTPIN       16
#define DHTTYPE      DHT22

// -------------------- OBJETOS --------------------
DHT dht(DHTPIN, DHTTYPE);

// -------------------- WIFI -----------------------
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// -------------------- MQTT -----------------------
const char* BROKER_MQTT = "44.223.43.74";
int BROKER_PORT = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const char* TOPICO_TEMP      = "/TEF/device002/attrs/t";
const char* TOPICO_UMID      = "/TEF/device002/attrs/u";
const char* TOPICO_DIST      = "/TEF/device002/attrs/d";
const char* TOPICO_MOV       = "/TEF/device002/attrs/m";

const char* ID_MQTT = "fiware_002";

// -------------------- ESTADO ANTERIOR (para exibição dinâmica) --------------------
float tempAnterior = NAN;
float umidAnterior = NAN;
long distAnterior = -1;
bool movAnterior = false;

// -------------------- FUNÇÕES DE FORMATAÇÃO --------------------
String setaVariacaoFloat(float atual, float anterior) {
  if (isnan(anterior)) return ""; // primeira leitura
  if (atual > anterior + 0.05) return " ↑"; // aumento
  if (atual < anterior - 0.05) return " ↓"; // queda
  return " ="; // estável
}

String setaVariacaoLong(long atual, long anterior) {
  if (anterior < 0) return "";
  if (atual > anterior) return " ↑";
  if (atual < anterior) return " ↓";
  return " =";
}

void imprimirCabecalho(unsigned long ms) {
  Serial.println("\n===== DADOS DOS SENSORES =====");
  Serial.printf("Timestamp (ms desde boot): %lu\n", ms);
}

// (Função reservada para futura expansão de saída em outro display)
// void imprimirLeituras(float temp, float umid, long distancia, bool movimento) {}


// -------------------- FUNÇÕES --------------------
void conectarWiFi() {
  Serial.print("Conectando ao WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    
    if (client.connect(ID_MQTT)) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente");
      delay(2000);
    }
  }
}

// -------------------- DISTÂNCIA --------------------
long medirDistancia() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
  long distancia = duracao * 0.034 / 2;

  return distancia;
}

// -------------------- SETUP -----------------------
void setup() {
  Serial.begin(115200);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();

  conectarWiFi();
  client.setServer(BROKER_MQTT, BROKER_PORT);
}

// -------------------- LOOP ------------------------
void loop() {

  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // ----- LEITURAS -----
  long distancia = medirDistancia();
  float temp = dht.readTemperature();
  float umid = dht.readHumidity();

  static long ultimaDist = distancia;
  bool movimento = abs(distancia - ultimaDist) > 5;
  ultimaDist = distancia;

  if (isnan(temp) || isnan(umid)) {
    Serial.println("Erro ao ler o DHT22");
    return;
  }

  // ----- PUBLICAÇÃO MQTT -----
  char buffer[10];

  dtostrf(temp, 4, 2, buffer);
  client.publish(TOPICO_TEMP, buffer);

  dtostrf(umid, 4, 2, buffer);
  client.publish(TOPICO_UMID, buffer);

  dtostrf(distancia, 4, 0, buffer);
  client.publish(TOPICO_DIST, buffer);

  client.publish(TOPICO_MOV, movimento ? "1" : "0");

  // ----- MONITOR SERIAL DINÂMICO -----
  imprimirCabecalho(millis());
  Serial.printf("Temperatura: %.2f °C%s\n", temp, setaVariacaoFloat(temp, tempAnterior).c_str());
  Serial.printf("Umidade: %.2f %% %s\n", umid, setaVariacaoFloat(umid, umidAnterior).c_str());
  Serial.printf("Distância: %ld cm%s\n", distancia, setaVariacaoLong(distancia, distAnterior).c_str());
  Serial.printf("Movimento: %s%s\n", movimento ? "SIM" : "NAO", (movimento != movAnterior ? (movimento ? " (NOVO)" : " (PAROU)") : ""));
  Serial.println("--------------------------------");

  // Atualiza histórico
  tempAnterior = temp;
  umidAnterior = umid;
  distAnterior = distancia;
  movAnterior = movimento;

  delay(1000);
}