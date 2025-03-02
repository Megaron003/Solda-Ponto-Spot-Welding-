/*********************************************
  Autor: Marlon Nardi Walendorff
  Projeto: Solda ponto para baterias de lítio com Arduino
  Detalhes do projeto:https://marlonnardi.com/2023/12/03/como-fazer-solda-ponto-profissional-para-baterias-18650-e-mais-construa-sua-propria-bicicleta-eletrica-1/
  /**********************************************/
 
 
//==================== Inclusão de Bibliotecas =================//
#include <Adafruit_SSD1306.h>
#include <RotaryEncoder.h>
 
//==================== Mapeamento de Hardware ==================//
#define pin_Encoder_CLK 3
#define pin_Encoder_DT 2
#define pin_Encoder_SW 4
 
#define pin_Trigger 5
#define pin_Triac 9
 
#define min_Power_ms 3
#define max_Power_ms 120
 
//==================== Instânciando Objetos ====================//
Adafruit_SSD1306 Display(128, 64, &Wire, -1, 400000, 400000);
RotaryEncoder EncoderOne(pin_Encoder_CLK, pin_Encoder_DT);
 
//==================== Variáveis Globais ==================//
byte aux2 = 0;
 
int16_t valorEncoder = 0;
uint16_t time_ms = 0;
 
 
void setup()
{
 
  Serial.begin(9600);
  //Configura pino como saída
  pinMode(pin_Triac, OUTPUT);
 
  //Configura pino como entrada PULL-UP
  pinMode(pin_Encoder_SW, INPUT_PULLUP);
  //Configura pino como entrada PULL-UP
  pinMode(pin_Trigger, INPUT_PULLUP);
 
 
 
  //================= Interrupção Externa ========================//
  /* Vincula duas interrupções externas no pino 2 e 3 nas funções ISR0 e ISR1
     para garantir que o encoder sempre seja lido com prioridade.*/
  attachInterrupt(digitalPinToInterrupt(2), ISR0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), ISR1, CHANGE);
 
  EncoderOne.setPosition(25); // Energia inicial em 25%
 
  //Inicializa o OLED 128X64 0.96 INCH com endereço I2C 0x3C
  Display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
  Display.setTextColor(WHITE); //Define a cor do texto
 
 
  //Limpa o display, necessário para apagar a imagem inicial da adafruit
  Display.clearDisplay();
  //Atualiza o display
  Display.display();
 
 
 
  Display.setTextSize(1); //Define o tamanho da fonte do texto
  //Posição Largura/Altura
  Display.setCursor(0, 0);
  Display.print("Acesse o projeto em:");
 
  Display.setTextSize(1); //Define o tamanho da fonte do texto
  //Posição Largura/Altura
  Display.setCursor(0, 25);
  Display.print("marlonnardi.com");
 
  Display.display();
 
 
  delay(2000);
 
  Display.clearDisplay();
  Display.display();
 
 
}//endSetup --------------------------------------
 
 
void loop()
{
  trigger();
  screenOne();
}//end_void_loop ----------------------
 
 
 
void trigger()
{
 
  if (digitalRead(pin_Trigger) ) //Se o botão está solto
  {
    aux2 = 1;
  }
 
  if (!digitalRead(pin_Trigger) && aux2 == 1) //Se o botão está pressionado
  {
 
    digitalWrite(pin_Triac, HIGH);
    delay(time_ms);
    digitalWrite(pin_Triac, LOW);
    delay(time_ms);
 
    aux2 = 0;
  }
 
 
}//----------------------- end_selecionaTela
 
 
//================== ISRs Interrupções Externas =======================//
/* Caso qualquer pino do encoder envie sinal, o metodo .tick() sempre será
  chamado, atualizando o valor do encoder via sua biblioteca. */
 
void ISR0()// Função ligada a uma interrupção ISR logo não pode retornar valor e deve ser mais rápida possível
{
  EncoderOne.tick();// Começa a ler o valor do encoder
}//-------------------------endISR0
 
void ISR1()// Função ligada a uma interrupção ISR logo não pode retornar valor e deve ser mais rápida possível
{
  EncoderOne.tick();// Começa a ler o valor do encoder
 
}//------------------------endISR1
 
 
void screenOne()
{
  Display.clearDisplay();
 
  Display.setTextSize(2); //Define o tamanho da fonte do texto
  //Posição Largura/Altura
  Display.setCursor(27, 0);
  Display.print("Energy:");
 
 
  if (valorEncoder <= 9)
  {
    Display.setTextSize(4); //Define o tamanho da fonte do texto
    //Posição Largura/Altura
    Display.setCursor(46, 25);
    Display.print(valorEncoder);
 
    //Posição Largura/Altura
    Display.setCursor(71, 25);
    Display.print("%");
 
  }
 
  if (valorEncoder >= 10 && valorEncoder <= 99)
  {
 
    Display.setTextSize(4); //Define o tamanho da fonte do texto
    //Posição Largura/Altura
    Display.setCursor(31, 25);
    Display.print(valorEncoder);
 
    //Posição Largura/Altura
    Display.setCursor(81, 25);
    Display.print("%");
 
  }
 
  if (valorEncoder >= 100)
  {
 
    Display.setTextSize(4); //Define o tamanho da fonte do texto
    //Posição Largura/Altura
    Display.setCursor(18, 25);
    Display.print(valorEncoder);
 
    //Posição Largura/Altura
    Display.setCursor(92, 25);
    Display.print("%");
 
  }
 
 
 
 
  valorEncoder = EncoderOne.getPosition();//Captura o valor do encoder
 
  if (EncoderOne.getPosition() < 1)
  {
    EncoderOne.setPosition(1);
    valorEncoder = 1;
  }
 
  if (EncoderOne.getPosition() > 100)
  {
    EncoderOne.setPosition(100);
    valorEncoder = 100;
  }
 
 
  time_ms = map(valorEncoder, 1, 100, min_Power_ms, max_Power_ms);
 
  Serial.println(time_ms);
 
 
  Display.display();
 
}//end_screenOne ----------------------
