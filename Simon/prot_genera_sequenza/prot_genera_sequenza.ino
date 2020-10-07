/*
  Prototipo Generatore di Sequenze
  author: Alberto Coppini

  Sketch per il collaudo dei metodi 
  	genera_sequenza 
  	incr_sequenza
    mostra_sequenza
  per la creazione automatica di sequenze e la visualizzazione tramite led e toni

  modified dd mmm aaaa
  by Name Surname

*/

const int O_BLU_LED  = 2;
const int O_RED_LED  = 3;
const int O_GRE_LED  = 4;
const int O_YEL_LED  = 5;
const int I_BLU_BTN  = 6;
const int I_RED_BTN  = 7;
const int I_GRE_BTN  = 8;
const int I_YEL_BTN  = 9;
const int O_PIEZO_BUZ  = 10;

const unsigned int BLU_FREQ = 2093;
const unsigned int RED_FREQ = 2349;
const unsigned int GRE_FREQ = 2637;
const unsigned int YEL_FREQ = 2794;

const unsigned int SEQ_ON_MS = 1000;
const unsigned int SEQ_OFF_MS = 500;
const int SEQ_LUNG_INIT = 4;

void setup() {
  Serial.begin(9600);
  pinMode(O_BLU_LED, OUTPUT);
  pinMode(O_RED_LED, OUTPUT);
  pinMode(O_GRE_LED, OUTPUT);
  pinMode(O_YEL_LED, OUTPUT);
  pinMode(I_BLU_BTN, INPUT_PULLUP);
  pinMode(I_RED_BTN, INPUT_PULLUP);
  pinMode(I_GRE_BTN, INPUT_PULLUP);
  pinMode(I_YEL_BTN, INPUT_PULLUP);
  pinMode(O_PIEZO_BUZ, OUTPUT);
  init_sequenza();
  genera_sequenza(SEQ_LUNG_INIT);
}

void loop() {
  mostra_sequenza(SEQ_ON_MS, SEQ_OFF_MS);
  genera_sequenza(1);
  delay(2000);
}


String sequenza = "";

/*
  Metodo genera_sequenza
  Param  lung: numero di passi da accodare alla sequenza
  incrementa la sequenza attuale di un numero di 
  passi random pari a lung
*/
void genera_sequenza(int lung){
  for (int i=0; i<lung; i++){
  	sequenza.concat(random(I_BLU_BTN, I_YEL_BTN+1));
  }
}

/*
  Metodo init_sequenza
  Azzera la sequenza attuale e si prepara alla generazione di una 
  nuova sequenza random
*/
void init_sequenza(){
  	sequenza = "";
	randomSeed(analogRead(0));
}



/*
  Metodo mostra_sequenza
  Param acceso_ms: numero di ms in cui ciascun tasto rimarrà acceso
  param spento_ms: numero di ms in cui ciascun tasto rimarrà spento
  Il metodo riproduce la sequenza attuale utilizzando i led e i toni.
  Attenzione, il metodo utilizza delay ed è quindi bloccante per l'intera
  durata della sequenza
*/
void mostra_sequenza(unsigned long acceso_ms, unsigned long spento_ms){
  for (int i=0; i< sequenza.length(); i++){
  	char btn_char = sequenza.charAt(i);
    int btn_int = atoi(&btn_char);
    attiva_tono_seq(btn_int);
    delay(acceso_ms);
    disattiva_tono_seq();
    if (i < (sequenza.length()-1))
    	delay(spento_ms);  
  }	
}

/*
  Metodo attiva_tono_seq
  Param  btn: pin a cui è collegato il tasto da illuminare
  Viene accesa la luce corrispondente al tasto ed emesso il suono fino alla chiamata di disattiva_tono_seq
*/
void attiva_tono_seq(int btn) {
  if (btn == I_BLU_BTN) {
    digitalWrite(O_BLU_LED, HIGH);
    tone(O_PIEZO_BUZ, BLU_FREQ);
  }
  else if (btn == I_RED_BTN) {
    digitalWrite(O_RED_LED, HIGH);
    tone(O_PIEZO_BUZ, RED_FREQ);
  }
  else if (btn == I_GRE_BTN) {
    digitalWrite(O_GRE_LED, HIGH);
    tone(O_PIEZO_BUZ, GRE_FREQ);
  }
  else if (btn == I_YEL_BTN) {
    digitalWrite(O_YEL_LED, HIGH);
    tone(O_PIEZO_BUZ, YEL_FREQ);
  }
}

/*
  Metodo disattiva_tono_seq
  Interrompe l'illuminazione del tasto e il suono precedentemente attivato tramite attiva_tono_seq
*/
void disattiva_tono_seq() {
  digitalWrite(O_BLU_LED, LOW);
  digitalWrite(O_RED_LED, LOW);
  digitalWrite(O_GRE_LED, LOW);
  digitalWrite(O_YEL_LED, LOW);
  noTone(O_PIEZO_BUZ);
}
