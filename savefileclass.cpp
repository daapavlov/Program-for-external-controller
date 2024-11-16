#include "savefileclass.h"
#include "ui_savefileclass.h"
#include "QtGui"
#include "fstream"

SaveFileClass::SaveFileClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveFileClass)
{
    ui->setupUi(this);
}

SaveFileClass::~SaveFileClass()
{
    delete ui;
}
QString SaveFileClass::OpenAndWriteFileByDate(std::vector<float>array1, std::vector<float>array2, std::vector<float>array2zad, std::vector<float>array3, std::vector<float>array3zad, std::vector<float>array4, std::vector<float>array4zad,
                                              std::vector<float>array5, std::vector<float>array5zad, std::vector<float>array6, std::vector<float>array6zad, std::vector<float>array7, std::vector<float>array7zad, std::vector<float>array8,
                                              std::vector<float>array9, std::vector<float>array10,std::vector<float>array11,std::vector<float>array12)
{
    PathToFile.clear();
    PathToFile = "E:\\soft_QT\\ModelAI25\\Graphic\\File ["+QDateTime::currentDateTime().toString("dd.MM.yyyy. (hh-mm-ss)")+"].txt";
    ArrayStringPathToFile = PathToFile.toUtf8().data();
    std::fstream fout;
    fout.open(ArrayStringPathToFile, std::ios::out);
    if(fout.is_open())
    {
        for(uint i=0; i<array1.size(); i++)
        {
            fout<<""<<array1[i]<<"\t"<<array2[i]<<"\t"<<array2zad[i]<<"\t"<<array3[i]<<"\t"<<array3zad[i]<<"\t"<<array4[i]<<"\t"<<array4zad[i]<<"\t"<<array5[i]<<"\t"<<array5zad[i]<<"\t"<<array6[i]<<"\t"<<array6zad[i]<<
                  "\t"<<array7[i]<<"\t"<<array7zad[i]<<"\t"<<array8[i]<<"\t"<<array9[i]<<"\t"<<array10[i]<<"\t"<<array11[i]<<"\t"<<array12[i]<<"\n";
        }
        fout.close();
        this->close();
        return "Данные успешно записаны в файл";
    }
    else
    {
        QMessageBox::about(this,"Ошибка", "Не удалось открыть файл для записи");
        this->close();
        return "Данные не записаны, файл не открылся";
    }
}

QString SaveFileClass::OpenAndWriteFile(std::vector<float>array1, std::vector<float>array2, std::vector<float>array2zad, std::vector<float>array3, std::vector<float>array3zad, std::vector<float>array4, std::vector<float>array4zad,
                                        std::vector<float>array5, std::vector<float>array5zad, std::vector<float>array6, std::vector<float>array6zad, std::vector<float>array7, std::vector<float>array7zad, std::vector<float>array8,
                                        std::vector<float>array9, std::vector<float>array10,std::vector<float>array11,std::vector<float>array12)

{
    PathToFile.clear();
    PathToFile = QFileDialog::getSaveFileName(this, "Сохранить","E:\\soft_QT\\ModelAI25\\Graphic");
    if(!(PathToFile != ""))
    {
        return "Данные не записаны, файл не открылся";
    }
    PathToFile.append(".txt");
    ArrayStringPathToFile = PathToFile.toUtf8().data();
    std::fstream fout;
    fout.open(ArrayStringPathToFile, std::ios::out);
    if(fout.is_open())
    {
        for(uint i=0; i<array1.size(); i++)
        {
            fout<<""<<array1[i]<<"\t"<<array2[i]<<"\t"<<array2zad[i]<<"\t"<<array3[i]<<"\t"<<array3zad[i]<<"\t"<<array4[i]<<"\t"<<array4zad[i]<<"\t"<<array5[i]<<"\t"<<array5zad[i]<<"\t"<<array6[i]<<"\t"<<array6zad[i]<<
                  array7[i]<<"\t"<<array7zad[i]<<"\t"<<array8[i]<<"\t"<<array9[i]<<"\t"<<array10[i]<<"\t"<<array11[i]<<"\t"<<array12[i]<<"\n";
        }
        fout.close();
        this->close();
        return "Данные успешно записаны в файл";
    }
    else
    {
        QMessageBox::about(this,"Ошибка", "Не удалось открыть файл для записи");
        this->close();
        return "Данные не записаны, файл не открылся";
    }
}
