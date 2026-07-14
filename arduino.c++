const int pinoChuvaAnalog = A0;   
const int pinoTrigger = 10;       
const int pinoEcho = 9;           
const int pinoNodeVermelho = 8;   
const int pinoNodeAmarelo = 12;   
const int ledVermelho = 5;
const int ledAmarelo = 4;
const int ledVerde = 3;
const int pinoBuzzer = 2;

void setup() {
  pinMode(pinoTrigger, OUTPUT); pinMode(pinoEcho, INPUT);
  pinMode(ledVermelho, OUTPUT); pinMode(ledAmarelo, OUTPUT); pinMode(ledVerde, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT); 
  pinMode(pinoNodeVermelho, OUTPUT); pinMode(pinoNodeAmarelo, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Leitura Ultrassônico
  digitalWrite(pinoTrigger, LOW); delayMicroseconds(2);
  digitalWrite(pinoTrigger, HIGH); delayMicroseconds(10);
  digitalWrite(pinoTrigger, LOW);
  long duracao = pulseIn(pinoEcho, HIGH, 30000);
  int distancia = duracao * 0.034 / 2;
  if (distancia <= 0) distancia = 100;

  int valorChuva = analogRead(pinoChuvaAnalog);

  // --- MONITOR SERIAL ORGANIZADO ---
  Serial.print("Chuva:"); Serial.print(valorChuva);
  Serial.print(" | "); // Separador visual
  Serial.print("Distancia:"); Serial.print(distancia);
  Serial.print("cm | ");

  // 1. VERMELHO (PERIGO)
  if (valorChuva < 400 || (distancia > 0 && distancia <= 30)) {
    digitalWrite(ledVermelho, HIGH); digitalWrite(ledAmarelo, LOW); digitalWrite(ledVerde, LOW);
    digitalWrite(pinoBuzzer, HIGH);
    digitalWrite(pinoNodeAmarelo, LOW);
    delay(10); 
    digitalWrite(pinoNodeVermelho, HIGH);
    Serial.println("STATUS: PERIGO 🚨"); // Usei println para pular linha
  } 
  // 2. AMARELO (ATENÇÃO)
  else if ((valorChuva >= 400 && valorChuva < 700) || (distancia > 30 && distancia <= 60)) {
    digitalWrite(ledVermelho, LOW); digitalWrite(ledAmarelo, HIGH); digitalWrite(ledVerde, LOW);
    digitalWrite(pinoBuzzer, LOW);
    digitalWrite(pinoNodeVermelho, LOW);
    delay(10); 
    digitalWrite(pinoNodeAmarelo, HIGH);
    Serial.println("STATUS: ATENCAO ⚠️");
  } 
  // 3. VERDE (SEGURO)
  else {
    digitalWrite(ledVermelho, LOW); digitalWrite(ledAmarelo, LOW); digitalWrite(ledVerde, HIGH);
    digitalWrite(pinoBuzzer, LOW);
    digitalWrite(pinoNodeVermelho, LOW); digitalWrite(pinoNodeAmarelo, LOW);
    Serial.println("STATUS: SEGURO ✅");
  }

  // --- CONTROLE DE VELOCIDADE ---
  // 300 = Muito rápido | 1000 = 1 segundo | 2000 = 2 segundos
  delay(1000); 
}
