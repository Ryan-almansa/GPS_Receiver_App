#include <QCoreApplication>
#include "GPSReceiver.h" // Seule l'en-tête de la classe est nécessaire ici

int main(int argc, char* argv[])
{
    // Utilisation de QCoreApplication pour une application sans interface graphique
    // (cela est parfait pour la réception de données en console)
    QCoreApplication a(argc, argv);

    // Création de l'objet récepteur
    GPSReceiver receiver;

    // ATTENTION: Remplacez COM5 par le port COM de votre Arduino
    const QString PORT_NAME = "COM4";
    const int BAUD_RATE = 9600;

    receiver.openSerialPort(PORT_NAME, BAUD_RATE);

    // Lancement de l'événement loop de Qt
    return a.exec();
}