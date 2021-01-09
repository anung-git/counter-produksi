
#ifndef Rtc_H_
#define Rtc_H_

#include <Wire.h>
class Rtc{
    private:
        /* data */
        unsigned char makedec(unsigned char bcd);
        unsigned char makebcd(unsigned char dec);
    public:
        Rtc(/* args */);
        ~Rtc();
        void setTime(unsigned char jam, unsigned char menit, unsigned char detik);
        void getTime(unsigned char &jam, unsigned char &menit, unsigned char &detik);
        unsigned char getHari(unsigned char tanggal,unsigned char bulan, int tahun);
        void setTanggal(unsigned char tanggal,unsigned char bulan, int tahun);
        void getTanggal(unsigned char &hari,unsigned char &tanggal,unsigned char &bulan, int &tahun);
        
};

#endif
