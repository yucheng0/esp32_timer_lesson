portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile bool Led1State = 0;  //volatile ?????????
bool Led2State = 0;
hw_timer_t * timer;   // 這個指標很重要在指向計時器的

//LED1會在計時中斷觸發時閃爍，LED2會在LOOP中一直閃爍
#define LED1PIN 2     // 這個感覺是板子內部燈
#define LED2PIN 32    // 這個是我外拉的

// 中斷服務程式 -----> 中斷後執行的程式

void IRAM_ATTR TimerISR(){
  //注意，在中斷中不要出現Serial.print
  
  portENTER_CRITICAL(&mux);    // 類似push 把所有暫存器都保留起來

  Led1State = !Led1State;      // 將LED反相
  digitalWrite(LED1PIN,Led1State); // 內部燈也會閃

  portEXIT_CRITICAL(&mux);  // 類似pop  把所有的暫存器還原
  
}
void setup() {

  Serial.begin(115200);
  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
  // 針對Time的設定中斷...... 它是Time幾呢?

  timer = timerBegin(0, 80, true);   // 0 代表Time0  預除80 , 以80Mhz/80 = 1MHz = 1us;   true 是上緣觸發; false 是下緣觸發
  timerAttachInterrupt(timer, &TimerISR, true);   // timer 是上方所定義的指標 , TimerISR是中斷服務程式
  timerAlarmWrite(timer, 1000000, true);    // 100000 每1秒1次t true 是否重複觸發
  timerAlarmEnable(timer);    // 啟動Timer0中斷不`停止
  delay(1000);
}

void loop() {
  
  delay(2000);
  Led2State = !Led2State;
  Serial.println(Led2State);
  digitalWrite(LED2PIN,Led2State);
  
}