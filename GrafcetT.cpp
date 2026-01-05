/*
 * GrafcetT.cpp
 *
 * by Tullio Carlassara - 2016 - 2025
 *
 * This library is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
 */

#include "GrafcetT.h"
#include <Arduino.h>

MemoriaT** memorie=NULL;
FlagT** flags=NULL;
IngressoT** ingressi=NULL;
UscitaT** uscite=NULL;
TimerTonT** timer=NULL;
SubT** subroutines=NULL;
CounterUpDownT** counterUpDown=NULL;

int GrafcetT::numeroMemorie=0;
int GrafcetT::numeroFlags=0;
int GrafcetT::numeroIngressi=0;
int GrafcetT::numeroUscite=0;
int GrafcetT::numeroTimerTon=0;
int GrafcetT::numeroSub=0;
int GrafcetT::numeroCounterUpDown=0;
unsigned long GrafcetT::newTime=0;

template <typename T>
T incrementaArray(int dimensioneFinale, T arrayIniziale){
  T arrayFinale = reinterpret_cast<T>(new void*[dimensioneFinale]);
  for(int i=0;i<dimensioneFinale-1;i++){
    arrayFinale[i]=arrayIniziale[i];
  }
  delete[] arrayIniziale;
  return arrayFinale;
}

//***********************************************************************************************

void GrafcetT::inizializza(){
  numeroMemorie=MemoriaT::i;
  numeroFlags=FlagT::i;
  numeroIngressi=IngressoT::i;
  numeroUscite=UscitaT::i;
  numeroSub=SubT::i;
  numeroTimerTon=TimerTonT::i;
  numeroCounterUpDown=CounterUpDownT::i;
  newTime=0;
  
  for(int i=0;i<numeroIngressi; i++){
    ingressi[i]->setupIngresso();
  }
  for(int i=0; i<numeroUscite; i++){
    uscite[i]->setupUscita();
  }  
}

void GrafcetT::acquisizioneIngressi(){
  //acquisizione tempo
  newTime = millis();

  //acquisizione ingressi
  for(int i=0; i<numeroIngressi; i++){
    ingressi[i]->leggi(newTime);
  }

  //aggiornamento timer
  for(int i=0; i<numeroTimerTon; i++){
    timer[i]->tempo(newTime);
  }

  //aggiornamento counter
  for(int i=0; i<numeroCounterUpDown; i++){
    counterUpDown[i]->conta();
  }

  //chiamata sub;
  for(int i=0; i<numeroSub; i++){
    subroutines[i]->esegui();
  }
}

void GrafcetT::pubblicazioneUscite(){
  //pubblicazione uscite
  for(int i=0; i<numeroUscite; i++){
    uscite[i]->scrivi();
  }
}

void GrafcetT::aggiornaStati(){
  // aggiornaMemorie e calcola onEn, onEx
  for(int i=0; i<numeroMemorie; i++){
    memorie[i]->aggiorna();
  }
  // aggiornaFlags e calcola up, down e change
  for(int i=0; i<numeroFlags; i++){
    flags[i]->aggiorna();
  }
}

//***********************************************************************************************

int MemoriaT::i=0;

MemoriaT::MemoriaT(){
  
  stato=false;
  oldStato=false;
  onEn=false;
  onEx=false;
  
  memorie=incrementaArray(i+1,memorie);
  memorie[i]=this;
  i++;
}

void MemoriaT::aggiorna(){
  onEn=(stato && !oldStato);
  onEx=(!stato && oldStato);
  oldStato=stato;
}

//***********************************************************************************************

int FlagT::i=0;

FlagT::FlagT(){
  stato = false;
  oldStato = false;
  up = false;
  down = false;
  change=false;

  flags=incrementaArray(i+1,flags);
  flags[i]=this;
  i++;
}

void FlagT::aggiorna(){
  up = (stato && !oldStato);
  down = (!stato && oldStato);
  change = (up || down);
  oldStato=stato;
}

//***********************************************************************************************

int IngressoT::i=0;

IngressoT::IngressoT(int pin, bool pullUp, bool antiRimbalzo, unsigned long tempoAntirimbalzo, bool invertiLogica){
  this->pin = pin;
  this->pullUp = pullUp;
  this->antiRimbalzo = antiRimbalzo;
  this->tempoAntirimbalzo=tempoAntirimbalzo;
  this->invertiLogica=invertiLogica;
  flag=false;

  ingressi=incrementaArray(i+1,ingressi);
  ingressi[i]=this;
  i++;
}

void IngressoT::setupIngresso(){
  if(!pullUp) pinMode(pin, INPUT);
  else  pinMode(pin, INPUT_PULLUP);

  stato = digitalRead(pin);
  if(invertiLogica) stato = !stato;

  oldStato = stato;
  up = down = change = false;
}


void IngressoT::leggi(unsigned long newTime){
  if(!antiRimbalzo) {
    stato = digitalRead(pin);
    if(invertiLogica) stato = !stato;
  } else {
    nuovoStato = digitalRead(pin);
    if(invertiLogica) nuovoStato = !nuovoStato;

    if(!flag && (nuovoStato != stato)) {
      stato = nuovoStato;
      oldMillis = newTime;
      flag = true;
    }
    if(flag && newTime - oldMillis >= tempoAntirimbalzo)
      flag = false;
  }

  up=(stato && !oldStato);
  down=(!stato &&  oldStato);
  change=(up || down);
  oldStato=stato;
}


//***********************************************************************************************

int UscitaT::i=0;

UscitaT::UscitaT(int pin){
  this->pin=pin;
  stato=false;

  uscite=incrementaArray(i+1,uscite);
  uscite[i]=this;
  i++;
}

void UscitaT::setupUscita(){
  pinMode(pin,OUTPUT);
}

void UscitaT::scrivi(){
  digitalWrite(pin,stato);
}

//***********************************************************************************************

int TimerTonT::i=0;

TimerTonT::TimerTonT(unsigned long pt){
  this->pt=pt;

  timer=incrementaArray(i+1,timer);
  timer[i]=this;
  i++;
}
    
void TimerTonT::tempo(unsigned long newTime){
  if(in) conteggio = newTime-oldTime;
  else {
    conteggio=0;
    oldTime=newTime;
  }
  if (conteggio>=pt) stato=true;
  else stato=false;
}

//***********************************************************************************************

int SubT::i=0;

SubT::SubT(void(*funzione)(), MemoriaT& memoriaInizio, MemoriaT& memoriaFine){
  this->funzione=funzione;
  this->memoriaInizio=&memoriaInizio;
  this->memoriaFine=&memoriaFine;

  subroutines=incrementaArray(i+1,subroutines);
  subroutines[i]=this;
  i++;
}

void SubT::esegui(){
  if(in){
    if(attivaPrimaMem){
      memoriaInizio->stato=true;
      attivaPrimaMem=false;
    }
    funzione();
  }
  else{
    memoriaFine->stato=false;
    attivaPrimaMem=true;
  }
  if(memoriaFine->stato) stato=true;
  else stato=false;
}

//***********************************************************************************************

int CounterUpDownT::i=0;

CounterUpDownT::CounterUpDownT(int pv){
  this->pv=pv;
  
  counterUpDown=incrementaArray(i+1,counterUpDown);
  counterUpDown[i]=this;
  i++;
}

void CounterUpDownT::conta(){
  if(up && !oldUp)conteggio++;
  oldUp=up;
  if(down && !oldDown)conteggio--;
  oldDown=down;
  if(reset) conteggio=0;
  if(conteggio>=pv) stato=true;
  else stato=false;
}

