// C++ code
//
// Configurações iniciais: Executa apenas uma vez.
void setup()
{
  pinMode(2, OUTPUT);	// Define o pino 2 (LED Vermelho) como saída.
  pinMode(3, OUTPUT);	// Define o pino 3 (LED Amarelo) como saída.
  pinMode(4, OUTPUT);	// Define o pino 4 (LED Verde) como saída.
}

// Loop principal: Executa repetidamente e infinitamente.
void loop()
{
  // --- Sequência do LED Vermelho (pino 2) ---
  digitalWrite(2, HIGH);	// LIGA o LED Vermelho.
  delay(1000);				// Espera 1 segundo (1000 ms).
  digitalWrite(2, LOW);		// DESLIGA o LED Vermelho.
  delay(1000);	  			// Espera 1 segundo (1000 ms).

  
  // --- Sequência do LED Amarelo (pino 3) ---
  digitalWrite(3, HIGH);	// LIGA o LED Amarelo.
  delay(1000);				// Espera 1 segundo (1000 ms).
  digitalWrite(3, LOW);		// DESLIGA o LED Amarelo.
  delay(1000);	  			// Espera 1 segundo (1000 ms).

  
  // --- Sequência do LED Verde (pino 4) ---
  digitalWrite(4, HIGH);	// LIGA o LED Verde.
  delay(1000);				// Espera 1 segundo (1000 ms).
  digitalWrite(4, LOW);		// DESLIGA o LED Verde.
  delay(1000);	  			// Espera 1 segundo (1000 ms).

  // Volta ao início do loop.
}