#define LED_PIN 13
#define BUT1_PIN 2
#define BUT2_PIN 4
// Tempo de bounce
// (não sabia o tempo exato, então coloquei 20)
#define BOUNCE 20

// Variáveis para verificar o estado atual dos botões
// Elas são diferentes de digitalRead() pois só se alteram
// no momento que se aperta/solta o botão considerando bounce
int but1Pressed;
int but2Pressed;

// Tempo decorrido desde que cada botão foi pressionado
unsigned long sinceBut1Pressed;
unsigned long sinceBut2Pressed;

// Tempo de pisca-pisca do LED
unsigned long delayTime;

// Hora da iteração anterior do loop
unsigned long lastTime;

/ Hora da iteração atual
unsigned long currTime;

// Diz se o LED está "travado" (dois botões pressionados há
// mais de 500 ms)
int locked;

// Estado do LED
int state;


// Função auxiliar para ver o tempo que ambos os botões
// estão sendo pressionados
int periodBothHaveBeenPressed() {
	
	unsigned long but1 = currTime - sinceBut1Pressed;
	unsigned long but2 = currTime - sinceBut2Pressed;
	
	if (digitalRead(BUT1_PIN) && digitalRead(BUT2_PIN))
		return (but1 < but2)? but1: but2;

	return 0;
}


void setup()
{
	but1Pressed = 0;
	but2Pressed = 0;
	sinceBut1Pressed = 0;
	sinceBut2Pressed = 0;
	delayTime = 1000;
	lastTime = millis();
	locked = 0;
	state = 1;
	
	pinMode(LED_PIN, OUTPUT);
	pinMode(BUT1_PIN, INPUT); 
	pinMode(BUT2_PIN, INPUT);
	
	digitalWrite(LED_PIN, state);
}

void loop() 
{
	// Pega o tempo atual
	currTime = millis();

	// Caso de os dois botões estarem apertados por mais
	// de meio segundo (locked fica setado)
	if (locked) {
		digitalWrite(LED_PIN, LOW);
		while(1);
	}
	
	// Loop "usual", que pisca o LED
	if (currTime - delayTime >= lastTime) {
		lastTime = currTime;
		state = !state;
		digitalWrite(LED_PIN, state);
	}
	
	// Caso se aperte o botão de acelerar
	// Verifica se ele está sendo apertado, para
	// não ficar alterando o delay o tempo todo
	if (digitalRead(BUT1_PIN) && !but1Pressed) {
		delayTime = (delayTime > 100)? delayTime - 100 : 0;
		but1Pressed = 1;
		sinceBut1Pressed = currTime;
	}
	
	// Verifica se o botão 1 foi solto
	// Leva em consideração o tempo de bounce
	if (!digitalRead(BUT1_PIN) && (currTime - sinceBut1Pressed > BOUNCE)) {
		but1Pressed = 0;
	}	
	
	// Caso se aperte o botão de desacelerar
	// Verifica se ele está sendo apertado, para
	// não ficar alterando o delay o tempo todo
	// Obs.: Tempo máximo -> 3 segundos
	if (digitalRead(BUT2_PIN) && !but2Pressed) {
		delayTime = (delayTime + 100) % 3001;
		but2Pressed = 1;
		sinceBut2Pressed = currTime;
	}
	
	// Verifica se o botão 2 foi solto
	// Leva em consideração o tempo de bounce
	if (!digitalRead(BUT2_PIN) && (currTime - sinceBut2Pressed > BOUNCE)) {
		but2Pressed = 0;
	}

	// Dois botões apertados por mais de meio segundo,
	// liga a trava
	if (periodBothHaveBeenPressed() > 500
		&& (but1Pressed && but2Pressed)) {
		locked = 1;
	}
}
