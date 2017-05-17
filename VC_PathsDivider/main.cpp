#include "VC_PathsDivider.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    VC_PathsDivider w;
    w.show();
    return a.exec();
}