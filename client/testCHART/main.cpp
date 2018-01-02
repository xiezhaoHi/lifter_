#include "testCHART.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	testCHART w;
	w.show();
	return a.exec();
}
