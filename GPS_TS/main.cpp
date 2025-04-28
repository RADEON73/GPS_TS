#include <QtCore/QCoreApplication>
#include "core/Application.h"

using namespace std;

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    return Application::exec();
}
