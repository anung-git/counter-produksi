
#ifndef Kerja_H_
#define Kerja_H_

class Kerja
{
private:
    /* data */
    unsigned int start, stop, time;

public:
    Kerja(/* args */);
    ~Kerja();
    void setStart(unsigned char jam, unsigned char menit);
    void setStop(unsigned char jam, unsigned char menit);
    void setTime(unsigned char jam, unsigned char menit);
    bool getState(void);
};


#endif
