//======================INITIALIZATION======================================
const int numButtons = 8;   // Jumlah tombol
const int level1 = 11;  // Pin tombol LEVEL 1
const int level2 = 12;  // Pin tombol LEVEL 2
const int buzzerPin = 38;  // Pin buzzer
const int level1_led = 40;  // pin indikator LED level 1
const int level2_led = 42;  // pin indikator LED level 2
const int idle_led = 44;  // pin indikator LED Idle
const int buttonPins[numButtons] = {3, 4, 5, 6, 7, 8, 9, 10};  // Pin tombol
const int ledPins[numButtons] = {22, 24, 26, 28, 30, 32, 34, 36};  // Pin LED

int currentButton = -1;  // Tombol yang sedang aktif
unsigned long previousMillis = 0;  // Variabel untuk menyimpan waktu sebelumnya
unsigned long interval;  // Interval perpindahan tombol dalam milidetik (3 detik)
unsigned long startTime = 0;  // Waktu mulai latihan
unsigned long elapsedTime = 0;  // Waktu yang telah berlalu sejak latihan dimulai
unsigned long detik,menit,milidetik;
int benar,salah;

bool flag = false;
bool isTrainingStarted = false;  // Status latihan dimulai
bool level1_state = false;
bool level2_state = false;


int benarButtons[100]; // Array untuk menyimpan nomor tombol yang ditekan dengan benar
unsigned long benarTimes[100]; // Array untuk menyimpan waktu tombol ditekan dengan benar
// unsigned long benarTimes[numButtons]; // Array untuk menyimpan waktu tombol ditekan dengan benar

int angkaMuncul[8] = {1, 2, 3, 4, 5, 6, 7,8};
int jumlahAngka = sizeof(angkaMuncul) / sizeof(angkaMuncul[0]);

//========================DONE==========================================



//=========================FUNCTION====================================
void startTraining() {
  startTime = millis();  // Mendapatkan waktu mulai latihan
  elapsedTime = 0;  // Mengatur waktu yang telah berlalu menjadi 0
  previousMillis = startTime;  // Menyimpan waktu mulai latihan sebagai waktu sebelumnya
  isTrainingStarted = true;
  resetLEDs();
  benar = 0;
  salah = 0;
}

void resetLEDs() {
  for (int i = 0; i < numButtons; i++) {
    digitalWrite(ledPins[i], LOW);  // Mematikan semua LED
  }
}

int getRandomButton() {
  if (jumlahAngka == 0) {
    resetArray();
  }
  
  int indeksAcak = random(jumlahAngka);  // Memilih indeks acak dari 0 hingga jumlahAngka
  int buttonIndex = angkaMuncul[indeksAcak];  // Mendapatkan indeks tombol acak
  angkaMuncul[indeksAcak] = angkaMuncul[jumlahAngka - 1];  // Mengganti angka acak dengan angka terakhir dalam array
  jumlahAngka--;  // Mengurangi jumlah angka yang tersisa
  
  return buttonIndex;
}


void resetArray() {
  // Mengisi kembali array angkaMuncul dengan angka dari 0 hingga 10
  for (int i = 0; i <= numButtons; i++) {
    angkaMuncul[i] = i;
  }
  jumlahAngka = sizeof(angkaMuncul) / sizeof(angkaMuncul[0]);
}


void playBuzzer() {
  unsigned long buzzerStartTime = millis();
  while (millis() - buzzerStartTime < 3000) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzerPin, HIGH);  // Menyalakan buzzer
      delay(1000);
      digitalWrite(buzzerPin, LOW);  // Mematikan buzzer
      delay(1000);
    }
  }
}

//===========================DONE=====================================



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
  Serial.begin(9600);  // Inisialisasi Serial Monitor
  pinMode(idle_led, OUTPUT);
  digitalWrite(idle_led, HIGH);
  pinMode(level1_led, OUTPUT);
  pinMode(level2_led, OUTPUT);
  pinMode(buzzerPin, OUTPUT); 
  pinMode(level1, INPUT_PULLUP);  // Mengatur pin tombol start sebagai input dengan pull-up resistor internal
  pinMode(level2, INPUT_PULLUP);  // Mengatur pin tombol start sebagai input dengan pull-up resistor internal
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);  // Mengatur pin tombol sebagai input dengan pull-up resistor internal
    pinMode(ledPins[i], OUTPUT);  // Mengatur pin LED sebagai output
  }
  
  randomSeed(analogRead(0));  // Mengatur seed angka acak menggunakan nilai acak dari pin analog
}


// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {

  // cek tombol level 1
  if (!isTrainingStarted && digitalRead(level1) == LOW && level1_state ==false) {  // Memeriksa apakah tombol start ditekan untuk memulai latihan
      digitalWrite(idle_led, LOW);
      digitalWrite(level1_led,HIGH);
      playBuzzer();
      startTraining();  // Memulai latihan jika tombol level1 ditekan
      interval = 1500;
      level2_state = true;
  }


  // cek tombol level 2
  if (!isTrainingStarted && digitalRead(level2) == LOW && level2_state ==false) {  // Memeriksa apakah tombol start ditekan untuk memulai latihan
      digitalWrite(idle_led, LOW);
      digitalWrite(level2_led,HIGH);
      playBuzzer();
      startTraining();  // Memulai latihan jika tombol level2 ditekan
      interval = 700;
      level1_state = true;
  }
  
  if (isTrainingStarted) {
    unsigned long currentMillis = millis();  // Mendapatkan waktu sekarang
    elapsedTime = currentMillis - startTime;  // Menghitung waktu yang telah berlalu
  //  Serial.println(elapsedTime);Serial.print(",");
    if (elapsedTime - previousMillis >= interval) {
      previousMillis = elapsedTime;  // Menyimpan waktu sekarang sebagai waktu sebelumnya
      
      if (currentButton != -1 && elapsedTime >= interval) {
        Serial.print("X");  // Mencetak "false" pada Serial Monitor jika tidak menekan tombol dalam interval perpindahan tombol
        Serial.println(",");
        currentButton = -1;  // Menandakan tidak ada tombol yang aktif
        salah++;
        resetLEDs();
      }

      if (currentButton == -1) {
        currentButton = getRandomButton();  // Memilih tombol acak yang harus ditekan
   
        digitalWrite(ledPins[currentButton], HIGH);  // Menyalakan LED pada tombol yang harus ditekan
      }
    }



    for (int i = 0; i < numButtons; i++) {
      if (digitalRead(buttonPins[i]) == LOW && currentButton == i) {
        unsigned long buttonTime = elapsedTime;  // Mendapatkan waktu saat tombol ditekan
        menit = (buttonTime / 1000) / 60;
        detik = (buttonTime/ 1000) % 60;
        milidetik = buttonTime % 1000;
        // Menampilkan waktu tombol ditekan di Serial Monitor
        Serial.print("O");Serial.print(",");
        Serial.print(menit);Serial.print(":");Serial.print(detik);Serial.print(":");Serial.print(milidetik);Serial.print(",");Serial.print("Tombol:");Serial.println(i+1);
        benar++;
        digitalWrite(ledPins[currentButton], LOW);  // Mematikan LED pada tombol yang telah ditekan
        currentButton = -1;  // Menandakan tidak ada tombol yang aktif
        benarButtons[benar] = i + 1; // Menyimpan nomor tombol yang ditekan dengan benar ke dalam array benarButtons
        benarTimes[benar] = buttonTime; // Menyimpan waktu tombol ditekan dengan benar ke dalam array benarTimes
        
        break;
      }
    }
    
    if (elapsedTime >= 60000) {  // Jika waktu latihan mencapai 60 detik, latihan selesai
      isTrainingStarted = false;
      digitalWrite(idle_led, HIGH);
      digitalWrite(level1_led,LOW);
      digitalWrite(level2_led,LOW);
      resetLEDs();
      resetArray();
      level1_state = false;
      level2_state = false;
      Serial.print("HASIL:");Serial.print(",");Serial.print("Tombol O:"); Serial.print(benar);Serial.print(",");Serial.print("Tombol X:");Serial.println(salah);
      Serial.println("RESULT O BUTTON");
      for (int i = 1; i <= benar; i++) {
        Serial.print("Tombol:");
        Serial.print(benarButtons[i]);
        menit = (benarTimes[i] / 1000) / 60;
        detik = (benarTimes[i]/ 1000) % 60;
        milidetik = benarTimes[i] % 1000;
        Serial.print(",");
        // Serial.println(benarTimes[i]);
        Serial.print(menit);Serial.print(":");Serial.print(detik);Serial.print(":");Serial.println(milidetik);
      }
    }
  }
}


