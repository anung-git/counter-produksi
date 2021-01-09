#include <WiFiUdp.h>
#include "Android.h"
//
//          *cmd=val#
//      001 = set time
//      002 = set adzan
//      003 = set iqomah
//      004 = set sholat
void Android::run()
{
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        // receive incoming UDP packets
        // Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
        int len = Udp.read(incomingPacket, 255);
        if (len > 0)
        {
            incomingPacket[len] = 0;
        }
        // Serial.printf("UDP packet contents: %s\n", incomingPacket);
        // send back a reply, to the IP address and port we got the packet from
        // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        // Udp.write("Daxtronic_JWS");
        // Udp.endPacket();

        // for (int i = 0; i < len; i++)
        // {
        //     Serial.print(incomingPacket[i]);
        // }

        // Serial.println();

        if (incomingPacket[0] == '*' && incomingPacket[len - 1] == '#')
        {

            // Serial.println("cmd finish");
            cmd = 100 * (incomingPacket[1] - '0');
            cmd += 10 * (incomingPacket[2] - '0');
            cmd += incomingPacket[3] - '0';
            // if (cmd == 0){
            //     this->setTime();
            // }
            // if (cmd == 1){
            //     this->setIqomah();
            // }
            this->mydmd->beepOn();
            switch (cmd)
            {
            case 1:
                this->setTime();
                break;
            case 2:
                this->setSenin();
                break;
            case 3:
                this->setSelasa();
                break;
            case 4:
                this->setRabu();
                break;
            case 5:
                this->setKamis();
                break;
            case 6:
                this->setJumat();
                break;
            case 7:
                this->setSabtu();
                break;
            case 8:
                this->setMinggu();
                break;

            case 9:
                this->initActual();
                break;
            case 10:
                this->initTarget();
                break;
            case 11:
                this->resetActual();
                break;
            case 12:
                this->resetTarget();
                break;
            case 13:
                this->setTimer();
                break;
            case 14:
                this->setPlay();
                break;

            default:
                break;
            }
            this->clear(len);
            redy = true;
            delay(10);
        }
    }
}

void Android::setPlay()
{
    *playstate = incomingPacket[5] - '0';
}
void Android::setPlayState(uint8_t *state)
{
    playstate = state;
}
void Android::setTimer()
{
    unsigned int memData;
    memData = 1000 * (incomingPacket[5] - '0');
    memData += 100 * (incomingPacket[6] - '0');
    memData += 10 * (incomingPacket[7] - '0');
    memData += incomingPacket[8] - '0';

    eprom.writeInt(0x50, alamat.CYCLE_TIME_S, memData);
    delay(10);
    memData = 100 * (incomingPacket[9] - '0');
    memData += 10 * (incomingPacket[10] - '0');
    memData += incomingPacket[11] - '0';
    eprom.writeInt(0x50, alamat.CYCLE_TIME_MS, memData);

}
void Android::initActual()
{
    uint16_t memData;
    uint8_t hr, tgl, bln;
    int thn;

    memData = 1000 * (incomingPacket[5] - '0');
    memData += 100 * (incomingPacket[6] - '0');
    memData += 10 * (incomingPacket[7] - '0');
    memData += incomingPacket[8] - '0';

    waktu.getTanggal(hr, tgl, bln, thn);
    this->Actual->setValue(memData);
    this->Actual->saveToEprom(tgl, bln, MY_ACTUAL_ADDR);
    
    // delay(10);
}
void Android::initTarget()
{
    uint16_t memData;
    uint8_t hr, tgl, bln;
    int thn;

    memData = 1000 * (incomingPacket[5] - '0');
    memData += 100 * (incomingPacket[6] - '0');
    memData += 10 * (incomingPacket[7] - '0');
    memData += incomingPacket[8] - '0';
    waktu.getTanggal(hr, tgl, bln, thn);

    // Serial.print("init target = ");
    // Serial.println(memData);
    this->Target->setValue(memData);
    this->Target->saveToEprom(tgl, bln, MY_TARGET_ADDR);
    // delay(10);
}

void Android::setActual(Counter *aktual)
{
    this->Actual = aktual;
}
void Android::setTarget(Counter *target)
{
    this->Target = target;
}

void Android::resetTarget()
{
    uint8_t hr, tgl, bln;
    int thn;
    waktu.getTanggal(hr, tgl, bln, thn);
    this->Target->reset();
    this->Target->saveToEprom(tgl, bln, MY_TARGET_ADDR);
    
    // delay(10);
}

void Android::resetActual()
{
    uint8_t hr, tgl, bln;
    int thn;
    waktu.getTanggal(hr, tgl, bln, thn);
    this->Actual->reset();
    this->Actual->saveToEprom(tgl, bln, MY_ACTUAL_ADDR);
    
    // delay(10);
}

void Android::clear(uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        incomingPacket[i] = 0;
    }
}
void Android::setSenin()
{
    uint16_t memData;

    memData = 10 * (incomingPacket[5] - '0');
    memData += incomingPacket[6] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[7] - '0');
    memData += incomingPacket[8] - '0';
    eprom.writeInt(0x50, senin.mulai(), memData);

    memData = 10 * (incomingPacket[9] - '0');
    memData += incomingPacket[10] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[11] - '0');
    memData += incomingPacket[12] - '0';
    eprom.writeInt(0x50, senin.istirahat1(), memData);

    memData = 10 * (incomingPacket[13] - '0');
    memData += incomingPacket[14] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[15] - '0');
    memData += incomingPacket[16] - '0';
    eprom.writeInt(0x50, senin.masuk1(), memData);

    memData = 10 * (incomingPacket[17] - '0');
    memData += incomingPacket[18] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[19] - '0');
    memData += incomingPacket[20] - '0';
    eprom.writeInt(0x50, senin.istirahat2(), memData);

    memData = 10 * (incomingPacket[21] - '0');
    memData += incomingPacket[22] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[23] - '0');
    memData += incomingPacket[24] - '0';
    eprom.writeInt(0x50, senin.masuk2(), memData);

    memData = 10 * (incomingPacket[25] - '0');
    memData += incomingPacket[26] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[27] - '0');
    memData += incomingPacket[28] - '0';
    eprom.writeInt(0x50, senin.istirahat3(), memData);

    memData = 10 * (incomingPacket[29] - '0');
    memData += incomingPacket[30] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[31] - '0');
    memData += incomingPacket[32] - '0';
    eprom.writeInt(0x50, senin.masuk3(), memData);

    memData = 10 * (incomingPacket[33] - '0');
    memData += incomingPacket[34] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[35] - '0');
    memData += incomingPacket[36] - '0';
    eprom.writeInt(0x50, senin.selesai(), memData);

    memData = 1000 * (incomingPacket[37] - '0');
    memData += 100 * (incomingPacket[38] - '0');
    memData += 10 * (incomingPacket[39] - '0');
    memData += incomingPacket[40] - '0';
    eprom.writeInt(0x50, senin.plan(), memData);
}
void Android::setSelasa()
{
    uint16_t memData;

    memData = 10 * (incomingPacket[5] - '0');
    memData += incomingPacket[6] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[7] - '0');
    memData += incomingPacket[8] - '0';
    eprom.writeInt(0x50, selasa.mulai(), memData);

    memData = 10 * (incomingPacket[9] - '0');
    memData += incomingPacket[10] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[11] - '0');
    memData += incomingPacket[12] - '0';
    eprom.writeInt(0x50, selasa.istirahat1(), memData);

    memData = 10 * (incomingPacket[13] - '0');
    memData += incomingPacket[14] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[15] - '0');
    memData += incomingPacket[16] - '0';
    eprom.writeInt(0x50, selasa.masuk1(), memData);

    memData = 10 * (incomingPacket[17] - '0');
    memData += incomingPacket[18] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[19] - '0');
    memData += incomingPacket[20] - '0';
    eprom.writeInt(0x50, selasa.istirahat2(), memData);

    memData = 10 * (incomingPacket[21] - '0');
    memData += incomingPacket[22] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[23] - '0');
    memData += incomingPacket[24] - '0';
    eprom.writeInt(0x50, selasa.masuk2(), memData);

    memData = 10 * (incomingPacket[25] - '0');
    memData += incomingPacket[26] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[27] - '0');
    memData += incomingPacket[28] - '0';
    eprom.writeInt(0x50, selasa.istirahat3(), memData);

    memData = 10 * (incomingPacket[29] - '0');
    memData += incomingPacket[30] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[31] - '0');
    memData += incomingPacket[32] - '0';
    eprom.writeInt(0x50, selasa.masuk3(), memData);

    memData = 10 * (incomingPacket[33] - '0');
    memData += incomingPacket[34] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[35] - '0');
    memData += incomingPacket[36] - '0';
    eprom.writeInt(0x50, selasa.selesai(), memData);

    memData = 1000 * (incomingPacket[37] - '0');
    memData += 100 * (incomingPacket[38] - '0');
    memData += 10 * (incomingPacket[39] - '0');
    memData += incomingPacket[40] - '0';
    eprom.writeInt(0x50, selasa.plan(), memData);
}
void Android::setRabu()
{
    uint16_t memData;

    memData = 10 * (incomingPacket[5] - '0');
    memData += incomingPacket[6] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[7] - '0');
    memData += incomingPacket[8] - '0';
    eprom.writeInt(0x50, rabu.mulai(), memData);

    memData = 10 * (incomingPacket[9] - '0');
    memData += incomingPacket[10] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[11] - '0');
    memData += incomingPacket[12] - '0';
    eprom.writeInt(0x50, rabu.istirahat1(), memData);

    memData = 10 * (incomingPacket[13] - '0');
    memData += incomingPacket[14] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[15] - '0');
    memData += incomingPacket[16] - '0';
    eprom.writeInt(0x50, rabu.masuk1(), memData);

    memData = 10 * (incomingPacket[17] - '0');
    memData += incomingPacket[18] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[19] - '0');
    memData += incomingPacket[20] - '0';
    eprom.writeInt(0x50, rabu.istirahat2(), memData);

    memData = 10 * (incomingPacket[21] - '0');
    memData += incomingPacket[22] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[23] - '0');
    memData += incomingPacket[24] - '0';
    eprom.writeInt(0x50, rabu.masuk2(), memData);

    memData = 10 * (incomingPacket[25] - '0');
    memData += incomingPacket[26] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[27] - '0');
    memData += incomingPacket[28] - '0';
    eprom.writeInt(0x50, rabu.istirahat3(), memData);

    memData = 10 * (incomingPacket[29] - '0');
    memData += incomingPacket[30] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[31] - '0');
    memData += incomingPacket[32] - '0';
    eprom.writeInt(0x50, rabu.masuk3(), memData);

    memData = 10 * (incomingPacket[33] - '0');
    memData += incomingPacket[34] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[35] - '0');
    memData += incomingPacket[36] - '0';
    eprom.writeInt(0x50, rabu.selesai(), memData);

    memData = 1000 * (incomingPacket[37] - '0');
    memData += 100 * (incomingPacket[38] - '0');
    memData += 10 * (incomingPacket[39] - '0');
    memData += incomingPacket[40] - '0';
    eprom.writeInt(0x50, rabu.plan(), memData);
}
void Android::setKamis()
{
    uint16_t memData;
    memData = 10 * (incomingPacket[5] - '0');
    memData += incomingPacket[6] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[7] - '0');
    memData += incomingPacket[8] - '0';
    eprom.writeInt(0x50, kamis.mulai(), memData);

    memData = 10 * (incomingPacket[9] - '0');
    memData += incomingPacket[10] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[11] - '0');
    memData += incomingPacket[12] - '0';
    eprom.writeInt(0x50, kamis.istirahat1(), memData);

    memData = 10 * (incomingPacket[13] - '0');
    memData += incomingPacket[14] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[15] - '0');
    memData += incomingPacket[16] - '0';
    eprom.writeInt(0x50, kamis.masuk1(), memData);

    memData = 10 * (incomingPacket[17] - '0');
    memData += incomingPacket[18] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[19] - '0');
    memData += incomingPacket[20] - '0';
    eprom.writeInt(0x50, kamis.istirahat2(), memData);

    memData = 10 * (incomingPacket[21] - '0');
    memData += incomingPacket[22] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[23] - '0');
    memData += incomingPacket[24] - '0';
    eprom.writeInt(0x50, kamis.masuk2(), memData);

    memData = 10 * (incomingPacket[25] - '0');
    memData += incomingPacket[26] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[27] - '0');
    memData += incomingPacket[28] - '0';
    eprom.writeInt(0x50, kamis.istirahat3(), memData);

    memData = 10 * (incomingPacket[29] - '0');
    memData += incomingPacket[30] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[31] - '0');
    memData += incomingPacket[32] - '0';
    eprom.writeInt(0x50, kamis.masuk3(), memData);

    memData = 10 * (incomingPacket[33] - '0');
    memData += incomingPacket[34] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[35] - '0');
    memData += incomingPacket[36] - '0';
    eprom.writeInt(0x50, kamis.selesai(), memData);

    memData = 1000 * (incomingPacket[37] - '0');
    memData += 100 * (incomingPacket[38] - '0');
    memData += 10 * (incomingPacket[39] - '0');
    memData += incomingPacket[40] - '0';
    eprom.writeInt(0x50, kamis.plan(), memData);
}
void Android::setJumat()
{
    uint16_t memData;
    memData = 10 * (incomingPacket[5] - '0');
    memData += incomingPacket[6] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[7] - '0');
    memData += incomingPacket[8] - '0';
    eprom.writeInt(0x50, jumat.mulai(), memData);

    memData = 10 * (incomingPacket[9] - '0');
    memData += incomingPacket[10] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[11] - '0');
    memData += incomingPacket[12] - '0';
    eprom.writeInt(0x50, jumat.istirahat1(), memData);

    memData = 10 * (incomingPacket[13] - '0');
    memData += incomingPacket[14] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[15] - '0');
    memData += incomingPacket[16] - '0';
    eprom.writeInt(0x50, jumat.masuk1(), memData);

    memData = 10 * (incomingPacket[17] - '0');
    memData += incomingPacket[18] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[19] - '0');
    memData += incomingPacket[20] - '0';
    eprom.writeInt(0x50, jumat.istirahat2(), memData);

    memData = 10 * (incomingPacket[21] - '0');
    memData += incomingPacket[22] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[23] - '0');
    memData += incomingPacket[24] - '0';
    eprom.writeInt(0x50, jumat.masuk2(), memData);

    memData = 10 * (incomingPacket[25] - '0');
    memData += incomingPacket[26] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[27] - '0');
    memData += incomingPacket[28] - '0';
    eprom.writeInt(0x50, jumat.istirahat3(), memData);

    memData = 10 * (incomingPacket[29] - '0');
    memData += incomingPacket[30] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[31] - '0');
    memData += incomingPacket[32] - '0';
    eprom.writeInt(0x50, jumat.masuk3(), memData);

    memData = 10 * (incomingPacket[33] - '0');
    memData += incomingPacket[34] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[35] - '0');
    memData += incomingPacket[36] - '0';
    eprom.writeInt(0x50, jumat.selesai(), memData);

    memData = 1000 * (incomingPacket[37] - '0');
    memData += 100 * (incomingPacket[38] - '0');
    memData += 10 * (incomingPacket[39] - '0');
    memData += incomingPacket[40] - '0';
    eprom.writeInt(0x50, jumat.plan(), memData);
}
void Android::setSabtu()
{
    uint16_t memData;
    memData = 10 * (incomingPacket[5] - '0');
    memData += incomingPacket[6] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[7] - '0');
    memData += incomingPacket[8] - '0';
    eprom.writeInt(0x50, sabtu.mulai(), memData);

    memData = 10 * (incomingPacket[9] - '0');
    memData += incomingPacket[10] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[11] - '0');
    memData += incomingPacket[12] - '0';
    eprom.writeInt(0x50, sabtu.istirahat1(), memData);

    memData = 10 * (incomingPacket[13] - '0');
    memData += incomingPacket[14] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[15] - '0');
    memData += incomingPacket[16] - '0';
    eprom.writeInt(0x50, sabtu.masuk1(), memData);

    memData = 10 * (incomingPacket[17] - '0');
    memData += incomingPacket[18] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[19] - '0');
    memData += incomingPacket[20] - '0';
    eprom.writeInt(0x50, sabtu.istirahat2(), memData);

    memData = 10 * (incomingPacket[21] - '0');
    memData += incomingPacket[22] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[23] - '0');
    memData += incomingPacket[24] - '0';
    eprom.writeInt(0x50, sabtu.masuk2(), memData);

    memData = 10 * (incomingPacket[25] - '0');
    memData += incomingPacket[26] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[27] - '0');
    memData += incomingPacket[28] - '0';
    eprom.writeInt(0x50, sabtu.istirahat3(), memData);

    memData = 10 * (incomingPacket[29] - '0');
    memData += incomingPacket[30] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[31] - '0');
    memData += incomingPacket[32] - '0';
    eprom.writeInt(0x50, sabtu.masuk3(), memData);

    memData = 10 * (incomingPacket[33] - '0');
    memData += incomingPacket[34] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[35] - '0');
    memData += incomingPacket[36] - '0';
    eprom.writeInt(0x50, sabtu.selesai(), memData);

    memData = 1000 * (incomingPacket[37] - '0');
    memData += 100 * (incomingPacket[38] - '0');
    memData += 10 * (incomingPacket[39] - '0');
    memData += incomingPacket[40] - '0';
    eprom.writeInt(0x50, sabtu.plan(), memData);
}
void Android::setMinggu()
{
    uint16_t memData;
    memData = 10 * (incomingPacket[5] - '0');
    memData += incomingPacket[6] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[7] - '0');
    memData += incomingPacket[8] - '0';
    eprom.writeInt(0x50, minggu.mulai(), memData);

    memData = 10 * (incomingPacket[9] - '0');
    memData += incomingPacket[10] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[11] - '0');
    memData += incomingPacket[12] - '0';
    eprom.writeInt(0x50, minggu.istirahat1(), memData);

    memData = 10 * (incomingPacket[13] - '0');
    memData += incomingPacket[14] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[15] - '0');
    memData += incomingPacket[16] - '0';
    eprom.writeInt(0x50, minggu.masuk1(), memData);

    memData = 10 * (incomingPacket[17] - '0');
    memData += incomingPacket[18] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[19] - '0');
    memData += incomingPacket[20] - '0';
    eprom.writeInt(0x50, minggu.istirahat2(), memData);

    memData = 10 * (incomingPacket[21] - '0');
    memData += incomingPacket[22] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[23] - '0');
    memData += incomingPacket[24] - '0';
    eprom.writeInt(0x50, minggu.masuk2(), memData);

    memData = 10 * (incomingPacket[25] - '0');
    memData += incomingPacket[26] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[27] - '0');
    memData += incomingPacket[28] - '0';
    eprom.writeInt(0x50, minggu.istirahat3(), memData);

    memData = 10 * (incomingPacket[29] - '0');
    memData += incomingPacket[30] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[31] - '0');
    memData += incomingPacket[32] - '0';
    eprom.writeInt(0x50, minggu.masuk3(), memData);

    memData = 10 * (incomingPacket[33] - '0');
    memData += incomingPacket[34] - '0';
    memData *= 60;
    memData += 10 * (incomingPacket[35] - '0');
    memData += incomingPacket[36] - '0';
    eprom.writeInt(0x50, minggu.selesai(), memData);

    memData = 1000 * (incomingPacket[37] - '0');
    memData += 100 * (incomingPacket[38] - '0');
    memData += 10 * (incomingPacket[39] - '0');
    memData += incomingPacket[40] - '0';
    eprom.writeInt(0x50, minggu.plan(), memData);
}
void Android::setDMD(DMD *panel)
{
    mydmd = panel;
}

void Android::setTime()
{
    uint8_t jam, menit, detik;
    uint8_t tgl, bln;
    uint16_t thn;
    jam = 10 * (incomingPacket[5] - '0');
    jam += incomingPacket[6] - '0';

    menit = 10 * (incomingPacket[7] - '0');
    menit += incomingPacket[8] - '0';

    detik = 10 * (incomingPacket[9] - '0');
    detik += incomingPacket[10] - '0';

    tgl = 10 * (incomingPacket[11] - '0');
    tgl += incomingPacket[12] - '0';

    bln = 10 * (incomingPacket[13] - '0');
    bln += incomingPacket[14] - '0';

    thn = 1000 * (incomingPacket[15] - '0');
    thn += 100 * (incomingPacket[16] - '0');
    thn += 10 * (incomingPacket[17] - '0');
    thn += incomingPacket[18] - '0';

    waktu.setTime(jam, menit, detik);
    waktu.setTanggal(tgl, bln, thn);
    // this->mydmd->beepOn();
}
String Android::getMsg()
{
    redy = false;
    return String(incomingPacket);
}
bool Android::available()
{
    // return redy;
    if (redy)
    {
        redy = false;
        return true;
        /* code */
    }
    else
    {
        return false;
        /* code */
    }
}
Android::Android(/* args */)
{

    Udp.begin(localUdpPort);
}
Android::~Android()
{
}

// cmd=val1,val2,val3