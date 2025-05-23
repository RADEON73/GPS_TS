#include "Connect.h"

Connect::Connect(QWidget* parent) : QDialog(parent)
{
	setupUi(this);
	setWindowFlag(Qt::FramelessWindowHint);
}

void Connect::reject()
{
	QApplication::quit();
}

void Connect::on_authorizationBtn_clicked()
{
	/*if (lastAuthorizedUser && lastAuthorizedUser->getLogin() == loginEdit->text() && lastAuthorizedUser->getPass() == passEdit->text()) {
		accept();
		QMessageBox::critical(this, "Ошибка авторизации", "Этот пользователь уже авторизован");
	}
	else {
		emit authRequest(loginEdit->text(), passEdit->text());
	}*/
}

void Connect::showLogonForm()
{
	/*lastAuthorizedUser = curUsrPtr;
	loginEdit->clear();
	passEdit->clear();
	QWidget::show();*/
}