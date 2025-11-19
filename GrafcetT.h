/*
 * GrafcetT.h
 *
 * by Tullio Carlassara - 2016 - 2025
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
  static void aggiornaStati();
  
private:
  static int numeroMemorie;
  static int numeroFlags;
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
  bool oldStato;
  bool onEn; // riseUp
  bool onEx; // fallDown
  static int i;
  void aggiorna();
};

//***********************************************************************************************

class FlagT{
public:
  FlagT();
  bool stato;
  bool oldStato;
  bool up; // riseUp
  bool down; // fallDown
  bool change;
  void aggiorna();
  inline void set()   { stato = true; }
  inline void reset() { stato = false; }

  static int i;
};

//***********************************************************************************************

class IngressoT{
public:
  IngressoT(int pin, bool pullUp=false, bool antiRimbalzo=false, unsigned long tempoAntirimbalzo=50, bool invertiLogica=false);
  void setupIngresso();
  void leggi();
  bool stato;
  bool up; // riseUp
  bool down; // fallDown
  bool change; // change
  static int i;
  
private:
  bool flag;
  bool nuovoStato;
  bool oldStato;
  bool pullUp;
  bool antiRimbalzo;
  int pin;
  bool invertiLogica;
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
  inline void set()   { stato = true; }
  inline void reset() { stato = false; }

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
    inline unsigned long getConteggio() { return conteggio; }

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
  inline int getConteggio() { return conteggio; }
  
private:
  int conteggio=0;
  bool oldUp,oldDown;
  int pv;
};

#endif
