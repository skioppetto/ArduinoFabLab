
const int I_KEYS = A1;
const int O_SR_DATA = 13;
const int O_SR_LATCH = 12;
const int O_SR_CLOCK = 11;
const int O_BUZZ = 10;
const byte SR_OFF = 0x0F;
const byte SR_BLUE_LED = 0x1F;
const byte SR_RED_LED = 0x2F;
const byte SR_YELLOW_LED = 0x4F;
const byte SR_GREEN_LED = 0x8F;
const unsigned int BLUE_FREQ = 2093;
const unsigned int RED_FREQ = 2349;
const unsigned int GREEN_FREQ = 2637;
const unsigned int YELLOW_FREQ = 2794;
const unsigned int DEFEAT_FREQ = 440;
const int DEFEAT_MS = 1000;
const int NEXT_GAME_MS = 250;
const int NEXT_SEQUENCE_MS = 2000;
const int GAME_TIMEOUT_MS = 20000;
const int DISPLAY_NUMBER_MS = 1000;
const byte NOTES_LED[] = {SR_BLUE_LED, SR_RED_LED, SR_YELLOW_LED, SR_GREEN_LED};
const int NOTES_FREQ[] = {BLUE_FREQ, RED_FREQ, GREEN_FREQ, YELLOW_FREQ};
const int NOTES_KEYS_MIN_V[] = {700, 500, 300, 100};
const int SEQ_ON_MS = 600;
const int SEQ_OFF_MS = 200;
const int SEQ_LEN_INIT = 4;
const int DEBOUNCE_DELAY_MS = 100;

int pressed = 0;
bool pressing = false;
bool newGame = true;
byte sequence[99] = {};
int sequenceCount = 0;
byte reproduced[99] = {};
int reproducedCount = 0;
unsigned long startGameTime = 0;
unsigned long debounceLastTime = 0;

void setup() {
  pinMode(I_KEYS, INPUT);
  pinMode(O_SR_DATA, OUTPUT);
  pinMode(O_SR_LATCH, OUTPUT);
  pinMode(O_SR_CLOCK, OUTPUT);
  pinMode(O_BUZZ, OUTPUT);
  testLedNotes();
  delay(100);
  testDisplay();
  initSequence();
}


void loop() {
  if (newGame) {
    startGameTime = millis();
    reproducedCount = 0;
    showSequence();
    newGame = false;
  }
  byte key = readKey();
  if (!pressing && key > 0) {
    pressing = true;
    playLedNote(key);
    reproduced[reproducedCount] = key;
    ++reproducedCount;
  } else if (pressing && key == 0) {
    pressing = false;
    stopLedNote();
  }
  if (key == 0) {
    if (reproducedCount == sequenceCount) {
      if (sequenceCompare()) {
        showScore(sequenceCount);
        appendToSequence(1);
        newGame = true;
        delay(NEXT_GAME_MS);
      } else {
        tone (O_BUZZ, DEFEAT_FREQ);
        delay(DEFEAT_MS);
        noTone(O_BUZZ);
        delay(NEXT_SEQUENCE_MS);
        initSequence();
      }
      return;
    }
  }
  if ((millis() - startGameTime) > GAME_TIMEOUT_MS) {
    tone (O_BUZZ, DEFEAT_FREQ);
    delay(DEFEAT_MS);
    noTone(O_BUZZ);
    delay(NEXT_SEQUENCE_MS);
    initSequence();
  }
}

void showScore(int score) {
  int dec = score / 10;
  int unit = score % 10;
  if (dec > 0) {
    displayNumber(dec);
    delay(DISPLAY_NUMBER_MS);
  }
  displayNumber(unit);
  delay((dec > 0) ? DISPLAY_NUMBER_MS : DISPLAY_NUMBER_MS * 2);
  updateSr(SR_OFF);
}

boolean sequenceCompare() {
  if (reproducedCount != sequenceCount) return false;
  for (int n = 0; n < sequenceCount; n++) {
    if (sequence[n] != reproduced[n])
      return false;
  }
  return true;
}

byte calcKeyPressed(int anValue) {
  for (byte i = 0; i < 4; i++) {
    if (anValue > NOTES_KEYS_MIN_V[i]) {
      return ++i;
    }
  }
  return 0x00;
}

void appendToSequence(int elCount) {
  for (int i = 0; i < elCount; i++) {
    sequence[sequenceCount] = random(1, 5);
    ++sequenceCount;
  }
}

void initSequence() {
  sequenceCount = 0;
  randomSeed(millis());
  newGame = true;
  appendToSequence(SEQ_LEN_INIT);
}

void showSequence() {
  for (int i = 0; i < sequenceCount; i++) {
    playLedNote(sequence[i]);
    delay(SEQ_ON_MS);
    stopLedNote();
    delay(SEQ_OFF_MS);
  }
}

byte readKey() {
  if ((millis() - debounceLastTime) > DEBOUNCE_DELAY_MS) {
    int an_value = analogRead(I_KEYS);
    if (pressed == 0) {
      pressed = calcKeyPressed(an_value);
    } else if (pressed > 0) {
      if (an_value < NOTES_KEYS_MIN_V[3]) {
        pressed = 0;
      }
    }
    debounceLastTime = millis();
  }
  return pressed;
}

void playLedNote(int note) {
  tone(O_BUZZ, NOTES_FREQ[note - 1]);
  updateSr(NOTES_LED[note - 1]);
}

void stopLedNote() {
  updateSr(SR_OFF);
  noTone(O_BUZZ);
}

void displayNumber(int number) {
  updateSr (((byte)number));
}

void updateSr(byte value) {
  digitalWrite(O_SR_LATCH, LOW);
  shiftOut(O_SR_DATA, O_SR_CLOCK, MSBFIRST, value);
  digitalWrite(O_SR_LATCH, HIGH);
}

void testDisplay() {
  for (int i = 0; i < 10; i++) {
    displayNumber(i);
    delay(125);
  }
  updateSr(SR_OFF);
}

void testLedNotes() {
  for (int i = 1; i <= 4; i++) {
    playLedNote(i);
    delay(125);
    stopLedNote();
    delay(125);
  }
}
