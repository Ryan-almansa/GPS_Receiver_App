#ifndef GPSRECEIVER_H
#define GPSRECEIVER_H

#include <QObject>
#include <QSerialPort> // La classe Qt pour la communication série
#include <QDebug>      // Pour afficher les messages dans la console
#include <QString>     // Inclus implicitement, mais bonne pratique

class GPSReceiver : public QObject
{
    Q_OBJECT
public:
    explicit GPSReceiver(QObject* parent = nullptr);
    void openSerialPort(const QString& portName, int baudRate);

private slots:
    // Slot qui se déclenche automatiquement lorsque de nouvelles données arrivent
    void handleReadyRead();

private:
    QSerialPort* m_serialPort;

    // --- NOUVELLE DÉCLARATION DE LA FONCTION DE CONVERSION ---
    // Cette fonction sera utilisée en interne (private) pour le décodage.
    double nmeaToDecimal(const QString& nmeaCoord, const QString& direction);
    // -----------------------------------------------------------
};

#endif // GPSRECEIVER_H