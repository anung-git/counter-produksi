#ifndef ANDROID_H
#define ANDROID_H
#include <Arduino.h>
#include "Rtc.h"
#include "Eprom.h"
#include "Alamat.h"
#include "DMD.h"
#include "Addres.h"

#include "Counter.h"

#define MY_TARGET_ADDR 500
#define MY_ACTUAL_ADDR 502
// #define MY_TARGET_ADDR 500
// #define MY_ACTUAL_ADDR 502

class Android
{
private:
    DMD *mydmd;
    Counter *Target; // Counter(0, 9999);
    Counter *Actual; // Counter(0, 9999);
    Rtc waktu = Rtc();
    Eprom eprom = Eprom();
    Alamat alamat = Alamat();
    uint8_t cmd;
    WiFiUDP Udp;

    Addres senin = Addres(0);
    Addres selasa = Addres(20);
    Addres rabu = Addres(40);
    Addres kamis = Addres(60);
    Addres jumat = Addres(80);
    Addres sabtu = Addres(100);
    Addres minggu = Addres(120);

    const unsigned int localUdpPort = 4210; // local port to listen on
    char incomingPacket[255];               // buffer for incoming packets
    bool redy;

    uint8_t *playstate;

    void setPlay();
    void setTime();
    void setSenin();
    void setSelasa();
    void setRabu();
    void setKamis();
    void setJumat();
    void setSabtu();
    void setMinggu();
    void resetActual();
    void resetTarget();
    void clear(uint8_t length);

    void initActual();
    void initTarget();
    void setTimer();

public:
    void setPlayState(uint8_t *state);
    void setDMD(DMD *panel);
    void setActual(Counter *aktual);
    void setTarget(Counter *target);
    Android();
    void run();
    unsigned int command();
    String value();
    bool available();
    String getMsg();
    ~Android();
};
#endif