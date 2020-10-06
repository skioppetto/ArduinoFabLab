/*
  Prototipo Verifica Tastiera
  author: Alberto Coppini

  Sketch per il collaudo del metodo verifica_tastiera per il gioco Simon Says.
  Attende la pressione di uno dei 4 tasti. A ciascuno corrisponde l'accensione di un tasto ed emissione di una nota musicale

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
}

int last_value = 0;
void loop() {
  int value = verifica_tastiera();
  if (value != last_value){
    Serial.print(value);
    last_value = value;
    }
}

int pressed = 0;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

/*
  Metodo verifica_tastiera
  Return: pin corrispondente al tasto premuto
  Verifica la pressione di uno dei tasti, attiva il tono illuminando il tasto ed emettendo la nota musicale.
  Al rilascio del tasto viene restituito il valore 0 e interrotta l'illuminazione e il suono. 
*/
int verifica_tastiera() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (pressed == 0) {
      int blu_st = digitalRead(I_BLU_BTN);
      int red_st = digitalRead(I_RED_BTN);
      int gre_st = digitalRead(I_GRE_BTN);
      int yel_st = digitalRead(I_YEL_BTN);
      if (blu_st == LOW) {
        pressed = I_BLU_BTN;
        attiva_tono_seq(I_BLU_BTN);
        lastDebounceTime = millis();
      }
      else if (red_st == LOW) {
        pressed = I_RED_BTN;
        attiva_tono_seq(I_RED_BTN);
        lastDebounceTime = millis();
      }
      else if (gre_st == LOW) {
        pressed = I_GRE_BTN;
        attiva_tono_seq(I_GRE_BTN);
        lastDebounceTime = millis();
      }
      else if (yel_st == LOW) {
        pressed = I_YEL_BTN;
        attiva_tono_seq(I_YEL_BTN);
        lastDebounceTime = millis();
      }
    }
    else if (pressed > 0) {
      int press_st = digitalRead(pressed);
      if (press_st == HIGH) {
        pressed = 0;
        disattiva_tono_seq();
        lastDebounceTime = millis();
      }
    }
  }
  return pressed;
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
