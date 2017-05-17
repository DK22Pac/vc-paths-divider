#include "VC_PathsDivider.h"
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QTextStream>

VC_PathsDivider::VC_PathsDivider(QWidget *parent) : QMainWindow(parent) {
    // Размеры и название окна
    setMinimumSize(400, 75);
    setMaximumSize(400, 75);
    setWindowTitle("GTA Vice City Paths Divider");

    // Элементы - строка, 2 кнопки и переключатель
    QLineEdit *lePath = new QLineEdit(this);
    QPushButton *btnBrowse = new QPushButton("Browse...", this);
    QPushButton *btnDivide = new QPushButton("Divide", this);
    QDoubleSpinBox *sbDivider = new QDoubleSpinBox(this);

    // Расположение
    lePath->setGeometry(10, 10, 300, 20);
    btnBrowse->setGeometry(320, 7, 70, 25);
    btnDivide->setGeometry(200, 40, 190, 25);
    sbDivider->setGeometry(10, 42, 180, 20);

    // При нажатии на "Browse..."
    connect(btnBrowse, &QPushButton::clicked, [=]() {
        lePath->setText(QFileDialog::getOpenFileName(this, "Select paths.ipl file", "", "IPL files (*.ipl)"));
    });

    // При нажатии на "Divide"
    connect(btnDivide, &QPushButton::clicked, [=]() {
        btnDivide->setEnabled(false); // временно "выключаем" кнопку
        if (!lePath->text().isEmpty() && sbDivider->value() != 0 && sbDivider->value() != 1) { // если строка не пустая, а делитель не равен '0' или '1'
            QFile inputFile(lePath->text());
            if (inputFile.open(QIODevice::ReadOnly)) { // если файл открылся
                QStringList outputLines; // список строк (пока что пустой), которые будут записаны в выходной файл
                while (!inputFile.atEnd()) { // пока не достигнем окончания файла
                    outputLines.append(inputFile.readLine()); // записываем строку в список
                    if (outputLines.last().startsWith("path")) { // если открывается секция 'path'
                        outputLines.append(inputFile.readLine()); // следующая за 'path' строка
                        while (!outputLines.last().startsWith("end")) { // читаем секцию, пока не наткнемся на 'end'
                            for (unsigned int i = 0; i < 12; ++i) { // следуюшие 12 строк
                                QString line = inputFile.readLine(); // получаем строку
                                QStringList lineValues = line.split(','); // разбиваем на части
                                for (unsigned int comp = 3; comp <= 5; ++comp) // делим 4-ый, 5-ый, 6-ой компонент (x,y,z)
                                    lineValues[comp] = QString(' ') + QString::number(lineValues[comp].toDouble() / sbDivider->value());
                                outputLines.append(lineValues.join(',')); // собираем обратно в одну строку и добавляем в список
                            }
                            outputLines.append(inputFile.readLine());
                        }
                    }
                }
                inputFile.close();
                QString outputPath = QFileDialog::getSaveFileName(this, "Save to file", lePath->text(), "IPL files(*.ipl)");
                if(!outputPath.isEmpty()){
                    QFile outputFile(outputPath);
                    if (outputFile.open(QIODevice::WriteOnly)) { // если файл открылся
                        QTextStream outStream(&outputFile);
                        for(QString &str : outputLines) // записываем строки из списка в новый файл
                            outStream << str;
                    }
                    outputFile.close();
                }
            }
        }
        btnDivide->setEnabled(true); // "включаем" кнопку обратно
    });
}