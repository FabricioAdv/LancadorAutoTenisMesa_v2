//-Informacoes de progamacao-----------------------------------------

//-Valores EEPROM----------------------------------------------------

//-EEPROM da programacao-
//EEPROM posicao 1 - Valor salvo da posicao do servo motor 1 (Nao utilizado)
//EEPROM posicao 2 - Valor salvo da posicao do servo motor 2 (Nao utilizado)
//EEPROM posicao 3 - Valor da maior pontuacao
//EEPROM posicao 4 - Valor da maior sequencia de bolinhas rebatidas
//EEPROM posicao 5 - Valor da percentagem de acertos

//-EEPROM das configuracoes-
//EEPROM posicao 10 - Valor salvo da configuracoes do grau de dificuldade dos lancamentos
//EEPROM posicao 11 - Valor salvo da configuracoes de lancamento, randomico ou nao randomico
//EEPROM posicao 12 - Valor salvo do tempo de lancamento
//EEPROM posicao 13 - Fator de multiplicacao do tempo de lancamento
//EEPROM posicao 14 - Valor salvo do tempo de espera por rebatida
//EEPROM posicao 15 - Fator de multiplicacao do tempo de espera por rebatida

//-------------------------------------------------------------------

//-Pinagem-----------------------------------------------------------

//-Analogico-
//NUMERO DO PINO         |NOME NA PROGRAMACAO      |DESCRICAO DE FUNCAO
//A0                      sens_laser_ldr            Recebimento do sinal do LDR para computacao de pontos

//-Digital-
//NUMERO DO PINO         |NOME NA PROGRAMACAO      |DESCRICAO DE FUNCAO
//2                       sens_laser                Recebimento do sinal do LDR para computacao de pontos
//3                       ponto                     Envio de sinal para amostragem no display de 7 segmentos

//-PWM-
//NUMERO DO PINO         |NOME NA PROGRAMACAO      |DESCRICAO DE FUNCAO
//8                       motor1                    Controle de rotacao do motor de lancamento 1
//9                       motor2                    Controle de rotacao do motor de lancamento 2

//-Servos motores-
//NUMERO DO PINO         |NOME NA PROGRAMACAO      |DESCRICAO DE FUNCAO
//11                      servo1                    Contrle de posicao do servo para o lancamento 1
//12                      servo2                    Contrle de posicao do servo para o lancamento 2

//-Motor de passo-
//NUMERO DO PINO         |NOME NA PROGRAMACAO      |DESCRICAO DE FUNCAO
//23                      
//25                      
//27                      
//29                      

//-------------------------------------------------------------------

//-------------------------------------------------------------------


//-Inclusao de bibliotecas-------------------------------------------

#include <Servo.h>
#include <Stepper.h>
#include <EEPROM.h>

//-------------------------------------------------------------------


//-Definicoes--------------------------------------------------------

#define grau_pul 1.8

//-------------------------------------------------------------------


//-Pinos-------------------------------------------------------------

//-Analogico-
#define sens_laser_ldr A0

//-Digital-
#define sens_laser 2
#define ponto 3

//-PWM-
#define motor1 8
#define motor2 9

//-------------------------------------------------------------------


//-Variaveis---------------------------------------------------------

int GRF_auto = 0;
int GRF_man = 0;

int grau_sm1;
int grau_sm2;
int esc_process = 0;
String leitura = "";
String esc_menu = "principal";

boolean ball_rebatida = false;
boolean acerto = false;
boolean erro = false;

int ponto_atual = 0;
int ponto_acumulado = 0;
int lancamentos_acumulados = 0;
int sequencia = 0;
int acertos_atual = 0;
int acertos_atual_percent = 0;
int percent;

long tempo_inicial;
long tempo_decor1;
long tempo_decor2;

long tempo_inicial_a;
long tempo_decor_aj;
int tempo_ajuda = 5000;
boolean exe_ja = false;
boolean exe_ja2 = true;
boolean menu_ativ = false;

int pos_sm1;
int pos_ant1 = 0;
int pos_fim1 = 0;

int pos_sm2;
int pos_ant2 = 0;
int pos_fim2 = 0;

int pwm_mot1 = 0;
int pwm_mot2 = 0;

boolean calc = false;
boolean calcP = false;

int pross = 0;
boolean prossP = false;
boolean prossPP = false;

int config_esc_random;
int config_esc_tempo;

int man_pos1;
int man_pos2;
int man_pwm;

int test_mot = 0;

int steps360 = 192;
int steps90 = 12;

String lanca = "";
String seq = "";
String acertos = "";
String acertos_percent = "";

String maior_seq = "";
String maior_pontuacao = "";

String maior_percent = "";

//-------------------------------------------------------------------


//-Definicoes de programacao-----------------------------------------

Servo servo1;
Servo servo2;
Stepper motPass(steps360, 29, 27, 25, 23);

//-------------------------------------------------------------------


//-Sub rotinas-------------------------------------------------------

//-Controle do motor de passo----------------------------------------

void ctrl_motor_pass(int steps)
{
  Serial.println(F("Movimentando motor de passo..."));
    
  motPass.step(steps);

  Serial.println(F("Motor de passo na posicao\n"));
}

//-------------------------------------------------------------------

//-Controle dos servos-----------------------------------------------

boolean serv_motor_ang1(int grau)
{
  Serial.println(F("Movimentando servo motor..."));

  ctrl_vel_serv_motor1(grau, 40);

  if (servo1.read() == grau)
  {
    Serial.println(F("Servo motor na posicao\n"));

    return 1;

    EEPROM.update(1, 0);
    EEPROM.update(1, grau);
  }
}

boolean serv_motor_ang2(int grau)
{
  Serial.println(F("Movimentando servo motor..."));

  ctrl_vel_serv_motor2(grau, 40);

  if (servo2.read() == grau)
  {
    Serial.println(F("Servo motor na posicao\n"));

    return 1;

    EEPROM.update(2, 0);
    EEPROM.update(2, grau);
  }
}

void ctrl_vel_serv_motor1(int ang, int tempo)
{
  pos_fim1 = ang;

  if (pos_fim1 > pos_ant1)
  {
    for (pos_sm1 = pos_ant1; pos_sm1 <= pos_fim1; pos_sm1++)
    {
      servo1.write(pos_sm1);
      delay(tempo);
    }
  }
  else if (pos_fim1 < pos_ant1)
  {
    for (pos_sm1 = pos_ant1; pos_sm1 >= pos_fim1; pos_sm1--)
    {
      servo1.write(pos_sm1);
      delay(tempo);
    }
  }

  pos_ant1 = pos_fim1;
}

void ctrl_vel_serv_motor2(int ang, int tempo)
{
  pos_fim2 = ang;

  if (pos_fim2 > pos_ant2)
  {
    for (pos_sm2 = pos_ant2; pos_sm2 <= pos_fim2; pos_sm2++)
    {
      servo2.write(pos_sm2);
      delay(tempo);
    }
  }
  else if (pos_fim2 < pos_ant2)
  {
    for (pos_sm2 = pos_ant2; pos_sm2 >= pos_fim2; pos_sm2--)
    {
      servo2.write(pos_sm2);
      delay(tempo);
    }
  }

  pos_ant2 = pos_fim2;
}

//-------------------------------------------------------------------

//-Controle de PWM dos motores de lancamento-------------------------

void ctrl_pwm_mot(int variavel)
{
  if (variavel)
  {
    pwm_mot1 = random(200,230);
    pwm_mot2 = random(200,230);

    ctrl_pwm_mot1(pwm_mot1);
    ctrl_pwm_mot2(pwm_mot2);
  }
  else
  {
    pwm_mot1 = random(200,230);
    pwm_mot2 = pwm_mot1;

    ctrl_pwm_mot1(pwm_mot1);
    ctrl_pwm_mot2(pwm_mot2);
  }
}

void ctrl_pwm_mot1(int pwm)
{
  analogWrite(motor1, pwm);
}

void ctrl_pwm_mot2(int pwm)
{
  analogWrite(motor2, pwm);
}

//-------------------------------------------------------------------

//-Controle de pontuacao---------------------------------------------

void laser_acerto()
{
  tempo_decor1 = millis() - tempo_inicial;

  int val_ldr = analogRead(sens_laser_ldr);
  val_ldr = map(val_ldr, 0, 1023, 0, 255);

  if (((digitalRead(sens_laser)) || val_ldr < 85) && GRF_auto == 4 && (tempo_decor1 < (EEPROM.read(14) * EEPROM.read(15))))
  {
    Serial.println(F("Bolinha rebatida"));
    Serial.println(F("Computando pontos..."));
    Serial.println(F("Resetando lancamentos...\n"));

    delay (2000);

    acerto = true;

    GRF_auto = 1;

    ball_rebatida = true;
  }

  else if (GRF_auto == 4 && tempo_decor1 > ((EEPROM.read(14) * EEPROM.read(15))))
  {
    Serial.println(F("Erro do jogador ao rebater"));
    Serial.println(F("Resetando lancamentos...\n"));
    
    delay (2000);

    erro = true;

    GRF_auto = 1;

    ball_rebatida = true;
  }
}

void pontos()
{
  lancamentos_acumulados++;
  
  if (acerto)
  {
    ponto_atual++;
    ponto_acumulado++;
    

    if (ponto_atual > sequencia)
    {
      sequencia = ponto_atual;
    }

    if (ponto_atual > EEPROM.read(4))
    {
      EEPROM.update(4, ponto_atual);
    }

    if (ponto_acumulado > EEPROM.read(3))
    {
      EEPROM.update(3, ponto_acumulado);
    }

    acerto = false;
  }

  if (erro)
  {
    ponto_atual = 0;

    erro = false;
  }

  percent = (ponto_acumulado * pow(lancamentos_acumulados, -1)) * 100;
}

//-------------------------------------------------------------------

//-Comunicacao serial------------------------------------------------

//-Menu--------------------------------------------------------------

void menu_geral()
{
  tempo_decor_aj = millis() - tempo_inicial_a;

  if (exe_ja && !GRF_auto && (tempo_decor_aj > tempo_ajuda))
  {
    menu_ativ = true;
  }

  if (((esc_menu == "principal") && exe_ja2) || ((esc_menu == "principal") && menu_ativ))
  {
    menu_principal();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
  else if (((esc_menu == "automatico") && exe_ja2) || ((esc_menu == "automatico") && menu_ativ))
  {
    menu_automatico();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
  else if (((esc_menu == "manual") && exe_ja2) || ((esc_menu == "manual") && menu_ativ))
  {
    menu_manual();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
  else if (((esc_menu == "configuracoes") && exe_ja2) || ((esc_menu == "configuracoes") && menu_ativ))
  {
    menu_configuracoes();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
//  else if (((esc_menu == "config_dific") && exe_ja2) || ((esc_menu == "config_dific") && menu_ativ))
//  {
//    menu_config_dific();
//
//    menu_ativ = false;
//    exe_ja = false;
//    exe_ja2 = false;
//  }
  else if (((esc_menu == "config_lanca") && exe_ja2) || ((esc_menu == "config_lanca") && menu_ativ))
  {
    menu_config_lanca();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
  else if (((esc_menu == "config_lanca_rand") && exe_ja2) || ((esc_menu == "config_lanca_rand") && menu_ativ))
  {
    menu_config_lanca_rand();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
  else if (((esc_menu == "config_lanca_temp_lanca") && exe_ja2) || ((esc_menu == "config_lanca_temp_lanca") && menu_ativ))
  {
    menu_config_lanca_temp_lanca();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
  else if (((esc_menu == "config_lanca_temp_reb") && exe_ja2) || ((esc_menu == "config_lanca_temp_reb") && menu_ativ))
  {
    menu_config_lanca_temp_reb();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
  else if (((esc_menu == "manual_val") && exe_ja2) || ((esc_menu == "manual_val") && menu_ativ))
  {
    menu_manual_val();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
  else if (((esc_menu == "manual_rand") && exe_ja2) || ((esc_menu == "manual_rand") && menu_ativ))
  {
    menu_manual_rand();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
  else if (((esc_menu == "estatisticas") && exe_ja2) || ((esc_menu == "estatisticas") && menu_ativ))
  {
    menu_estatisticas();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
  else if (((esc_menu == "teste") && exe_ja2) || ((esc_menu == "teste") && menu_ativ))
  {
    menu_teste();

    menu_ativ = false;
    exe_ja = false;
    exe_ja2 = false;
  }
}

void menu_principal()
{
  Serial.println(F("- Menu - Principal ---------------------------------------------------"));
  Serial.println(F("- Opcoes de lancamento -----------------------------------------------"));
  Serial.println(F("Digite \"automatico\" para acessar o menu de lancamentos automatico"));
  Serial.println(F("Digite \"manual\" para acessar o menu de lancamentos manual"));
  Serial.println(F("----------------------------------------------------------------------"));
  Serial.println(F("- Opcoes de posicionamento -------------------------------------------"));
  Serial.println(F("Digite \"home\" para enviar os motores para a posicao inicial"));
  Serial.println(F("Digite \"teste\" para iniciar os testes de ciclo"));
  Serial.println(F("----------------------------------------------------------------------"));
  Serial.println(F("- Opcoes de dados guardados ------------------------------------------"));
  Serial.println(F("Digite \"estatisticas\" para mostrar a pontuacao do jogo"));
  Serial.println(F("----------------------------------------------------------------------"));
  Serial.println(F("- Configuracoes ------------------------------------------------------"));
  Serial.println(F("Digite \"configuracoes\" para acessar o menu de configuracoes"));
  Serial.println(F("----------------------------------------------------------------------"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

void menu_automatico()
{
  Serial.println(F("- Menu - Automatico --------------------------------------------------"));
  Serial.println(F("--Parametros configurados--"));
  
  Serial.println("Deficiencia: " + (String) EEPROM.read(10));

  if (EEPROM.read(11) == 1)
  {
     Serial.println(F("Lancamento randomico: Sim"));
  }
  else if (EEPROM.read(11) == 0)
  {
     Serial.println(F("Lancamento randomico: Nao"));
  }
  
  Serial.println("Tempo de lancamento: " + (String) (EEPROM.read(12) * EEPROM.read(13)) + " ms");
  Serial.println("Tempo de rebatida: " + (String) (EEPROM.read(14) * EEPROM.read(15)) + " ms\n");
  
  Serial.println(F("Digite \"ligar\" para iniciar os lancamentos automaticos"));
  Serial.println(F("Digite \"desligar\" para interromper os lancamentos automaticos\n"));
  
  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

void menu_manual()
{
  Serial.println(F("- Menu - Manual ------------------------------------------------------"));
  
  Serial.println(F("--Parametros configurados--"));

  if (EEPROM.read(11) == 1)
  {
     Serial.println(F("Lancamento randomico: Sim\n"));
  }
  else if (EEPROM.read(11) == 0)
  {
     Serial.println(F("Lancamento randomico: Nao\n"));
  }
  
  Serial.println(F("Digite \"valores\" para iniciar a insercao de valores aleatorios de posicoes"));
  Serial.println(F("Digite \"randomico\" para iniciar os lancamentos em valores randomicos\n"));
  
  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

void menu_manual_val()
{
  Serial.println(F("- Menu - Manual - Valores ---------------------------------------------\n"));

  Serial.println(F("\"Necessario entrada de valores para o lancamento\"\n"));
  
  Serial.println(F("Dados a serem inseridos:"));
  Serial.println(F("1) Digite o valor de posicao do primeiro motor de passo: (90 a 110)"));
  Serial.println(F("2) Digite o valor de posicao do segundo motor de passo: (0 a 60)"));
  Serial.println(F("3) Digite o valor de PWM para os motores de lancamento: (0 a 255)"));
  Serial.println(F("4) Digite \"lancar\" para iniciar o lancamento com os valores inseridos\n"));

  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

void menu_manual_rand()
{
  Serial.println(F("- Menu - Manual - Randomico ------------------------------------------\n"));

  Serial.println(F("\"Sem necessidade da entrada de valores de posicao, os valores sao sorteados aleatoriamente\"\n"));

  man_pos1 = random(90, 120);
  man_pos2 = random(0, 60);
  man_pwm = random(0, 255);

  Serial.println("Posicao do motor 1 sorteada: " + (String) man_pos1);
  Serial.println("Posicao do motor 2 sorteada: " + (String) man_pos2);
  Serial.println("Velocidade do motor de lancamento sorteada: " + (String) man_pwm + "\n");
  
  Serial.println(F("Dados a serem inseridos:"));
  Serial.println(F("1) Digite \"lancar\" para iniciar o lancamento com os valores sorteados\n"));

  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

void menu_configuracoes()
{
  Serial.println(F("- Menu - Configuracoes -----------------------------------------------"));
  Serial.println(F("Digite \"dificuldade\" para acessar as configuracoes de grau de dificuldade - (incompleto)"));
  Serial.println(F("Digite \"lancamentos\" para acessar as configuracoes de lancamentos"));
  Serial.println(F("Digite \"resetar dados\" para resetar todos os dados salvos\n"));

  Serial.println(F("Digite \"avancados\" para acessar as configuracoes avancadas - (incompleto)\n"));
  
  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

//void menu_config_dific()
//{
//  Serial.println(F("- Menu - Configuracoes - Dificuldade ---------------------------------\n"));
//
//  Serial.println("Dado atual salvo na EEPROM: " + (String) EEPROM.read(10) + "\n");
//
//  Serial.println(F("Digite o nivel de dificuldade:"));  
//  Serial.println(F("Digite \"alto\" para selecionar o grau de dificuldade alto"));
//  Serial.println(F("Digite \"medio\" para selecionar o grau de dificuldade medio"));
//  Serial.println(F("Digite \"baixo\" para selecionar o grau de dificuldade baixo\n"));
//  
//  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
//  Serial.println(F("----------------------------------------------------------------------\n"));
//}

void menu_config_lanca()
{
  Serial.println(F("- Menu - Configuracoes - Lancamentos ---------------------------------"));
  Serial.println(F("Digite \"randomico\" para acessar as configuracoes de lancamentos randomicos"));
  Serial.println(F("Digite \"tempo lancamento\" para acessar as alterar o tempo de lancamento"));
  Serial.println(F("Digite \"tempo rebatida\" para acessar as alterar o tempo de rebatida\n"));
  
  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

//void menu_config_avancada()
//{
//  Serial.println(F("- Menu - Configuracoes - Avancadas -----------------------------------"));
//  Serial.println(F("Digite \"pwm\" para configurar a velocidade de lancamento no modo automatico"));
//  Serial.println(F("Digite \"...\""));
//  Serial.println(F("Digite \"...\"\n"));
//  
//  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
//  Serial.println(F("----------------------------------------------------------------------\n"));
//}

void menu_estatisticas()
{
  if (!GRF_auto)
  {
    lanca = (String) lancamentos_acumulados;
    seq = (String) sequencia;
    acertos = (String) ponto_acumulado;
    acertos_percent = (String) percent;

    maior_seq = (String) EEPROM.read(4);
    maior_pontuacao = (String) EEPROM.read(3);
    
    if (percent > EEPROM.read(5))
    {
      EEPROM.update(5, percent);
    }

    maior_percent = (String) EEPROM.read(5);

    Serial.println(F("- Estatisticas de jogo -----------------------------------------------"));
    Serial.println("Quantidade de lancamentos nesta partida: " + lanca);
    Serial.println("Maior sequencia nesta partida: " + seq);
    Serial.println("Quantidade de acertos nesta partida: " + acertos);
    Serial.println("Percentual de acertos nesta partida: " + acertos_percent + "\n");

    Serial.println("Maior sequencia de pontos: " + maior_seq);
    Serial.println("Maior potuacao: " + maior_pontuacao);
    Serial.println("Maior percentual de acertos: " + maior_percent + "\n");

    Serial.println(F("Digite \"resetar estatisticas\" para resetar as pontuacoes do jogo\n"));

    Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));      
    Serial.println(F("----------------------------------------------------------------------\n"));
  }
  else if (GRF_auto)
  {
    Serial.println(F("O sistema se encontra iniciado"));
    Serial.println(F("Interrompa-o para resetar as estatisticas\n"));
  }

  lanca = "";
  seq = "";
  acertos = "";
  acertos_percent = "";
  maior_seq = "";
  maior_pontuacao = "";
  maior_percent = "";
}

void menu_teste()
{
  Serial.println(F("- Menu - Teste -------------------------------------------------------"));
  Serial.println(F("Digite \"1\" para testar o servo motor 1 (posicao)"));
  Serial.println(F("Digite \"2\" para testar o servo motor 2 (posicao)"));
  Serial.println(F("Digite \"3\" para testar o motor de lancamento 1"));
  Serial.println(F("Digite \"4\" para testar o motor de lancamento 2"));
  Serial.println(F("Digite \"5\" para testar o servo motor 3 (dosagem)\n"));
  
  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

void menu_config_lanca_rand()
{
  Serial.println(F("- Menu - Configuracoes - Lancamentos - Randomico ---------------------\n"));

  if (EEPROM.read(11) == 1)
  {
     Serial.println(F("Dado atual salvo na EEPROM: Sim\n"));
  }
  else if (EEPROM.read(11) == 0)
  {
     Serial.println(F("Dado atual salvo na EEPROM: Nao\n"));
  }

  Serial.println(F("Digite \"sim\" para ativar os valores randomicos nos motores de lancamento"));
  Serial.println(F("Digite \"nao\" para desativar os valores randomicos nos motores de lancamento\n"));
  
  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

void menu_config_lanca_temp_lanca()
{
  Serial.println(F("- Menu - Configuracoes - Lancamentos - Tempo de lancamento -----------\n"));

  Serial.println("Dado atual salvo na EEPROM (Tempo): " + (String) EEPROM.read(12) + " ms");
  Serial.println("Dado atual salvo na EEPROM (Fator de multiplicacao): " + (String) EEPROM.read(13));
  Serial.println("Resultado do calculo: " + (String) (EEPROM.read(12) * EEPROM.read(13)) + " ms\n");

  Serial.println(F("1) Digite o tempo de lancamento em milisegundos (ms): (max 255)"));
  Serial.println(F("2) Digite o fator de multiplicacao: (max 255)\n"));
  
  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

void menu_config_lanca_temp_reb()
{
  Serial.println(F("- Menu - Configuracoes - Lancamentos - Tempo de rebatida -------------\n"));

  Serial.println("Dado atual salvo na EEPROM (Tempo): " + (String) EEPROM.read(14) + " ms");
  Serial.println("Dado atual salvo na EEPROM (Fator de multiplicacao): " + (String) EEPROM.read(15));
  Serial.println("Resultado do calculo: " + (String) (EEPROM.read(14) * EEPROM.read(15)) + " ms\n");
  
  Serial.println(F("1) Digite o tempo de rebatida em milisegundos (ms): (max 255)"));
  Serial.println(F("2) Digite o fator de multiplicacao: (max 255)\n"));
  
  Serial.println(F("Digite \"voltar\" para retornar ao menu anterior"));
  Serial.println(F("----------------------------------------------------------------------\n"));
}

//-------------------------------------------------------------------

void serialEvent()
{
  while (Serial.available())
  {
    leitura = Serial.readString();

    Serial.println("Dado recebido: " + leitura);

    if (((leitura == "Automatico") || (leitura == "automatico") || (leitura == "AUTOMATICO")) && (esc_menu == "principal"))
    {
      esc_menu = "automatico";
      
      exe_ja2 = true;

      leitura = "";
    }
    else if (((leitura == "Manual") || (leitura == "manual") || (leitura == "MANUAL")) && (esc_menu == "principal"))
    {
      esc_menu = "manual";
      
      exe_ja2 = true;

      leitura = "";
    }
    else if (((leitura == "Home") || (leitura == "home") || (leitura == "HOME")) && (esc_menu == "principal"))
    {
      if (GRF_auto == 0)
      {
        Serial.println(F("Enviando para a posicao de HOME..."));

        serv_motor_ang1(0);

        serv_motor_ang2(0);


        if (serv_motor_ang1 && serv_motor_ang2)
        {
          EEPROM.update(1, 0);

          EEPROM.update(2, 0);

          Serial.println(F("Posicao de HOME alcancada\n"));
        }
      }
      else
      {
        Serial.println(F("O sistema se encontra iniciado"));
        Serial.println(F("Interrompa-o para resetar o sistema\n"));
      }

      exe_ja2 = true;

      leitura = "";
    }
    else if (((leitura == "Teste") || (leitura == "teste") || (leitura == "TESTE")) && (esc_menu == "principal"))
    {
      esc_menu = "teste";
      
      exe_ja2 = true;

      leitura = "";
    }
    else if (((leitura == "Estatisticas") || (leitura == "estatisticas") || (leitura == "ESTATISTICAS")) && (esc_menu == "principal"))
    {
      esc_menu = "estatisticas";

      exe_ja2 = true;
      
      leitura = "";
    }
    else if (((leitura == "Configuracoes") || (leitura == "configuracoes") || (leitura == "CONFIGURACOES")) && (esc_menu == "principal"))
    {
      esc_menu = "configuracoes";
      
      exe_ja2 = true;

      leitura = "";
    }

    else if (((leitura != "Voltar") && (leitura != "voltar") && (leitura != "VOLTAR")) && (esc_menu == "teste"))
    {
      if (leitura == "1")
      {
        test_mot = 1;
      
        exe_ja2 = true;
      }
      else if (leitura == "2")
      {
        test_mot = 2;
      
        exe_ja2 = true;
      }
      else if (leitura == "3")
      {
        test_mot = 3;
      
        exe_ja2 = true;
      }
      else if (leitura == "4")
      {
        test_mot = 4; 
      
        exe_ja2 = true;
      }
      else if (leitura == "5")
      {
        test_mot = 5;
      
        exe_ja2 = true;
      }
      else
      {
        Serial.println(F("Entrada de dados incorreta\n"));
      }

      teste();

      leitura = "";
    }
    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "teste"))
    {
      esc_menu = "principal";
      
      exe_ja2 = true;

      leitura = "";
    }
    
    else if (((leitura == "Resetar Estatisticas") || (leitura == "resetar estatisticas") || (leitura == "RESETAR ESTATISTICAS") || (leitura == "resetar Estatisticas")) && (esc_menu == "estatisticas"))
    {
      if (!GRF_auto)
      {
        Serial.println(F("Resentando estatisticas..."));
        
        lancamentos_acumulados = 0;
        sequencia = 0;
        percent = 0;
        ponto_acumulado = 0;
        ponto_atual = 0;
        
        EEPROM.update(3, 0);
        EEPROM.update(4, 0);
        EEPROM.update(5, 0);

        Serial.println(F("Estatisticas resetadas\n"));
      }
      else if (GRF_auto)
      {
        Serial.println(F("O sistema se encontra iniciado"));
        Serial.println(F("Interrompa-o para resetar o sistema\n"));
      }
      
      exe_ja2 = true;

      leitura = "";
    }
    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "estatisticas"))
    {
      esc_menu = "principal";
      
      exe_ja2 = true;

      leitura = "";
    }


    else if (((leitura == "Ligar") || (leitura == "ligar") || (leitura == "LIGAR")) && (esc_menu == "automatico"))
    {
      if (!GRF_auto)
      {
        Serial.println(F("Iniciando lancamentos..."));

        GRF_auto = 1;

        Serial.println(F("Lancamentos iniciados\n"));
      }
      else
      {
        Serial.println(F("O sistema ja se encontra iniciado\n"));
      }

      exe_ja = true;
  
      leitura = "";
    }
    else if (((leitura == "Desligar") || (leitura == "desligar") || (leitura == "DESLIGAR")) && (esc_menu == "automatico"))
    {
      if (GRF_auto)
      {
        Serial.println(F("Interrompendo lancamentos...\n"));

        GRF_auto = 0;

        serv_motor_ang1(100);

        serv_motor_ang2(0);

        ctrl_pwm_mot1(0);
        ctrl_pwm_mot2(0);

        Serial.println(F("Lancamentos interrompidos\n"));

        tempo_inicial_a = millis();
      }
      else
      {
        Serial.println(F("O sistema ja se encontra interrompido\n"));
      }

      esc_process = 0;
    
      leitura = "";
    }
    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "automatico"))
    {
      if (GRF_auto)
      {
        Serial.println(F("O sistema se encontra iniciado"));
        Serial.println(F("Interrompa-o para voltar ao menu principal\n"));
      }
      else
      {
        esc_menu = "principal";
      
        exe_ja2 = true;

        leitura = "";
      }
    }


    else if (((leitura == "Valores") || (leitura == "valores") || (leitura == "VALORES")) && (esc_menu == "manual"))
    {
      esc_menu = "manual_val";

      exe_ja2 = true;
      
      leitura = "";
    }
    else if (((leitura == "Randomico") || (leitura == "randomico") || (leitura == "RANDOMICO")) && (esc_menu == "manual"))
    {
      esc_menu = "manual_rand";

      exe_ja2 = true;
      
      leitura = "";
    }
    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "manual"))
    {
      esc_menu = "principal";

      exe_ja2 = true;

      leitura = "";
    }


    else if ((((leitura != "Voltar") && (leitura != "voltar") && (leitura != "VOLTAR")) && ((leitura != "Lancar") && (leitura != "lancar") && (leitura != "LANCAR"))) && (esc_menu == "manual_val"))
    {
      if (!pross)
      {        
        Serial.println("Dado a ser inserido na posicao do servo motor 1: " + (String) leitura.toInt() + " graus");
        
        if ((leitura.toInt() < 90) || (leitura.toInt() > 110))
        {
          Serial.println(F("\nA posicao inserida esta fora do range de lancamento\n"));
        }
        else
        {
          man_pos1 = leitura.toInt();
        
          Serial.println(F("Posicao salva\n"));
          
          pross = 1;

          prossP = true;

          prossPP = true;
        }
      }
      else if (pross == 1)
      {
        Serial.println("Dado a ser inserido na posicao do servo motor 2: " + (String) leitura.toInt() + " graus");
        
        if ((leitura.toInt() < 0) || (leitura.toInt() > 60))
        {
          Serial.println(F("\nA posicao inserida esta fora do range de lancamento\n"));
        }
        else
        {
          man_pos2 = leitura.toInt();
        
          Serial.println(F("Posicao salva\n"));
          
          pross = 2;
        }
      }
      else if (pross == 2)
      {
        Serial.println("Dado a ser inserido no PWM do motor de lancamento: " + (String) leitura.toInt());
        
        if ((leitura.toInt() < 0) || (leitura.toInt() > 255))
        {
          Serial.println(F("\nO valor inserido esta fora do \n"));
        }
        else
        {
          man_pwm = leitura.toInt();
        
          Serial.println(F("Velocidade salva\n"));
          
          prossP = false;
          
          pross = 0;
        }
      }
      
      leitura = "";
    }
    else if (((leitura == "Lancar") || (leitura == "lancar") || (leitura == "LANCAR")) && (esc_menu == "manual_val") && !prossP)
    {
      GRF_man = 1;

      prossPP = false;
      
      leitura = "";
    }
    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "manual_val") && !prossPP)
    {
      esc_menu = "manual";

      exe_ja2 = true;

      leitura = "";
    }


    else if (((leitura == "Lancar") || (leitura == "lancar") || (leitura == "LANCAR")) && (esc_menu == "manual_rand"))
    {
      GRF_man = 1;
      
      leitura = "";
    }
    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "manual_rand"))
    {
      esc_menu = "manual";

      exe_ja2 = true;

      leitura = "";
    }



//    else if (((leitura == "Deficiencia") || (leitura == "deficiencia") || (leitura == "DEFICIENCIA")) && (esc_menu == "configuracoes"))
//    {
//      esc_menu = "config_defic";
//
//      exe_ja2 = true;
//
//      leitura = "";
//    }
    else if (((leitura == "Lancamentos") || (leitura == "lancamentos") || (leitura == "LANCAMENTOS")) && (esc_menu == "configuracoes"))
    {
      esc_menu = "config_lanca";

      exe_ja2 = true;

      leitura = "";
    }
    else if (((leitura == "Resetar Dados") || (leitura == "resetar dados") || (leitura == "RESETAR DADOS") || (leitura == "resetar Dados")) && (esc_menu == "configuracoes"))
    {
      Serial.println(F("Resentando dados..."));

      ponto_atual = 0;
      ponto_acumulado = 0;
      lancamentos_acumulados = 0;

      EEPROM.update(3, 0);
      EEPROM.update(4, 0);
      EEPROM.update(5, 0);

      EEPROM.update(10, 0);
      EEPROM.update(11, 0);
      EEPROM.update(12, 1);
      EEPROM.update(13, 1);
      EEPROM.update(14, 1);
      EEPROM.update(15, 1);

      Serial.println(F("Dados resetados\n"));
      
      exe_ja2 = true;
      
      leitura = "";
    }
    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "configuracoes"))
    {
      esc_menu = "principal";

      exe_ja2 = true;

      leitura = "";
    }
    

//    else if (((leitura == "Alto") || (leitura == "alto") || (leitura == "ALTO")) && (esc_menu == "config_dific"))
//    {
//      EEPROM.update(10, 1);
//
//      Serial.println(F("Dados gravados na EEPROM\n"));
//        
//      esc_menu = "config_dific";
//
//      exe_ja2 = true;
//
//      leitura = "";
//    }
//    else if (((leitura == "Medio") || (leitura == "medio") || (leitura == "MEDIO")) && (esc_menu == "config_dific"))
//    {
//      EEPROM.update(10, 2);
//
//      Serial.println(F("Dados gravados na EEPROM\n"));
//      
//      esc_menu = "config_dific";
//
//      exe_ja2 = true;
//
//      leitura = "";
//    }
//    else if (((leitura == "Baixo") || (leitura == "baixo") || (leitura == "BAIXO")) && (esc_menu == "config_dific"))
//    {
//      EEPROM.update(10, 3);
//
//      Serial.println(F("Dados gravados na EEPROM\n"));
//      
//      esc_menu = "config_dific";
//
//      exe_ja2 = true;
//
//      leitura = "";
//    }
//    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "config_defic"))
//    {
//      esc_menu = "configuracoes";
//      exe_ja2 = true;
//
//      leitura = "";
//    }
    

    else if (((leitura == "Randomico") || (leitura == "randomico") || (leitura == "RANDOMICO")) && (esc_menu == "config_lanca"))
    {
      esc_menu = "config_lanca_rand";

      exe_ja2 = true;
      
      leitura = "";
    }
    else if (((leitura == "Tempo lancamento") || (leitura == "tempo lancamento") || (leitura == "TEMPO LANCAMENTO") || (leitura == "tempo Lancamento")) && (esc_menu == "config_lanca"))
    {
      esc_menu = "config_lanca_temp_lanca";

      exe_ja2 = true;
      
      leitura = "";
    }
    else if (((leitura == "Tempo rebatida") || (leitura == "tempo rebatida") || (leitura == "TEMPO REBATIDA") || (leitura == "tempo Rebatida")) && (esc_menu == "config_lanca"))
    {
      esc_menu = "config_lanca_temp_reb";

      exe_ja2 = true;
      
      leitura = "";
    }
    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "config_lanca"))
    {
      esc_menu = "configuracoes";
      
      exe_ja2 = true;

      leitura = "";
    }
    

    else if (((leitura == "Sim") || (leitura == "sim") || (leitura == "SIM")) && (esc_menu == "config_lanca_rand"))
    {
      EEPROM.update(11,1);

      Serial.println(F("Dados gravados na EEPROM\n"));

      exe_ja2 = true;
      
      leitura = "";
    }
    else if (((leitura == "Nao") || (leitura == "nao") || (leitura == "NAO")) && (esc_menu == "config_lanca_rand"))
    {
      EEPROM.update(11,0);

      Serial.println(F("Dados gravados na EEPROM\n"));
      
      exe_ja2 = true;
      
      leitura = "";
    }
    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "config_lanca_rand"))
    {
      esc_menu = "config_lanca";
      
      exe_ja2 = true;

      leitura = "";
    }


    else if (((leitura != "Voltar") && (leitura != "voltar") && (leitura != "VOLTAR")) && (esc_menu == "config_lanca_temp_lanca"))
    {
      if (!calc)
      {
        Serial.println("Dado a ser calculado como tempo: " + (String) leitura.toInt() + "ms");
        
        if (leitura.toInt() <= 0)
        {
          Serial.println(F("\nO dado inserido presisa ser maior que 0 ms\n"));
        }
        else
        {
          EEPROM.update(12,leitura.toInt());
        
          Serial.println(F("Dados gravados na EEPROM\n"));

          calcP = true;

          calc = true;
        }
      }
      else
      {
        Serial.println("Dado a ser calculado como fator de multiplicacao: " + (String) leitura.toInt());
        
        if (leitura.toInt() <= 0)
        {
          Serial.println(F("\nO dado inserido presisa ser maior que 0\n"));
        }
        else
        {
          EEPROM.update(13,leitura.toInt());
        
          Serial.println(F("Dados gravados na EEPROM\n"));
          
          calcP = false;
          
          calc = false;
          
          exe_ja2 = true;
        }
      }
      
      leitura = "";
    }
    else if ((((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "config_lanca_temp_lanca")) && !calcP)
    {
      esc_menu = "config_lanca";
      
      exe_ja2 = true;

      leitura = "";
    }


    else if (((leitura != "Voltar") && (leitura != "voltar") && (leitura != "VOLTAR")) && (esc_menu == "config_lanca_temp_reb"))
    {
      if (!calc)
      {       
        Serial.println("Dado a ser calculado como tempo: " + (String) leitura.toInt() + "ms");
        
        if (leitura.toInt() <= 0)
        {
          Serial.println(F("\nO dado inserido presisa ser maior que 0 ms\n"));
        }
        else
        {
          EEPROM.update(14,leitura.toInt());
        
          Serial.println(F("Dados gravados na EEPROM\n"));

          calcP = true;

          calc = true;
        }
      }
      else
      {
        Serial.println("Dado a ser calculado como fator de multiplicacao: " + (String) leitura.toInt());
        
        if (leitura.toInt() <= 0)
        {
          Serial.println(F("\nO dado inserido presisa ser maior que 0\n"));
        }
        else
        {
          EEPROM.update(15,leitura.toInt());
        
          Serial.println(F("Dados gravados na EEPROM\n"));

          calcP = false;
          
          calc = false;
      
          exe_ja2 = true;
        }
      }
      
      leitura = "";
    }
    else if ((((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "config_lanca_temp_reb")) && !calcP)
    {
      esc_menu = "config_lanca";
      
      exe_ja2 = true;

      leitura = "";
    }


    else if (((leitura == "Sim") || (leitura == "sim") || (leitura == "SIM")) && (esc_menu == "config_avancada"))
    {
      esc_menu = "config_avancada";
      
      exe_ja2 = true;

      leitura = "";
    }
    else if (((leitura == "Nao") || (leitura == "nao") || (leitura == "NAO")) && (esc_menu == "config_avancada"))
    {
      esc_menu = "config_avancada";
      
      exe_ja2 = true;

      leitura = "";
    }
    else if (((leitura == "Voltar") || (leitura == "voltar") || (leitura == "VOLTAR")) && (esc_menu == "config_lanca_rand"))
    {
      esc_menu = "config_avancada";
      
      exe_ja2 = true;

      leitura = "";
    }


    else
    {
      Serial.println(F("Entrada de dados incorreta\n"));

      leitura = "";
    }
  }
}

//-------------------------------------------------------------------

//-Lancamento automatico---------------------------------------------

void automatico()
{
  if (GRF_auto == 1)
  {
    ctrl_pwm_mot1(0);
    ctrl_pwm_mot2(0);

    ball_rebatida = false;

    GRF_auto = 2;
  }

  else if (GRF_auto == 2)
  {
    grau_sm1 = random (95, 100);

    Serial.println("Angulo randomico do servo motor 1: " + (String) grau_sm1);
    
    serv_motor_ang1(grau_sm1);

    if (serv_motor_ang1)
    {
      GRF_auto = 3;
    }
  }

  else if (GRF_auto == 3)
  {
    grau_sm2 = random (10, 50);

    Serial.println("Angulo randomico do servo motor 2: " + (String) grau_sm2);

    serv_motor_ang2(grau_sm2);

    if (serv_motor_ang2)
    {
      GRF_auto = 4;
    }
  }

  else if (GRF_auto == 4)
  {
    if (serv_motor_ang1 && serv_motor_ang2)
    {
      Serial.println(F("Lancando bolinha..."));

      ctrl_pwm_mot(EEPROM.read(11));

      delay (EEPROM.read(12) * EEPROM.read(13));

      ctrl_motor_pass(steps90);

      Serial.println(F("Bolinha lancada\n"));

      Serial.println(F("Aguardando rebate...\n"));

      tempo_inicial = millis();

      do
      {
        laser_acerto();
      }
      while (!ball_rebatida);

      pontos();
    }
  }
}

//-------------------------------------------------------------------

//-Lancamento manual-------------------------------------------------

void manual()
{
  if (GRF_man == 1)
  {
    ctrl_pwm_mot1(0);
    ctrl_pwm_mot2(0);

    GRF_man = 2;
  }

  else if (GRF_man == 2)
  {

    Serial.println("Angulo randomico do servo motor 1: " + (String) man_pos1);

    serv_motor_ang1(man_pos1);

    if (serv_motor_ang1)
    {
      GRF_man = 3;
    }
  }

  else if (GRF_man == 3)
  {
    Serial.println("Angulo randomico do servo motor 2: " +  (String) man_pos2);

    serv_motor_ang2(man_pos2);

    if (serv_motor_ang2)
    {
      GRF_man = 4;
    }
  }

  else if (GRF_man == 4)
  {
    if (serv_motor_ang1 && serv_motor_ang2)
    {
      Serial.println(F("Lancando bolinha..."));

      ctrl_pwm_mot1(man_pwm);
      ctrl_pwm_mot2(man_pwm);

      delay(2000);

      ctrl_motor_pass(steps90);

      delay(2000);

      Serial.println(F("Bolinha lancada\n"));

      ctrl_pwm_mot1(0);
      ctrl_pwm_mot2(0);

      GRF_man = 0;

      exe_ja2 = true;
    }
  }
}

//-------------------------------------------------------------------

//-Teste de componentes----------------------------------------------

void teste()
{
  if (test_mot == 1)
  {
    Serial.println(F("Testando servo motor 1 (posicao)\n"));

    serv_motor_ang1(100);

    delay(2000);

    if (serv_motor_ang1)
    {
      serv_motor_ang1(110);
    }

    delay(2000);

    if (serv_motor_ang1)
    {
      serv_motor_ang1(90);
    }
    
    delay(2000);

    if (serv_motor_ang1)
    {
      serv_motor_ang1(100);
      
      Serial.println(F("Teste finalizado\n"));
      
      test_mot = 0;
    }    
  }
  else if (test_mot == 2)
  {
    Serial.println(F("Testando servo motor 2 (posicao)\n"));

    serv_motor_ang2(60);

    delay(2000);

    if (serv_motor_ang2)
    {
      serv_motor_ang2(0);
    }

    delay(2000);

    if (serv_motor_ang2)
    {
      serv_motor_ang2(0);
      
      Serial.println(F("Teste finalizado\n"));
      
      test_mot = 0;
    }    
  }
  else if (test_mot == 3)
  {
    Serial.println(F("Testando motor de lancamento 1\n"));

    Serial.println(F("Acelerando..."));
    for (int i = 0 ; i <= 255 ; i++)
    {
      ctrl_pwm_mot1(i);
      
      delay (20);
    }
    Serial.println(F("Acelerado\n"));
    
    delay (2000);

    Serial.println(F("Desacelerando..."));    
    for (int i = 255 ; i >= 0 ; i--)
    {
      ctrl_pwm_mot1(i);
      
      delay (20);
    }
    Serial.println(F("Desacelerado\n")); 
    
    Serial.println(F("Teste finalizado\n"));

    test_mot = 0;
  }
  else if (test_mot == 4)
  {
    Serial.println(F("Testando motor de lancamento 2\n"));

    Serial.println(F("Acelerando..."));
    for (int i = 0 ; i <= 255 ; i++)
    {
      ctrl_pwm_mot2(i);
      
      delay (20);
    }
    Serial.println(F("Acelerado\n"));
    
    delay (2000);

    Serial.println(F("Desacelerando..."));    
    for (int i = 255 ; i >= 0 ; i--)
    {
      ctrl_pwm_mot2(i);
      
      delay (20);
    }
    Serial.println(F("Desacelerado\n"));

    Serial.println(F("Teste finalizado\n"));

    ctrl_pwm_mot1(0);
    ctrl_pwm_mot2(0);

    test_mot = 0;
  }
  else if (test_mot == 5)
  {
    Serial.println(F("Testando servo motor 3 (dosagem)\n"));

    delay(500);
    
    ctrl_motor_pass(steps90);

    delay(500);

    ctrl_motor_pass(steps90);

    delay(500);
    
    ctrl_motor_pass(steps90);

    delay(500);
    
    ctrl_motor_pass(steps90);

    delay(500);
    
    ctrl_motor_pass(steps360);

    delay(500);

    Serial.println(F("Teste finalizado\n"));

    test_mot = 0;
  }
}

//-------------------------------------------------------------------

//-------------------------------------------------------------------

//-------------------------------------------------------------------


//-Configuracao de pinos---------------------------------------------

void config_pins()
{
  pinMode(sens_laser_ldr, INPUT);
  pinMode(sens_laser, INPUT);
  pinMode(ponto, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
}

//-------------------------------------------------------------------

//-Programacao-------------------------------------------------------

void setup()
{
  Serial.begin(9600);

  while (!Serial)
  {
    ;
  }

  Serial.println(F("Iniciando...\n"));

  Serial.println(F("Configurando pinos..."));

  config_pins();

  servo1.attach(11);
  servo2.attach(12);
  motPass.setSpeed(30);

  serv_motor_ang1(100);
  delay(1000);
  serv_motor_ang2(0);

  Serial.println(F("Pinos configurados\n"));

  Serial.println(F("Iniciado\n"));
}

void loop()
{
  menu_geral();

  automatico();
  manual();
}

//-------------------------------------------------------------------
