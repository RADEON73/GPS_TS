#include <QtCore/QCoreApplication>
#include <QtCore/QTextCodec>
#include "core/Application.h"
#include "encoding_fix.h"

using namespace std;

int main(int argc, char *argv[])
{
    // ��������� UTF-8 ��������� �� ���������

    // ��� Windows ������������� ����������� �������


    //TODO: ���������� �������� ���������� ��������� �� ����� �������������� ��� ������� ��������� �� ��� ���� ����, ���� �������������� � ������������������� ��� ���� ��������������

    Application app(argc, argv);

    //TODO: �������� �������� �� ��������� ������ (������������� ��� ��������)

    return Application::exec();
}
