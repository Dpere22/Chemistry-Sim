#include "mainwindow.h"

#include <QApplication>

/// @brief Application for teaching basic chemistry concepts
///
/// @author Goobers team
/// @author Diego Perez u1301598
/// @author Gabe Eralie u1344808
/// @author Jackson Heaps u1438591
/// @author William Yavornitzky u1381262
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model model;
    BounceEngine engine;
    MainWindow w(model, engine);
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
