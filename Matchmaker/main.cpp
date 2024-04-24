#include "HomePage.h"
#include "MatchMaker.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //HomePage w;
    MatchMaker w;
    w.show();
    return a.exec();
}
