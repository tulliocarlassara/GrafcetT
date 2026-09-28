/*
 * GrafcetT.h
 *
 * by Tullio Carlassara - 2016 - 2026
 *
 * This library is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.
 */

#ifndef GRAFCETT_H
#define GRAFCETT_H

#include <stdint.h>

class GrafcetT{
public:
  static void inizializza();
  static void acquisizioneIngressi();
  static void pubblicazioneUscite();
  static void aggiornaStati();
  static unsigned long newTime;
  // Abilitare dopo inizializza(); false evita la misura a ogni ciclo.
  static bool scanEnabled;
  // Intervallo tra acquisizioni successive, in microsecondi (include il debug).
  // La prima acquisizione inizializza il riferimento e lascia i tempi a zero.
  static uint32_t scanTime;
  static uint32_t maxScanTime;
  // Acquisizioni misurate dall'inizializzazione; riparte da zero dopo UINT32_MAX.
  static uint32_t scanCounter;
  
private:
  static uint32_t lastScanMicros;
  static bool scanStarted;
  static int numeroMemorie;
  static int numeroFlags;
  static int numeroIngressi;
  static int numeroUscite;
  static int numeroTimerTon;
  static int numeroSub;
  static int numeroCounterUpDown;
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
  void leggi(unsigned long newTime);
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
  bool oldStato;
};

//***********************************************************************************************

class TimerTonT{
  public:
    TimerTonT(unsigned long pt);
    bool stato;
    bool in;
    void tempo(unsigned long newTime);
    static int i;
    inline void setPt(unsigned long t) { pt = t; }
    inline unsigned long getConteggio() { return conteggio; }
    inline void setConteggio(unsigned long t) { conteggio = t; }

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
  bool finito=false;
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
  inline void setPv(int c) { pv = c; }
  inline int getConteggio() { return conteggio; }
  inline void setConteggio(int c) { conteggio = c; }
  
private:
  int conteggio=0;
  bool oldUp,oldDown;
  int pv;
};

#endif
