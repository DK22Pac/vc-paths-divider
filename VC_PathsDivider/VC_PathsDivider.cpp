#include "VC_PathsDivider.h"
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QTextStream>

VC_PathsDivider::VC_PathsDivider(QWidget *parent) : QMainWindow(parent) {
    // ������� � �������� ����
    setMinimumSize(400, 75);
    setMaximumSize(400, 75);
    setWindowTitle("GTA Vice City Paths Divider");

    // �������� - ������, 2 ������ � �������������
    QLineEdit *lePath = new QLineEdit(this);
    QPushButton *btnBrowse = new QPushButton("Browse...", this);
    QPushButton *btnDivide = new QPushButton("Divide", this);
    QDoubleSpinBox *sbDivider = new QDoubleSpinBox(this);

    // ������������
    lePath->setGeometry(10, 10, 300, 20);
    btnBrowse->setGeometry(320, 7, 70, 25);
    btnDivide->setGeometry(200, 40, 190, 25);
    sbDivider->setGeometry(10, 42, 180, 20);

    // ��� ������� �� "Browse..."
    connect(btnBrowse, &QPushButton::clicked, [=]() {
        lePath->setText(QFileDialog::getOpenFileName(this, "Select paths.ipl file", "", "IPL files (*.ipl)"));
    });

    // ��� ������� �� "Divide"
    connect(btnDivide, &QPushButton::clicked, [=]() {
        btnDivide->setEnabled(false); // �������� "���������" ������
        if (!lePath->text().isEmpty() && sbDivider->value() != 0 && sbDivider->value() != 1) { // ���� ������ �� ������, � �������� �� ����� '0' ��� '1'
            QFile inputFile(lePath->text());
            if (inputFile.open(QIODevice::ReadOnly)) { // ���� ���� ��������
                QStringList outputLines; // ������ ����� (���� ��� ������), ������� ����� �������� � �������� ����
                while (!inputFile.atEnd()) { // ���� �� ��������� ��������� �����
                    outputLines.append(inputFile.readLine()); // ���������� ������ � ������
                    if (outputLines.last().startsWith("path")) { // ���� ����������� ������ 'path'
                        outputLines.append(inputFile.readLine()); // ��������� �� 'path' ������
                        while (!outputLines.last().startsWith("end")) { // ������ ������, ���� �� ��������� �� 'end'
                            for (unsigned int i = 0; i < 12; ++i) { // ��������� 12 �����
                                QString line = inputFile.readLine(); // �������� ������
                                QStringList lineValues = line.split(','); // ��������� �� �����
                                for (unsigned int comp = 3; comp <= 5; ++comp) // ����� 4-��, 5-��, 6-�� ��������� (x,y,z)
                                    lineValues[comp] = QString(' ') + QString::number(lineValues[comp].toDouble() / sbDivider->value());
                                outputLines.append(lineValues.join(',')); // �������� ������� � ���� ������ � ��������� � ������
                            }
                            outputLines.append(inputFile.readLine());
                        }
                    }
                }
                inputFile.close();
                QString outputPath = QFileDialog::getSaveFileName(this, "Save to file", lePath->text(), "IPL files(*.ipl)");
                if(!outputPath.isEmpty()){
                    QFile outputFile(outputPath);
                    if (outputFile.open(QIODevice::WriteOnly)) { // ���� ���� ��������
                        QTextStream outStream(&outputFile);
                        for(QString &str : outputLines) // ���������� ������ �� ������ � ����� ����
                            outStream << str;
                    }
                    outputFile.close();
                }
            }
        }
        btnDivide->setEnabled(true); // "��������" ������ �������
    });
}