#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ESP8266WebServer.h>

// --- 1. CONFIGURAÇÕES WI-FI ---
const char* ssid = "";           
const char* password = ""; 

// --- 2. CONFIGURAÇÕES TELEGRAM ---
#define BOT_TOKEN "" 
#define CHAT_ID "" 

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
ESP8266WebServer server(80);

// --- 3. PINOS DE ENTRADA ---
const int pinoV = D2; 
const int pinoA = D3; 

bool jaNotificouV = false;
bool jaNotificouA = false;

// --- HTML COM GRÁFICO (Centralizado e Leve) ---
String getHTML() {
  String statusRelativo = "SEGURO";
  String corFundo = "#2ecc71"; // Verde
  if (digitalRead(pinoV) == HIGH) { statusRelativo = "PERIGO"; corFundo = "#e74c3c"; }
  else if (digitalRead(pinoA) == HIGH) { statusRelativo = "ATENCAO"; corFundo = "#f1c40f"; }

  String ptr = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1' charset='utf-8'>";
  ptr += "<title>Monitoramento Santos</title>";
  ptr += "<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>";
  ptr += "<script>google.charts.load('current', {'packages':['gauge']}); google.charts.setOnLoadCallback(drawChart);";
  ptr += "function drawChart() { var data = google.visualization.arrayToDataTable([['Label', 'Value'],['Status', ";
  ptr += (statusRelativo == "PERIGO" ? "100" : (statusRelativo == "ATENCAO" ? "50" : "0"));
  ptr += "]]); var options = { width: 400, height: 120, redFrom: 90, redTo: 100, yellowFrom:40, yellowTo: 90, minorTicks: 5 };";
  ptr += "var chart = new google.visualization.Gauge(document.getElementById('chart_div')); chart.draw(data, options); }";
  ptr += "setInterval(function() { window.location.reload(); }, 4000);</script>";
  
  // Estilo mantendo o original, mas garantindo centralização
  ptr += "<style>body{font-family:Arial; text-align:center; background-color:" + corFundo + "; transition: 0.5s; color:white; margin:0; display:flex; flex-direction:column; justify-content:center; align-items:center; height:100vh;}";
  ptr += "#chart_div { display: flex; justify-content: center; }</style></head>";
  
  ptr += "<body><h1> Defesa Civil Santos</h1>";
  ptr += "<h2>Status Atual: " + statusRelativo + "</h2>";
  ptr += "<div id='chart_div'></div>";
  ptr += "<p>Monitorando Encostas e Chuva via IoT</p></body></html>";
  
  return ptr;
}

void setup() {
  Serial.begin(115200);
  pinMode(pinoV, INPUT);
  pinMode(pinoA, INPUT);
  client.setInsecure(); // Necessário para o Telegram

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nConectado! IP: "); Serial.println(WiFi.localIP());

  server.on("/", []() { server.send(200, "text/html", getHTML()); });
  server.begin();
  
  Serial.println("Estabilizando sensores...");
  delay(5000); 

  digitalRead(pinoV); 
  digitalRead(pinoA);
  jaNotificouV = false; 
  jaNotificouA = false;

  
  Serial.println("Sistema Pronto!");
}

void loop() {
  server.handleClient();
  int v = digitalRead(pinoV);
  int a = digitalRead(pinoA);

  // 1. Lógica Vermelho (Perigo)
  if (v == HIGH && !jaNotificouV) {
      if(bot.sendMessage(CHAT_ID, "🚨 ALERTA CRITICO: Risco de desabamento!", "")) {
          jaNotificouV = true;
      }
  } 

  // 2. Lógica Amarelo (Atenção)
  if (a == HIGH && v == LOW && !jaNotificouA) {
      if(bot.sendMessage(CHAT_ID, "⚠️ Atencao: Chuva forte detectada.", "")) {
          jaNotificouA = true;
      }
  }

  // Reset das notificações
  if (v == LOW && a == LOW) {
    jaNotificouV = false;
    jaNotificouA = false;
  }
}
