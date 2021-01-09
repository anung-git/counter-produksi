
#include <Arduino.h>
#include "Rtc.h"

// senin 1
// selasa 2
// rabu 3
// kamis 4
// jumat 5
// sabtu 6
// minggu 7
unsigned char Rtc::makedec(unsigned char bcd)
{
    return ((bcd / 16 * 10) + (bcd % 16));
}
unsigned char Rtc::makebcd(unsigned char dec)
{
    return ((dec / 10 * 16) + (dec % 10));
}

void Rtc::setTime(unsigned char jam, unsigned char menit, unsigned char detik)
{
    Wire.beginTransmission(104);
    Wire.write(0);
    Wire.write(Rtc::makebcd(detik));
    Wire.write(Rtc::makebcd(menit));
    Wire.write(Rtc::makebcd(jam));
    Wire.endTransmission();
    Wire.beginTransmission(104);
    Wire.write(0x0e);
    Wire.write(0); //enable sqw 1hz
    Wire.endTransmission();
    delay(10);
}

void Rtc::getTime(unsigned char &jam, unsigned char &menit, unsigned char &detik)
{
    Wire.beginTransmission(104);
    Wire.write(0); // MSB
    Wire.endTransmission();
    if (Wire.requestFrom(104, 3) == 3)
    {
        detik = Rtc::makedec(Wire.read());
        menit = Rtc::makedec(Wire.read());
        jam = Rtc::makedec(Wire.read());
    }
}

unsigned char Rtc::getHari(unsigned char d, unsigned char m, int y)
{
    const unsigned char t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    unsigned char h = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
    if (h == 0)
        h = 7;
    return h;
}

void Rtc::setTanggal(unsigned char tanggal, unsigned char bulan, int tahun)
{
    unsigned char hari = Rtc::getHari(tanggal, bulan, tahun);
    Wire.beginTransmission(104);
    Wire.write(3);
    Wire.write(makebcd(hari));
    Wire.write(makebcd(tanggal));
    Wire.write(makebcd(bulan));
    Wire.write(makebcd(tahun - 2000));
    Wire.endTransmission();
    Wire.beginTransmission(104);
    Wire.write(0x0e);
    Wire.write(0); //enable sqw 1hz
    Wire.endTransmission();

    delay(10);
}
void Rtc::getTanggal(unsigned char &hari, unsigned char &tanggal, unsigned char &bulan, int &tahun)
{
    Wire.beginTransmission(104);
    Wire.write(3); // MSB
    Wire.endTransmission();
    if (Wire.requestFrom(104, 4) == 4)
    {
        hari = Rtc::makedec(Wire.read());
        tanggal = Rtc::makedec(Wire.read());
        bulan = Rtc::makedec(Wire.read());
        tahun = Rtc::makedec(Wire.read()) + 2000;
    }
}
Rtc::Rtc()
{
    // Wire.begin(5,4);
    Wire.begin();
}

Rtc::~Rtc()
{
}
