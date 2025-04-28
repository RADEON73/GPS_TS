// ConfigManager.h
/*
#pragma once
#include <QObject>
#include <QString>
#include <QVariantMap>

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject* parent = nullptr);
    
    bool loadConfig(const QString& filePath);
    bool saveConfig(const QString& filePath) const;
    void setDefaultConfig();
    
    QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant()) const;
    void setValue(const QString& key, const QVariant& value);
    
private:
    QVariantMap m_config;
    void createDefaultConfig();
};
*/