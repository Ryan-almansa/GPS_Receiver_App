#include "GPSReceiver.h"
#include <QStringList>
#include <QSerialPort> // Obligatoire car QSerialPort est utilisé ici
#include <QDebug>      // Obligatoire car qDebug/qCritical est utilisé ici

// --- DÉFINITION DU CONSTRUCTEUR (LNK2019 CORRIGÉ) ---
GPSReceiver::GPSReceiver(QObject* parent) : QObject(parent)
{
    m_serialPort = new QSerialPort(this);

    // Connexion du signal readyRead() à notre slot handleReadyRead()
    connect(m_serialPort, &QSerialPort::readyRead, this, &GPSReceiver::handleReadyRead);

    qDebug() << "Récepteur initialisé. Prêt à ouvrir le port.";
}

// Fonction de conversion du format NMEA (ddmm.mmmm) au format décimal (dd.dddddd)
double GPSReceiver::nmeaToDecimal(const QString& nmeaCoord, const QString& direction)
{
    if (nmeaCoord.isEmpty()) return 0.0;

    // 1. Convertir en double pour les opérations mathématiques
    double value = nmeaCoord.toDouble();

    // 2. Extraire les degrés (dd ou ddd)
    // Les degrés sont la partie entière avant les deux derniers chiffres de l'entier
    int degrees = static_cast<int>(value / 100.0);

    // 3. Extraire les minutes (mm.mmmm)
    double minutes = value - (degrees * 100.0);

    // 4. Calculer les degrés décimaux : Degrés + (Minutes / 60)
    double decimalCoord = degrees + (minutes / 60.0);

    // 5. Appliquer la direction (Sud ou Ouest = négatif)
    if (direction == "S" || direction == "W") {
        decimalCoord *= -1.0;
    }

    return decimalCoord;
}





// --- DÉFINITION DE openSerialPort ---
void GPSReceiver::openSerialPort(const QString& portName, int baudRate)
{
    // 2. Configuration et tentative d'ouverture
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(static_cast<QSerialPort::BaudRate>(baudRate));
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::ReadOnly)) {
        qDebug() << "Port série" << portName << "ouvert avec succès à" << baudRate << "bauds.";
        qDebug() << "En attente de trames GPS...";
    }
    else {
        qCritical() << "ERREUR: Impossible d'ouvrir le port" << portName << ":" << m_serialPort->errorString();
    }
}

// --- DÉFINITION DE handleReadyRead (Décodage GPGLL) ---
void GPSReceiver::handleReadyRead()
{
    // Lire toutes les données disponibles
    QByteArray data = m_serialPort->readAll();
    QString rawTrame = QString::fromLatin1(data).trimmed();

    if (rawTrame.isEmpty()) return;

    qDebug() << "--- Nouvelle Trame Reçue (" << data.size() << " octets) ---";
    qDebug() << rawTrame;
    qDebug() << "-------------------------------------------------------------";

    // ... (Logique de décodage GPGLL inchangée) ...
    if (rawTrame.startsWith("$GPGLL")) {
        qDebug() << "Trame GPGLL detectee. Debut du decodage.";
        QStringList fields = rawTrame.split(',');

        if (fields.size() >= 70) {
            QString status = fields.at(6);

            if (status == "A") {
                qDebug() << "Position valide (A) :";
                qDebug() << "  Latitude brute :" << fields.at(1) << fields.at(2);
                qDebug() << "  Longitude brute :" << fields.at(3) << fields.at(4);

                // Étape suivante : Conversion NMEA vers décimal et enregistrement BDD
            }
            // ... (dans la boucle foreach après avoir vérifié le $GPGLL)

             // Vérifier si la position est valide
            if (status == "A") {

                // --- CONVERSION NMEA VERS DÉCIMAL ---
                double latDecimal = nmeaToDecimal(fields.at(1), fields.at(2));
                double lonDecimal = nmeaToDecimal(fields.at(3), fields.at(4));
                // ------------------------------------

                qDebug() << "Position CONVERTIE (Décimale) :";
                qDebug() << "  Latitude:" << QString::number(latDecimal, 'f', 6);
                qDebug() << "  Longitude:" << QString::number(lonDecimal, 'f', 6);

                // ÉTAPE SUIVANTE : STOCKAGE EN BASE DE DONNÉES
                // Cette étape doit être faite AVANT l'affichage web.
                // DatabaseManager::savePosition("NOM_BATEAU", latDecimal, lonDecimal);

            }
            // ... (fin de la fonction)
        }
    }
}