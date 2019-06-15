#define SERIAL_DEBUG

const int capt = A0;
const int piezo = 9;

const int maxStylo = 1000;
const int minStylo = 850;

const int ambitusStylo = maxStylo - minStylo;
float stylo = 0;

// MIDI notes (69 <=> 440 Hz)
float ambitus = 66;
float offset = 42;

void setup() {
  pinMode(piezo,OUTPUT);
  pinMode(A0,INPUT);
  
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  stylo = analogRead(capt);
  
#ifdef SERIAL_DEBUG
  Serial.println(stylo);
#endif

  if (stylo <= maxStylo) {  
    // convertit l'entrée analog in en MIDI (normalisation puis conversion)
    
    // Normalisation et mise à l'échelle :
    // stylo = pow(stylo / (float) maxStylo, 1.f) * ambitus + offset;
    stylo = stylo > minStylo ? stylo : minStylo;
    stylo = pow((stylo - minStylo) / (float) ambitusStylo, 1.f) * ambitus + offset;
  
    // Conversion MIDI en frequence
    // stylo = (exp(0.057762265) * stylo * 69) / 440;
    // ou :
    // Fonction carré pour privilégier les fréquences graves
    // stylo = 440.f * pow(2.f, (stylo - 69.f) / 12.f);
    // caster stylo en int pour un rendu chromatique 
    stylo = 440.f * pow(2.f, ((int) stylo - 69.f) / 12.f);
    
    tone(piezo, stylo);
  } else {
    //coupe si trop loin
    noTone(piezo);
  }
}
