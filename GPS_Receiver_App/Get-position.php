<?php
// Tâches DEV: Développer la fonction PHP pour récupérer les données

// --- 1. Connexion à la BDD ---
// Remplacez par vos identifiants
$host = 'localhost';
$db   = 'votre_base_de_donnees';
$user = 'votre_utilisateur';
$pass = 'votre_mot_de_passe';
$charset = 'utf8mb4';

$dsn = "mysql:host=$host;dbname=$db;charset=$charset";
$options = [
    PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    PDO::ATTR_EMULATE_PREPARES   => false,
];

try {
     $pdo = new PDO($dsn, $user, $pass, $options);
} catch (\PDOException $e) {
     http_response_code(500);
     die("Erreur de connexion BDD: " . $e->getMessage());
}

// --- 2. Requête pour récupérer les dernières positions ---
// La structure de la table BATEAU et de la table POSITION est définie par le MCD du RT.
// Ici, on récupère les dernières positions de tous les bateaux.
$stmt = $pdo->query("
    SELECT b.nom, p.latitude, p.longitude, p.timestamp
    FROM position p
    JOIN bateau b ON p.id_bateau = b.id
    ORDER BY p.timestamp DESC
");

$positions = $stmt->fetchAll();

// --- 3. Renvoyer les données en JSON ---
header('Content-Type: application/json');
echo json_encode($positions);

?>