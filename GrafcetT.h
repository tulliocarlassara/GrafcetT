/*
 * GrafcetT.h
 *
 * by Tullio Carlassara - 2016
 *
 * This library is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
 */

#ifndef GRAFCETT_H
#define GRAFCETT_H

class GrafcetT{
public:
  static void inizializza();
  static void acquisizioneIngressi();
  static void pubblicazioneUscite();
  
private:
  static int numeroIngressi;
  static int numeroUscite;
  static int numeroTimerTon;
  static int numeroSub;
  static int numeroCounterUpDown;
  static unsigned long newTime;
};

//***********************************************************************************************

class MemoriaT{
public:
  MemoriaT();
  bool stato;
  static int i;
};

//***********************************************************************************************

class IngressoT{
public:
  IngressoT(int pin, bool pullUp=false, bool antiRimbalzo=false, unsigned long tempoAntirimbalzo=50);
  void setupIngresso();
  void leggi();
  bool stato;
  static int i;
  
private:
  bool flag;
  bool nuovoStato;
  bool pullUp;
  bool antiRimbalzo;
  int pin;
  unsigned long tempoAntirimbalzo;
  unsigned long oldMillis;
};

//***********************************************************************************************

class UscitaT{
public:
  UscitaT(int pin);
  bool stato;
  void setupUscita();
  void scrivi();
  static int i;
  
private:
  int pin;
};

//***********************************************************************************************

class TimerTonT{
  public:
    TimerTonT(unsigned long pt);
    bool stato;
    bool in;
    void tempo(unsigned long&);
    static int i;
    
  private:
    unsigned long conteggio=0;
    unsigned long oldTime;
    unsigned long pt;
};

//***********************************************************************************************

class SubT{
public:
  SubT(void (*funzione)(), MemoriaT& memoriaInizio, MemoriaT& memoriaFine);
  bool stato;
  bool in;
  static int i;
  void esegui();
  
private:
  void (*funzione)();
  MemoriaT* memoriaInizio,* memoriaFine;
  bool attivaPrimaMem=true;
};

//***********************************************************************************************

class CounterUpDownT{
public:
  CounterUpDownT(int pv);
  bool stato;
  bool up;
  bool down;
  bool reset;
  static int i;
  void conta();
private:
  int conteggio=0;
  bool oldUp,oldDown;
  int pv;
};

#endif
