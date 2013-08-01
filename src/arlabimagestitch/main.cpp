
#include <QtGui/QApplication>
#include "multiview.h"

//#include <QtPlugin>
//Q_IMPORT_PLUGIN(qjpeg) 

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QApplication::addLibraryPath("D:\\Qt\\433Ev\\plugins\\imageformats");
	multiview w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
