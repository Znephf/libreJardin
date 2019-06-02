BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS `unites` (
	`id`	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT
);
INSERT INTO `unites` (id,designation) VALUES (1,'UNITE'),
 (2,'HEURES'),
 (3,'JOURS'),
 (4,'FORFAIT'),
 (5,'KG'),
 (6,'METRES'),
 (7,'M²'),
 (8,'LITRES');
CREATE TABLE IF NOT EXISTS `type_de_tache` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`couleur`	TEXT,
	`forme`	INTEGER
);
INSERT INTO `type_de_tache` (id,designation,couleur,forme) VALUES (1,'Phases','#00ffff',6),
 (2,'Préparation des sols','#ffaa00',1),
 (3,'Labour','#aa5500',2),
 (4,'Préparation lit de semence','#aaaa7f',1),
 (5,'Semis','#ffff00',8),
 (6,'documentations','#00ff00',1),
 (7,'Fertilisation','#00ff00',2),
 (8,'Récolte','#ff5500',1),
 (9,'Fin de culture','#ff0000',5),
 (10,'Arrosages','#00ffff',5);
CREATE TABLE IF NOT EXISTS `type_de_moyen` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`commentaires`	TEXT
);
INSERT INTO `type_de_moyen` (id,designation,commentaires) VALUES (1,'Personnel','Personnel de l''entreprise'),
 (2,'Matériel',NULL),
 (3,'Prestations extérieures',NULL),
 (4,'Fournitures',NULL);
CREATE TABLE IF NOT EXISTS `tasks` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`commentaires`	TEXT,
	`depart`	TEXT,
	`fin`	TEXT,
	`duree`	INTEGER,
	`precedent`	INTEGER,
	`phase_parent`	INTEGER,
	`avancement`	INTEGER,
	`type`	INTEGER,
	`contrainte_date`	INTEGER,
	`id_culture`	INTEGER
);

CREATE TABLE IF NOT EXISTS `ressources` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`commentaires`	TEXT,
	`tache`	INTEGER,
	`cout_prevu`	NUMERIC,
	`cout_actualise`	NUMERIC
);

CREATE TABLE IF NOT EXISTS `moyens` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`commentaires`	TEXT,
	`cout_unitaire`	NUMERIC,
	`coordonnees`	INTEGER,
	`type`	INTEGER,
	`unite`	INTEGER
);

CREATE TABLE IF NOT EXISTS `liste_ressources` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`id_task`	INTEGER,
	`id_ressource`	INTEGER
);

CREATE TABLE IF NOT EXISTS `liste_moyens` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`designation`	TEXT,
	`id_ressource`	INTEGER,
	`id_moyen`	INTEGER,
	`quantite_prevu`	NUMERIC,
	`quantite_realise`	NUMERIC,
	`unite`	INTEGER
);

CREATE TABLE IF NOT EXISTS `coordonnees` (
	`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	`societe`	TEXT,
	`commentaires`	TEXT,
	`nom`	TEXT,
	`prenom`	TEXT,
	`adresse`	TEXT,
	`code_postal`	TEXT,
	`ville`	TEXT,
	`pays`	TEXT,
	`telephone_fixe`	TEXT,
	`portable`	TEXT,
	`fax`	TEXT,
	`email`	TEXT
);

COMMIT;
