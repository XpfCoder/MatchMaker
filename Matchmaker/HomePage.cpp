#include "HomePage.h"
#include <QMovie>
#include "CoreObject.h"
HomePage::HomePage(QWidget *parent)
{
    ui.setupUi(this);

    QMovie* movie = new QMovie(":/Matchmaker/image/dragon.gif");
    ui.logo->setMovie(movie);
    ui.logo->setScaledContents(true);
    movie->start();
    ui.logo->show();

    auto coreObject = CoreObject::instance();
    coreObject->init();
}

HomePage::~HomePage()
{

}
