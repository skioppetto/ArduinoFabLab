/*
  Prototipo Tono Sequenza
  author: Alberto Coppini
  
  Sketch per il collaudo del metodo attiva_tono per il gioco Simon Says. 
  Itera le 4 tipologie di tono, per ciascun tono accende la luce ed emette il tono corrispondente


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

void loop() {
  for (int i=I_BLU_BTN; i<=I_YEL_BTN; i++){
    attiva_tono_seq(i);
    delay(100);
    disattiva_tono_seq();
    delay(500);
  }
}
/*
  Metodo attiva_tono_seq
  Param  btn: pin a cui è collegato il tasto da illuminare
  Viene accesa la luce corrispondente al tasto ed emesso il suono fino alla chiamata di disattiva_tono_seq   
*/
void attiva_tono_seq(int btn){
  if (btn == I_BLU_BTN){
      digitalWrite(O_BLU_LED, HIGH);
      tone(O_PIEZO_BUZ, BLU_FREQ);
    }
  else if (btn == I_RED_BTN){
      digitalWrite(O_RED_LED, HIGH);
      tone(O_PIEZO_BUZ, RED_FREQ);
    }
  else if (btn == I_GRE_BTN){
      digitalWrite(O_GRE_LED, HIGH);
      tone(O_PIEZO_BUZ, GRE_FREQ);
    }
  else if (btn == I_YEL_BTN){
      digitalWrite(O_YEL_LED, HIGH);
      tone(O_PIEZO_BUZ, YEL_FREQ);
    }
  }

/*
  Metodo disattiva_tono_seq
  Interrompe l'illuminazione del tasto e il suono precedentemente attivato tramite attiva_tono_seq   
*/
void disattiva_tono_seq(){
    digitalWrite(O_BLU_LED, LOW);
    digitalWrite(O_RED_LED, LOW);
    digitalWrite(O_GRE_LED, LOW);
    digitalWrite(O_YEL_LED, LOW);
    noTone(O_PIEZO_BUZ);
  }
