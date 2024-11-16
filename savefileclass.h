#ifndef SAVEFILECLASS_H
#define SAVEFILECLASS_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QErrorMessage>
#include <QMessageBox>
#include <QDateTime>

namespace Ui {
class SaveFileClass;
}

class SaveFileClass : public QWidget
{
    Q_OBJECT

public:
    explicit SaveFileClass(QWidget *parent = nullptr);
    ~SaveFileClass();


    QString OpenAndWriteFile(std::vector<float>array1, std::vector<float>array2, std::vector<float>array2zad, std::vector<float>array3, std::vector<float>array3zad, std::vector<float>array4, std::vector<float>array4zad,
                                            std::vector<float>array5, std::vector<float>array5zad, std::vector<float>array6, std::vector<float>array6zad, std::vector<float>array7, std::vector<float>array7zad, std::vector<float>array8,
                                            std::vector<float>array9, std::vector<float>array10,std::vector<float>array11,std::vector<float>array12);
    QString OpenAndWriteFileByDate(std::vector<float>array1, std::vector<float>array2, std::vector<float>array2zad, std::vector<float>array3, std::vector<float>array3zad, std::vector<float>array4, std::vector<float>array4zad,
                                            std::vector<float>array5, std::vector<float>array5zad, std::vector<float>array6, std::vector<float>array6zad, std::vector<float>array7, std::vector<float>array7zad, std::vector<float>array8,
                                            std::vector<float>array9, std::vector<float>array10,std::vector<float>array11,std::vector<float>array12);

    std::vector<float> ArrayN1, ArrayN1zad, ArrayN2, ArrayN2zad, ArrayTime;
    std::vector<float> ArrayT3, ArrayT3zad, ArrayT4, ArrayT4zad, ArrayP2, ArrayP2zad;
    std::vector<float> ArrayR, ArrayRzad, ArraydKy, ArraydKyzad, ArrayGt, ArrayAL, ArrayALzad, ArrayKOT3, ArrayKOT5, ArrayRUD, ArrayT0, ArrayP0;

private:
    Ui::SaveFileClass *ui;
    QString PathToFile, PathToFileTXT, PathToFileBIN, PathToGroupFile;
    const char *ArrayStringPathToFile;
    const char *adressBinChar;
    QErrorMessage *ErrorMessage = new QErrorMessage();
    QMessageBox *MessageBox = new QMessageBox();
    QDateTime CurrentDateTime = QDateTime::currentDateTime();


//    MainWindow *window = new MainWindow();
    void SaveFileTXT();

    FILE *FileForGraphic;
    FILE *fileBin;
};

#endif // SAVEFILECLASS_H
