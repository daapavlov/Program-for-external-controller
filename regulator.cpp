#include "regulator.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

Regulator :: Regulator()
{

}
double Regulator :: SignalOut_DGt(double U)
{
    Qt=273.3f*U-93.3f;
    Gtdv=0.856f*Qt/3600.0f;
    return  Gtdv;
}
float Regulator :: SelectedSignal()
{
    US = RegulatorRotorSpeed_2();
    UDN1 = RegulatorRotorSpeed_1();
    UDN2Pr = AccelerationRotorSpeed_2Pr();
    UDN2Sb = AccelerationRotorSpeed_2Sb();
    UTG3 = RegulatorTemperature_3();
    UP2 = RegulatorPressure_2();
//    UTG4 = RegulatorTemperature_4();

    if(UDN2Pr < US) US=UDN2Pr;
    if(UDN2Sb > US) US=UDN2Sb;
//    if(UP2 < US) US=UP2;
//    if((UTG3 < US)&&(RotorSpeed_2/(n2max/100.0f) > 70.0f)) US=UTG3;
//    if((UTG4 < US)&&(RotorSpeed_2/(n2max/100.0f) > 70.0f)) US=UTG4;
    if(US < -UC1) US= -UC1+CSB*(US+UC1);
    if(US >  UC1) US=  UC1+CPR*(US-UC1);
    alexp=20.0f/34.0f;
    blexp=25.0f-alexp*100.0f;
    CIX =( alexp*RotorSpeed_2/(n2max/100.0f)+blexp)*1.2f;
    if(CIX < 5.0f) CIX=5.0f;
    if(CIX > 25.0f) CIX=25.0f;
    DUIX=CIX*US;
    US=MemoryParametrs[8]+DUIX*Tqr;

    if(US < Umin)   US=Umin;
    if(US > Umax)   US=Umax;

    MemoryParametrs[8]=US;
    return US;
}
float Regulator :: RegulatorRotorSpeed_2()
{
    Tn2 = -0.008f * RotorSpeed_2/(n2max/100.0f) + 1.2f;
    if(Tn2 < 0.4f) Tn2=0.4f;
    if(Tn2 > 0.8f) Tn2=0.8f;
    n2z = RPIDLIN(RUD_n2,JRUN,RUD) * (n2max/100.0f);
    n2zad = n2z;
    Errorn2 = (n2z - RotorSpeed_2)/(n2max/100.0f);
    An2 = ErrorOgr(Errorn2, -16.0f, 16.0f);
    Errorn2 = An2 * kn2;
    beta = Tn2/(Tn2 + 5.0f * Tqr);
    DUn2 = beta * MemoryParametrs[2] - kn2 * Tn2/Tqr * (1.0f - beta) * (RotorSpeed_2 - Memoryn2)/(n2max/100.0f);
    Memoryn2 = RotorSpeed_2;
    MemoryParametrs[2] = DUn2;
    Un2 = Errorn2 + DUn2;
    return Un2;
}
float Regulator :: RegulatorPressure_2()
{
    DUP2 = (p2max - Pressure_2) * 0.69f * kp2;
    return DUP2;
}
float Regulator :: RegulatorTemperature_3()
{
    ErrorT3 = (t3max - Temperature_3)/(t3max/100.0f);
    AT3 = ErrorOgr(ErrorT3, -8.0f, 8.0f);
    ErrorT3 = AT3 * kt3;
    DUT3 = -0.0005f/Tqr*(Temperature_3 - MemoryParametrs[3])/(t3max/100.0f);
    MemoryParametrs[3] = Temperature_3;
    UT3 = ErrorOgr(ErrorT3 + DUT3, -8.0f, 8.0f);
    return UT3;
}
float Regulator :: RegulatorRotorSpeed_1()
{
    bn1 = expf(-Tqr/TNV1);
    a2n = -bn1 * TNV1/TNV2;
    a1n = (1 - bn1) - a2n;

    DUn1 = RotorSpeed_1 * a1n + Memoryn1 * a2n + bn1 * MemoryParametrs[1];
    Memoryn1 = DUn1;
    Errorn1 = (n1max - DUn1) / (n1max/100.0f);
    An1 = ErrorOgr(Errorn1, -8.0f, 8.0f);
    Un1 = An1 * kn1;

    return Un1;
}

float Regulator :: AccelerationRotorSpeed_2Pr()
{
    /*ПРИЕМИCТОСТЬ*/
    adn2 = expf(-Tqr/TFUDN2);
    DUdn2 = adn2 * MemoryParametrs[5] + (1.0f - adn2) * RotorSpeed_2;
    MemoryParametrs[5] = DUdn2;
    fvsp = DUdn2 + (RotorSpeed_2 - DUdn2) * TUDN2/TFUDN2;
    dn2r = (fvsp - RotorSpeed_2);
    N21OTr = RotorSpeed_2/n2max;
    dn2z = RPIDLIN(DN2ZD,JDN2,N21OTr);
    Errordn2 = (dn2z - dn2r);
    APC = kdn2 * Errordn2;
    adn2 = expf(-Tqr/TFNDN2);
    DUdn2 = adn2 * MemoryParametrs[6] + (1.0f - adn2) * APC;
    MemoryParametrs[6] = DUdn2;
    Udn2 = DUdn2 + (APC - DUdn2) * TINDN2/TFNDN2;

    return Udn2;
}
float Regulator :: AccelerationRotorSpeed_2Sb()
{
    /*СБРОС*/
    N21OTr = RotorSpeed_2/n2max;
    dn2zs = RPIDLIN(DN2SB,JDN2S,N21OTr);
    Errordn2 = dn2zs - dn2r;
    APC = kdn2 * Errordn2;
    adn2 = expf(-Tqr/TFNDN2);
    DUdn2 = adn2 * MemoryParametrs[7] + (1.0f - adn2) * APC;
    MemoryParametrs[7] = DUdn2;
    Udn2s = DUdn2 + (APC - DUdn2) * TINDN2/TFNDN2;

    return Udn2s;
}
float  Regulator :: ErrorOgr (float X,float xmi,float xma)
{
    float ret;
    ret=X;
    if(X < xmi) ret=xmi;
    if(X > xma) ret=xma;
    return ret;
}
float Regulator :: RPIDLIN(float *G,int K,float X)
{

//c   Входные параметры:
//c   G - массив (длины 2*К ) значений аргумента и функции
//c       в узловых точках,
//c       X(1),X(2),...,X(K),F(1),F(2),...,F(K);
//c   K - число узлов интерполяции;
//c   X - значение аргумента;
//c   Выходной параметр:
//c   F - значение функции.

    int n=0;
    float F;
    float ak,bk;
    if(X < G[0]) X=G[0];
    if(X > G[K-1]) X=G[K-1];

    n=0;
    for(int j=0;j<K-1;j++)
    {
        if((X >= G[j]) && (X <= G[j+1]))
        {

            break;

        }n++;
    }
    ak=(G[K+n+1]-G[K+n])/(G[n+1]-G[n]);//
    bk=G[K+n]-G[n]*ak;
    F=ak*X+bk;
    return F;
}
