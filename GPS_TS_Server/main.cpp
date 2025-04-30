#include <QtCore/QCoreApplication>
#include <QtCore/QTextCodec>
#include "core/Application.h"
#include "encoding_fix.h"

using namespace std;

int main(int argc, char *argv[])
{
    //TODO: желательно добавить перезапуск программы от имени администратора при попытке запустить ее без этих прав, либо предупреждение о неработоспособности без прав администратора

    Application app(argc, argv);

    //TODO: добавить проверку на повторный запуск (предотвращать два процесса)

    return Application::exec();
}
