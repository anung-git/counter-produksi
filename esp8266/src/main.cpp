#include <Arduino.h>
#include <ESP8266WiFi.h> //memasukan library NodeMCU
#include <WiFiUdp.h>
#include <Ticker.h>

#include "Android.h"
#include "Alamat.h"
#include "Break.h"
#include "DMD.h"
#include "Eprom.h"
#include "Event.h"
#include "Rtc.h"
#include "Counter.h"
#include "Font.h"
#include "kecil.h"
#include "Button.h"
#include "Addres.h"

const char *ssid = "ANDON_03";     //nama SSID maksimal 31 karakter
const char *password = "12345678"; // password SSID minimal 8 karakter / NULL untuk tanpa password

#define DISPLAYS_ACROSS 3
#define DISPLAYS_DOWN 1

#define TARGET_ADDR 500
#define ACTUAL_ADDR 502
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN, D5, D6, D7, D8);

Android android = Android();
Rtc waktu = Rtc();
Eprom eprom = Eprom();
Event second = Event(1000);
Event setengahDetik = Event(500);
Event OnePcs = Event(1500);
Alamat alamat = Alamat();
Break Istirahat1 = Break();
Break Istirahat2 = Break();
Break Istirahat3 = Break();
Break jamKerja = Break();

Counter Target = Counter(0, 9999);
Counter Actual = Counter(0, 9999);
Button *Tombol1 = new Button(D0);
Button *Tombol2 = new Button(D3);
Button *Tombol3 = new Button(D4);

Addres senin = Addres(0);
Addres selasa = Addres(20);
Addres rabu = Addres(40);
Addres kamis = Addres(60);
Addres jumat = Addres(80);
Addres sabtu = Addres(100);
Addres minggu = Addres(120);

Ticker blinker;
//inisialisasi variabel
uint8_t playState = 1;
uint8_t jam, menit, detik;
uint8_t tanggal, bulan, hari;
int tahun;
uint8_t tempTanggal;
uint16_t plan;
unsigned int tmpPlan, tmpAct, tmpTar;
uint8_t refres;
// ICACHE_RAM_ATTR
void TimerHandler()
{
  dmd.scanDisplayBySPI();
}
// unsigned float angka;

void intToArray(char *bufer, int val, uint8_t start)
{
  char ribuan = val / 1000;
  char ratusan = (val % 1000) / 100;
  char puluhan = (val % 100) / 10;
  char satuan = val % 10;

  bufer[start] = ribuan + '0';
  bufer[start + 1] = ratusan + '0';
  bufer[start + 2] = puluhan + '0';
  bufer[start + 3] = satuan + '0';
}
void loadData()
{
  delay(10);
  if (hari == alamat.SENIN)
  {
    jamKerja.setStart(eprom.readInt(0x50, senin.mulai()));
    Istirahat1.setStart(eprom.readInt(0x50, senin.istirahat1()));
    Istirahat1.setStop(eprom.readInt(0x50, senin.masuk1()));
    Istirahat2.setStart(eprom.readInt(0x50, senin.istirahat2()));
    Istirahat2.setStop(eprom.readInt(0x50, senin.masuk2()));
    Istirahat3.setStart(eprom.readInt(0x50, senin.istirahat3()));
    Istirahat3.setStop(eprom.readInt(0x50, senin.masuk3()));
    jamKerja.setStop(eprom.readInt(0x50, senin.selesai()));
    plan = eprom.readInt(0x50, senin.plan());
    Target.getFromEprom(tanggal, bulan, TARGET_ADDR); //alamat.TARGET_SENIN);
  }
  if (hari == alamat.SELASA)
  {
    jamKerja.setStart(eprom.readInt(0x50, selasa.mulai()));
    Istirahat1.setStart(eprom.readInt(0x50, selasa.istirahat1()));
    Istirahat1.setStop(eprom.readInt(0x50, selasa.masuk1()));
    Istirahat2.setStart(eprom.readInt(0x50, selasa.istirahat2()));
    Istirahat2.setStop(eprom.readInt(0x50, selasa.masuk2()));
    Istirahat3.setStart(eprom.readInt(0x50, selasa.istirahat3()));
    Istirahat3.setStop(eprom.readInt(0x50, selasa.masuk3()));
    jamKerja.setStop(eprom.readInt(0x50, selasa.selesai()));
    plan = eprom.readInt(0x50, selasa.plan());
    Target.getFromEprom(tanggal, bulan, TARGET_ADDR); //alamat.TARGET_SELASA);
  }
  if (hari == alamat.RABU)
  {
    jamKerja.setStart(eprom.readInt(0x50, rabu.mulai()));
    Istirahat1.setStart(eprom.readInt(0x50, rabu.istirahat1()));
    Istirahat1.setStop(eprom.readInt(0x50, rabu.masuk1()));
    Istirahat2.setStart(eprom.readInt(0x50, rabu.istirahat2()));
    Istirahat2.setStop(eprom.readInt(0x50, rabu.masuk2()));
    Istirahat3.setStart(eprom.readInt(0x50, rabu.istirahat3()));
    Istirahat3.setStop(eprom.readInt(0x50, rabu.masuk3()));
    jamKerja.setStop(eprom.readInt(0x50, rabu.selesai()));
    plan = eprom.readInt(0x50, rabu.plan());
    Target.getFromEprom(tanggal, bulan, TARGET_ADDR); //alamat.TARGET_RABU);
  }
  if (hari == alamat.KAMIS)
  {
    jamKerja.setStart(eprom.readInt(0x50, kamis.mulai()));
    Istirahat1.setStart(eprom.readInt(0x50, kamis.istirahat1()));
    Istirahat1.setStop(eprom.readInt(0x50, kamis.masuk1()));
    Istirahat2.setStart(eprom.readInt(0x50, kamis.istirahat2()));
    Istirahat2.setStop(eprom.readInt(0x50, kamis.masuk2()));
    Istirahat3.setStart(eprom.readInt(0x50, kamis.istirahat3()));
    Istirahat3.setStop(eprom.readInt(0x50, kamis.masuk3()));
    jamKerja.setStop(eprom.readInt(0x50, kamis.selesai()));
    plan = eprom.readInt(0x50, kamis.plan());
    Target.getFromEprom(tanggal, bulan, TARGET_ADDR); //alamat.TARGET_KAMIS);
  }
  if (hari == alamat.JUMAT)
  {
    jamKerja.setStart(eprom.readInt(0x50, jumat.mulai()));
    Istirahat1.setStart(eprom.readInt(0x50, jumat.istirahat1()));
    Istirahat1.setStop(eprom.readInt(0x50, jumat.masuk1()));
    Istirahat2.setStart(eprom.readInt(0x50, jumat.istirahat2()));
    Istirahat2.setStop(eprom.readInt(0x50, jumat.masuk2()));
    Istirahat3.setStart(eprom.readInt(0x50, jumat.istirahat3()));
    Istirahat3.setStop(eprom.readInt(0x50, jumat.masuk3()));
    jamKerja.setStop(eprom.readInt(0x50, jumat.selesai()));
    plan = eprom.readInt(0x50, jumat.plan());
    Target.getFromEprom(tanggal, bulan, TARGET_ADDR); //alamat.TARGET_JUMAT);
  }
  if (hari == alamat.SABTU)
  {
    jamKerja.setStart(eprom.readInt(0x50, sabtu.mulai()));
    Istirahat1.setStart(eprom.readInt(0x50, sabtu.istirahat1()));
    Istirahat1.setStop(eprom.readInt(0x50, sabtu.masuk1()));
    Istirahat2.setStart(eprom.readInt(0x50, sabtu.istirahat2()));
    Istirahat2.setStop(eprom.readInt(0x50, sabtu.masuk2()));
    Istirahat3.setStart(eprom.readInt(0x50, sabtu.istirahat3()));
    Istirahat3.setStop(eprom.readInt(0x50, sabtu.masuk3()));
    jamKerja.setStop(eprom.readInt(0x50, sabtu.selesai()));
    plan = eprom.readInt(0x50, sabtu.plan());
    Target.getFromEprom(tanggal, bulan, TARGET_ADDR); //alamat.TARGET_SABTU);
  }
  if (hari == alamat.AHAD)
  {
    jamKerja.setStart(eprom.readInt(0x50, minggu.mulai()));
    Istirahat1.setStart(eprom.readInt(0x50, minggu.istirahat1()));
    Istirahat1.setStop(eprom.readInt(0x50, minggu.masuk1()));
    Istirahat2.setStart(eprom.readInt(0x50, minggu.istirahat2()));
    Istirahat2.setStop(eprom.readInt(0x50, minggu.masuk2()));
    Istirahat3.setStart(eprom.readInt(0x50, minggu.istirahat3()));
    Istirahat3.setStop(eprom.readInt(0x50, minggu.masuk3()));
    jamKerja.setStop(eprom.readInt(0x50, minggu.selesai()));
    plan = eprom.readInt(0x50, minggu.plan());
    Target.getFromEprom(tanggal, bulan, TARGET_ADDR); //alamat.TARGET_AHAD);
  }

  unsigned long timer;
  timer = 1000 * eprom.readInt(0x50, alamat.CYCLE_TIME_S);
  timer += eprom.readInt(0x50, alamat.CYCLE_TIME_MS);
  // Serial.print("timer = ");
  // Serial.println(timer);
  OnePcs.setEvent(timer);
}
void setup()
{
  Serial.begin(9600);
  dmd.selectFont(Font);
  android.setDMD(&dmd);
  android.setActual(&Actual);
  android.setTarget(&Target);
  WiFi.softAP(ssid, password); //Memulai membuat acces point
  // Serial.print("Access Point \"");
  // Serial.print(ssid);
  // Serial.println("\" started");
  // Serial.print("IP address:\t");
  // Serial.println(WiFi.softAPIP());
  delay(100);
  yield();
  dmd.beepOn();
  setengahDetik.start();
  second.start();
  OnePcs.start();
  waktu.getTime(jam, menit, detik);
  waktu.getTanggal(hari, tanggal, bulan, tahun);
  Target.getFromEprom(tanggal, bulan, TARGET_ADDR);
  Actual.getFromEprom(tanggal, bulan, ACTUAL_ADDR);
  // OnePcs.setEvent();
  // Scaning.attachInterruptInterval(2500, TimerHandler);
  // Scaning.disableTimer();
  //Initialize Ticker every 0.5s
  // waktu.setTime(05, 59, 0);
  blinker.attach_ms(2, TimerHandler);
  // waktu.getTime(jam, menit, detik);
  // waktu.getTanggal(hari, tanggal, bulan, tahun);
  loadData();
  tempTanggal = eprom.readByte(0x50, alamat.TEMP_TANGGAL); //baca tmp tanggal dr eprom
  android.setPlayState(&playState);
  dmd.clearScreen(true);
  dmd.setBrightnes(150);
  tmpAct = -1;
  tmpPlan = -1;
  tmpTar = -1;
}

void loop()
{
  //ambil data waktu dari rtc
  // Reset Otomatis ketika tanggal berbeda
  //baca tanggal dr rtc
  if (tempTanggal != tanggal)
  {
    // RESET DI MULAI
    Target.reset();
    Actual.reset();
    Target.saveToEprom(tanggal, bulan, TARGET_ADDR);
    Actual.saveToEprom(tanggal, bulan, ACTUAL_ADDR);

    loadData();
    // tmp ->save;
    eprom.writeByte(0x50, alamat.TEMP_TANGGAL, tanggal);
    tempTanggal = tanggal;

    // Serial.println("Reset Data ");
  }
  //-----------------------------
  if (second.getEvent())
  {

    // Serial.print("timer = ");
    // Serial.println(OnePcs.getInterval());
    if (++detik > 59)
    {
      waktu.getTime(jam, menit, detik);
      waktu.getTanggal(hari, tanggal, bulan, tahun);
    }
  }

  if (setengahDetik.getEvent())
  {
    Istirahat1.setTime(jam, menit);
    Istirahat2.setTime(jam, menit);
    Istirahat3.setTime(jam, menit);
    jamKerja.setTime(jam, menit);
    // if (Target.getValue() % 2 == 0)
    // {
    // }
    // else
    // {
    //   dmd.drawFilledBox(0, 0, 31, 15, GRAPHICS_INVERSE);
    // }
    char time[5];
    //jam
    time[0] = (jam / 10) + '0';
    time[1] = (jam % 10) + '0';
    if ((millis() % 1000) > 500)
    {
      time[2] = ':';
    }
    else
    {
      time[2] = ' ';
    }

    time[3] = (menit / 10) + '0';
    time[4] = (menit % 10) + '0';

    // Kondisi sedang istirahat 1
    if (Istirahat1.getState())
    {
      OnePcs.stop();
      refres = 0;
      dmd.drawString(32, 0, "STOP", 5, GRAPHICS_NORMAL);
      dmd.selectFont(System5x7);
      dmd.drawFilledBox(64, 0, 95, 15, GRAPHICS_INVERSE);
      dmd.drawString(67, 0, "TIME", 4, GRAPHICS_NORMAL);
      dmd.drawString(64, 8, time, 5, GRAPHICS_NORMAL);
      dmd.selectFont(Font);
      // Serial.println("Sedang Istirahat 1");
    }
    // Kondisi sedang istirahat 2
    else if (Istirahat2.getState())
    {
      refres = 0;
      dmd.drawString(32, 0, "STOP", 5, GRAPHICS_NORMAL);
      dmd.selectFont(System5x7);
      dmd.drawFilledBox(64, 0, 95, 15, GRAPHICS_INVERSE);
      dmd.drawString(67, 0, "TIME", 4, GRAPHICS_NORMAL);
      dmd.drawString(64, 8, time, 5, GRAPHICS_NORMAL);
      dmd.selectFont(Font);
      OnePcs.stop();
      // Serial.println("Sedang Istirahat 2");
    }
    //  Kondisi sedang istirahat 3
    else if (Istirahat3.getState())
    {
      refres = 0;
      dmd.drawString(32, 0, "STOP", 5, GRAPHICS_NORMAL);
      dmd.selectFont(System5x7);
      dmd.drawFilledBox(64, 0, 95, 15, GRAPHICS_INVERSE);
      dmd.drawString(67, 0, "TIME", 4, GRAPHICS_NORMAL);
      dmd.drawString(64, 8, time, 5, GRAPHICS_NORMAL);
      dmd.selectFont(Font);
      OnePcs.stop();
      // Serial.println("Sedang Istirahat 3");
    }
    else
    {
      //  Kondisi sedang jam kerja
      if (jamKerja.getState())
      {
        if (playState == 1)
        {
          OnePcs.start();
          if (!refres)
          {
            char ary[4];
            intToArray(ary, Actual.getValue(), 0);
            dmd.drawFilledBox(0, 0, 31, 15, GRAPHICS_INVERSE);
            dmd.drawString(0, 0, ary, 4, GRAPHICS_NORMAL);

            intToArray(ary, Target.getValue(), 0);
            dmd.drawFilledBox(32, 0, 63, 15, GRAPHICS_INVERSE);
            dmd.drawString(32, 0, ary, 4, GRAPHICS_NORMAL);

            intToArray(ary, plan, 0);
            dmd.drawFilledBox(64, 0, 95, 15, GRAPHICS_INVERSE);
            dmd.drawString(64, 0, ary, 4, GRAPHICS_NORMAL);
            refres = 1;
          }

          // Serial.println("Jam Kerja");
        }
        else
        {

          refres = 0;
          OnePcs.stop();
          dmd.drawString(32, 0, "STOP", 5, GRAPHICS_NORMAL);
          dmd.selectFont(System5x7);
          dmd.drawFilledBox(64, 0, 95, 15, GRAPHICS_INVERSE);
          dmd.drawString(67, 0, "TIME", 4, GRAPHICS_NORMAL);
          dmd.drawString(64, 8, time, 5, GRAPHICS_NORMAL);
          dmd.selectFont(Font);
          // Serial.println("paused");
        }
        // menghitung naik paning counter
      }
      //  Kondisi di luar jam kerja
      else
      {
        refres = 0;
        OnePcs.stop();
        dmd.drawString(32, 0, "STOP", 5, GRAPHICS_NORMAL);
        dmd.selectFont(System5x7);
        dmd.drawFilledBox(64, 0, 95, 15, GRAPHICS_INVERSE);
        dmd.drawString(67, 0, "TIME", 4, GRAPHICS_NORMAL);
        dmd.drawString(64, 8, time, 5, GRAPHICS_NORMAL);
        dmd.selectFont(Font);
        // Serial.println("Pulang");
      }
    }

    // Serial.print("Plan : ");
    // Serial.println(plan);
    // Serial.print("Target : ");
    // Serial.println(Target.getValue());
    // Serial.print("Actual : ");
    // Serial.println(Actual.getValue());
    // Scaning.enableTimer();
  }

  char angka[4];

  if (tmpAct != Actual.getValue())
  {
    intToArray(angka, Actual.getValue(), 0);
    dmd.drawFilledBox(0, 0, 31, 15, GRAPHICS_INVERSE);
    dmd.drawString(0, 0, angka, 4, GRAPHICS_NORMAL);
    tmpAct = Actual.getValue();
  }
  if (tmpTar != Target.getValue())
  {
    intToArray(angka, Target.getValue(), 0);
    dmd.drawFilledBox(32, 0, 63, 15, GRAPHICS_INVERSE);
    dmd.drawString(32, 0, angka, 4, GRAPHICS_NORMAL);
    tmpTar = Target.getValue();
  }
  if (tmpPlan != plan)
  {
    intToArray(angka, plan, 0);
    dmd.drawFilledBox(64, 0, 95, 15, GRAPHICS_INVERSE);
    dmd.drawString(64, 0, angka, 4, GRAPHICS_NORMAL);
    tmpPlan = plan;
  }
  //TOMBOL DITEKAN
  if (Tombol1->falling())
  {
    // Actual.incriment();
    // Actual.saveToEprom(tanggal, bulan, ACTUAL_ADDR);
    if (playState == 1)
    {
      playState = 0;
    }
    else
    {
      playState = 1;
    }

    dmd.beepOn();
  }
  if (Tombol2->falling())
  {
    Actual.incriment();
    Actual.saveToEprom(tanggal, bulan, ACTUAL_ADDR);
    dmd.beepOn();
  }
  if (Tombol3->falling())
  {
    Actual.incriment();
    Actual.saveToEprom(tanggal, bulan, ACTUAL_ADDR);
    dmd.beepOn();
  }
  // PLAN COUNTING UP
  if (OnePcs.getEvent() && Target.getValue() < plan)
  {
    // Serial.println("tambah 1");
    Target.incriment();
    Target.saveToEprom(tanggal, bulan, TARGET_ADDR);
  }
  android.run();
  if (android.available())
  {
    delay(10);
    waktu.getTime(jam, menit, detik);
    waktu.getTanggal(hari, tanggal, bulan, tahun);
    loadData();
  }
  dmd.scanDisplayBySPI();
}