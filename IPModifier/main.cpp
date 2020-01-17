#include "IPModifier.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	IPModifier w;
	w.show();
	return a.exec();
}
