
#ifndef Break_H_
#define Break_H_

class Break
{
private:
    /* data */
    unsigned int start, stop, time;

public:
    Break(/* args */);
    ~Break();
    void setStart(unsigned int time);
    void setStop(unsigned int time);
    void setTime(unsigned char jam, unsigned char menit);
    bool getState(void);
};


#endif
