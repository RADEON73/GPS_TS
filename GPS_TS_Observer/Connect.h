#pragma once
#include <QDialog>
#include "ui_Connect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ConnectForm; }
QT_END_NAMESPACE

class Connect : public QDialog, public Ui::ConnectForm
{
	Q_OBJECT

signals:
	void authRequest(QString login, QString pass);

public:
	explicit Connect(QWidget* parent = nullptr);
	~Connect() final = default;

	void showLogonForm();

public slots:
	void reject() override;

private slots:
	void on_authorizationBtn_clicked();
};