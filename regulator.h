#ifndef REGULATOR_H
#define REGULATOR_H



class Regulator
{

public:
    Regulator();
    ~Regulator();

    double SignalOut_DGt(double U);
    float SelectedSignal();

    void setTNV1(float X) {this -> TNV1 = X;}
    void setTNV2(float X) {this -> TNV2 = X;}
    float getTNV1(){return TNV1;}
    float getTNV2(){return TNV2;}
    void setTINDN2(float X){this -> TINDN2 = X;}
    void setTFNDN2(float X){this -> TFNDN2 = X;}
    void setTFUDN2(float X){this -> TFUDN2 = X;}
    void setTUDN2(float X){this -> TUDN2 = X;}
    float getTINDN2(){return TINDN2;}
    float getTFNDN2(){return TFNDN2;}
    float getTFUDN2(){return TFUDN2;}
    float getTUDN2(){return TUDN2;}

    float setProgrammRegulation(float ArraySize16[]){for(int i=0; i<16; i++)
                                                    {RUD_n2[i] = ArraySize16[i];}
                                                    return 0;}
    float getProgrammRegulation(float ArraySize16[]){for(int i=0; i<16; i++)
                                                    {ArraySize16[i] = RUD_n2[i];}
                                                    return 0;}
    float setProgrammRegulationPr(float ArraySize14[]){for(int i=0; i<16; i++)
                                                    {DN2ZD[i] = ArraySize14[i];}
                                                    return 0;}
    float getProgrammRegulationPr(float ArraySize14[]){for(int i=0; i<16; i++)
                                                    {ArraySize14[i] = DN2ZD[i];}
                                                    return 0;}
    float setProgrammRegulationSb(float ArraySize14[]){for(int i=0; i<16; i++)
                                                    {DN2SB[i] = ArraySize14[i];}
                                                    return 0;}
    float getProgrammRegulationSb(float ArraySize14[]){for(int i=0; i<16; i++)
                                                    {ArraySize14[i] = DN2SB[i];}
                                                    return 0;}
    void setUogr(float MAX, float MIN) {this -> Umax = MAX; this -> Umin = MIN;}
    float getUmin(){return Umin;}
    float getUmax(){return Umax;}
    void setTqr(float X){this-> Tqr = X;}
    float getTqr(){return Tqr;}
    void setMemoryParametrs(double Memoryn1, double Memoryn2){
        this->Memoryn1=Memoryn1; this->Memoryn2=Memoryn2;}

    float RUD;
    float Temperature_0;
    float Pressure_0;
    float RotorSpeed_1;
    float Pressure_2;
    float RotorSpeed_2;
    float Temperature_3;
    float Temperature_4;
    float GasDynamicStability;
    float Thrust;

    float kn2, kn1, kt3, kt4, kp2, kdn2;//коэффициенты
    float n1max, n2max = 16500.0f, p2max, t3max, t4max;//максимальные значения
    float MemoryParametrs[12];
    float n2zad;

private:
    float RegulatorRotorSpeed_2();//
    float RegulatorRotorSpeed_1();//
    float AccelerationRotorSpeed_2Pr();//
    float AccelerationRotorSpeed_2Sb();//
    float RegulatorTemperature_4();
    float RegulatorTemperature_3();//
    float RegulatorPressure_2();//
    float RegulatorGasDynamicStability();
    float RegulatorThrust();


    float  ErrorOgr (float X,float xmi,float xma);
    float  RPIDLIN(float *G,int K,float X);


    float Tqr = 0.02;

    /*SignalOut*/
    double  Gtdv, Qt;

    /*n1*/
    float TNV1 = 0.2f, TNV2 = 0.15f;
    float bn1, a2n, a1n;
    float DUn1, Memoryn1, Errorn1, An1, Un1;

    /*dn2*/
    float TINDN2 = 1.2f, TFNDN2 = 0.3f, TFUDN2 = 0.3f, TUDN2 = 0.9f;

    float adn2, DUdn2, fvsp, dn2r, Errordn2, APC, dn2z, Udn2;
    float dn2zs, Udn2s;
    float DN2ZD[14] = {0.25, 0.5, 0.55, 0.73, 0.8, 1.0, 1.2,
                     250, 1300, 1500, 1800, 2000, 2000, 1100};

    float DN2SB[14] = {0.5f, 0.55f, 0.7f, 0.8f, 0.9f, 1.0f, 1.2f,
                 -900.0f, -900.0f, -900.0f, -1800.0f, -2800.0f, -2200.0f, -2200.0f};

    int JDN2S = 7, JDN2 = 8;
    float N21OTr;

    /*n2*/
    float Tn2, Errorn2, An2, DUn2, Memoryn2, Un2, n2z, beta;
    float RUD_n2[16]={19., 20., 40., 70., 80., 90., 100., 101.,
                    59.5, 60, 65, 75, 90 ,100 ,100, 100}; //set

    int JRUN = 8;

    /*T3*/
    float ErrorT3, AT3, DUT3, UT3;

    /*P2*/
    float DUP2, UP2;

    /*Секлетор и интеграл*/
    float US, UDN2Pr, UDN2Sb, UDN1, UTG4, UTG3, UDP2;
    float UC1 = 0.02f, CPR = 4.0f, CSB = 2.0f;
    float alexp, blexp, CIX, DUIX;
    float Umin = 1.25f, Umax = 4.4f; //set



};



#endif // REGULATOR_H
