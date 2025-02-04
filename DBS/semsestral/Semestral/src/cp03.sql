-- DROP TABLE IF EXISTS Player;
-- DROP TABLE IF EXISTS Character_skills;
-- DROP TABLE IF EXISTS Skills;
-- DROP TABLE IF EXISTS Stats;
-- DROP TABLE IF EXISTS Hostile;
-- DROP TABLE IF EXISTS Covenant_membership;
-- DROP TABLE IF EXISTS Weapon;
-- DROP TABLE IF EXISTS Armor;
-- DROP TABLE IF EXISTS Developer_Character;
-- DROP TABLE IF EXISTS Developer_Covenant;
-- DROP TABLE IF EXISTS Developer_item;
-- DROP TABLE IF EXISTS In_fight;
-- DROP TABLE IF EXISTS Fight;
-- DROP TABLE IF EXISTS Covenant;
-- DROP TABLE IF EXISTS Item;
-- DROP TABLE IF EXISTS Character;
-- DROP TABLE IF EXISTS Developer_special_access;
-- DROP TABLE IF EXISTS Special_access;
-- DROP TABLE IF EXISTS Developer;
-- DROP TABLE IF EXISTS "User";

CREATE TABLE "User"
(
    Username VARCHAR(15) NOT NULL UNIQUE,
    Login VARCHAR(15) NOT NULL UNIQUE,
    User_id SERIAL NOT NULL,
    Password VARCHAR(30),
    PRIMARY KEY (User_id)
);

CREATE TABLE Player
(
    User_id INT NOT NULL,
    Username VARCHAR(15) NOT NULL,
    CONSTRAINT Player_User_id_fk FOREIGN KEY (User_id) REFERENCES "User" (User_id),
    CONSTRAINT Player_User_username_fk FOREIGN KEY (Username) REFERENCES "User" (Username),
    PRIMARY KEY (User_id)
);

CREATE TABLE Developer
(
    User_id INT NOT NULL,
    Username VARCHAR(15) NOT NULL,
    CONSTRAINT Developer_User_fk FOREIGN KEY (User_id) REFERENCES "User" (User_id),
    CONSTRAINT Developer_User_username_fk FOREIGN KEY (Username) REFERENCES "User" (Username),
    PRIMARY KEY (User_id)
);

CREATE TABLE Special_access
(
    Name VARCHAR(126) PRIMARY KEY
);

CREATE TABLE Developer_special_access
(
    Developer_id INT NOT NULL CONSTRAINT Developer_special_access_Developer_id_fk REFERENCES Developer ON UPDATE CASCADE,
    Special_access VARCHAR(126) NOT NULL CONSTRAINT Developer_special_access_Developer_name_fk REFERENCES Special_access ON UPDATE CASCADE,
    PRIMARY KEY (Developer_id, Special_access)
);

CREATE TABLE Character
(
    User_id INT NOT NULL UNIQUE CONSTRAINT Character_User_id_fk REFERENCES "User",
    Character_id SERIAL PRIMARY KEY,
    Name VARCHAR(25) NOT NULL UNIQUE,
    Creation_date DATE NOT NULL
);

CREATE TABLE Skills
(
    Name VARCHAR(20)
        CONSTRAINT Skills_pkey PRIMARY KEY
);

CREATE TABLE Character_skills
(
    Skill VARCHAR(50) NOT NULL CONSTRAINT Character_skills_skills_name_fk REFERENCES Skills,
    Character_id INT NOT NULL CONSTRAINT Character_skills_character_id_fk REFERENCES Character,
    PRIMARY KEY (Character_id, Skill)
);

CREATE TABLE Stats
(
    Vigor INT NOT NULL,
    Endurance INT NOT NULL,
    Mind INT NOT NULL,
    Strength INT NOT NULL,
    Dexterity INT NOT NULL,
    Character_id INT NOT NULL CONSTRAINT Stats_character_id_fk REFERENCES Character,
    PRIMARY KEY (Character_id)
);

CREATE TABLE Covenant
(
    Covenant_id SERIAL PRIMARY KEY,
    Covenant_name VARCHAR(50) NOT NULL UNIQUE,
    Covenant_head INT NOT NULL UNIQUE CONSTRAINT Covenant_head_id_fk REFERENCES Character,
    Location VARCHAR(50),
    Banner VARCHAR(100)
);

CREATE TABLE Covenant_membership
(
    Covenant_id INT CONSTRAINT covenant_id_fk REFERENCES Covenant,
    Member_id INT NOT NULL UNIQUE CONSTRAINT member_id_fk REFERENCES Character,
    PRIMARY KEY (Member_id, Covenant_id)
);

CREATE TABLE Hostile
(
    Covenant_id INT NOT NULL CONSTRAINT covenant_id_fk REFERENCES Covenant,
    Hostile_id INT NOT NULL CONSTRAINT enemy_id_fk REFERENCES Covenant,
    PRIMARY KEY (Covenant_id, Hostile_id)
);

CREATE TABLE Item
(
    Item_id SERIAL NOT NULL,
    Character_id INT NOT NULL,
    Item_name VARCHAR(25) NOT NULL UNIQUE,
    Item_type VARCHAR(20) NOT NULL,
    PRIMARY KEY (Character_id, Item_id),
    CONSTRAINT Unique_Item_id UNIQUE (Item_id),
    CONSTRAINT Item_Character_id_fk FOREIGN KEY (Character_id) REFERENCES Character
);

CREATE TABLE Weapon
(
    Item_id INT NOT NULL,
    Character_id INT NOT NULL,
    Item_name VARCHAR(25) NOT NULL,
    Type VARCHAR(20) NOT NULL,
    Damage INT NOT NULL,
    CONSTRAINT Weapon_Item_id_fk FOREIGN KEY (Item_id, Character_id) REFERENCES Item(Item_id, Character_id),
    CONSTRAINT Weapon_Item_name_fk FOREIGN KEY (Item_name) REFERENCES Item(Item_name)
);

CREATE TABLE Armor
(
    Item_id INT NOT NULL,
    Character_id INT NOT NULL,
    Item_name VARCHAR(25) NOT NULL,
    Type VARCHAR(20),
    Defense INT,
    Passive_skill VARCHAR(50),
    CONSTRAINT Armor_Item_id_fk FOREIGN KEY (Item_id, Character_id) REFERENCES Item(Item_id, Character_id),
    CONSTRAINT Armor_Item_name_fk FOREIGN KEY (Item_name) REFERENCES Item(Item_name)

);

CREATE TABLE Fight
(
    Enemy VARCHAR(30) NOT NULL,
    Place VARCHAR(30) NOT NULL,
    Time TIMESTAMP NOT NULL,
    Fight_id SERIAL PRIMARY KEY
);

CREATE TABLE In_fight
(
    Character_id INT NOT NULL CONSTRAINT In_fight_character_id_fk REFERENCES Character,
    Fight_id INT NOT NULL CONSTRAINT  In_fight_fight_id_fk REFERENCES Fight,
    PRIMARY KEY (Character_id, Fight_id)
);

CREATE TABLE Developer_Character
(
    Developer_id INT NOT NULL CONSTRAINT  Developer_Character_Developer_id_fk REFERENCES Developer,
    Character_id INT NOT NULL CONSTRAINT Developer_Character_Character_id_fk REFERENCES Character,
    Special_access VARCHAR(126) NOT NULL CONSTRAINT Developer_Character_Special_access_name_fk REFERENCES Special_access,
    PRIMARY KEY (Developer_id, Character_id)

);

CREATE TABLE Developer_Covenant
(
    Developer_id INT NOT NULL CONSTRAINT  Developer_Character_Developer_id_fk REFERENCES Developer,
    Covenant_id INT NOT NULL CONSTRAINT Developer_Character_Covenant_id_fk REFERENCES Covenant,
    Special_access VARCHAR(126) NOT NULL CONSTRAINT Developer_Character_Special_access_name_fk REFERENCES Special_access,
    PRIMARY KEY (Developer_id, Covenant_id)
);

CREATE TABLE Developer_item
(
    Developer_id INT NOT NULL,
    Item_id INT NOT NULL,
    Special_access VARCHAR(126) NOT NULL,
    CONSTRAINT Developer_item_Developer_id_fk FOREIGN KEY (Developer_id) REFERENCES Developer(User_id),
    CONSTRAINT Developer_item_Item_id_fk FOREIGN KEY (Item_id) REFERENCES Item(Item_id),
    CONSTRAINT Developer_item_Special_access_fk FOREIGN KEY (Special_access) REFERENCES Special_access(Name)
);

--filling database with data

-- insert into "User" (Username, Login, User_id, Password) values ('ahamal0', 'nbellenie0', 1, 'cU1\&PPI');
-- insert into "User" (Username, Login, User_id, Password) values ('sgaish1', 'cskitteral1', 2, 'cU3<h');
-- insert into "User" (Username, Login, User_id, Password) values ('btriplett2', 'qdoerffer2', 3, 'bF7&4*"k');
-- insert into "User" (Username, Login, User_id, Password) values ('iferbrache3', 'dpimer3', 4, 'zI3~w');
-- insert into "User" (Username, Login, User_id, Password) values ('bstrother4', 'gshrimpling4', 5, 'pJ1|v');
-- insert into "User" (Username, Login, User_id, Password) values ('osilbersak5', 'qtimson5', 6, 'cK3+3');
-- insert into "User" (Username, Login, User_id, Password) values ('lblues6', 'nbamlett6', 7, 'kD9&sZB');
-- insert into "User" (Username, Login, User_id, Password) values ('korgan7', 'emeasen7', 8, 'oS4?"T');
-- insert into "User" (Username, Login, User_id, Password) values ('ssarney8', 'dbridgwood8', 9, 'dW4\');
-- insert into "User" (Username, Login, User_id, Password) values ('jwatkiss9', 'rfulkes9', 10, 'lT6?/%#0');

-- insert into "User" (Username, Login, User_id, Password) values ('glaister0', 'bisaak0', 11, 'uP7+MQ');
-- insert into "User" (Username, Login, User_id, Password) values ('trozanski1', 'cpetrovic1', 12, 'mN6.');
-- insert into "User" (Username, Login, User_id, Password) values ('fdoncaster2', 'cullock2', 13, 'hD0~q0');
-- insert into "User" (Username, Login, User_id, Password) values ('rcamamile3', 'ahendrich3', 14, 'kG6>l\(');
-- insert into "User" (Username, Login, User_id, Password) values ('ireuter4', 'ckalberer4', 15, 'tC0~ZGc');
-- insert into "User" (Username, Login, User_id, Password) values ('tremirez5', 'ltatlow5', 16, 'iZ0,I.');
-- insert into "User" (Username, Login, User_id, Password) values ('esancho6', 'wbrade6', 17, 'nD5*Qt');
-- insert into "User" (Username, Login, User_id, Password) values ('cwedderburn7', 'cmatiebe7', 18, 'wW7~{');
-- insert into "User" (Username, Login, User_id, Password) values ('hpancoast8', 'mavann8', 19, 'pK5\c');
-- insert into "User" (Username, Login, User_id, Password) values ('cmewburn9', 'feger9', 20, 'zH4#h8*');

-- INSERT INTO Player(User_id, Username)
-- SELECT User_id, Username
-- FROM "User"
-- WHERE User_id NOT IN (SELECT User_id FROM Developer)
-- ORDER BY RANDOM()
-- LIMIT 10;
--
-- INSERT INTO Developer(User_id, Username)
-- SELECT User_id, Username
-- FROM "User"
-- WHERE User_id NOT IN (SELECT User_id FROM Player)
-- ORDER BY RANDOM()
-- LIMIT 10;

-- INSERT INTO Special_access (Name) VALUES
--                                       ('Can change item stats'),
--                                       ('Can change covenant names'),
--                                       ('Can change character skills'),
--                                       ('Can change covenants members'),
--                                       ('Can change covenants enemies'),
--                                       ('Can change items types'),
--                                       ('Can change characters stats'),
--                                       ('Can change weapons skills'),
--                                       ('Can change armors passive skills'),
--                                       ('Can change characters items');
--
--
-- INSERT INTO Developer_special_access (Developer_id, Special_access) VALUES
--                                                                         (8, 'Can change items types'),
--                                                                         (11, 'Can change characters stats'),
--                                                                         (8, 'Can change weapons skills'),
--                                                                         (19, 'Can change characters items'),
--                                                                         (3, 'Can change armors passive skills'),
--                                                                         (8, 'Can change covenants enemies'),
--                                                                         (4, 'Can change item stats'),
--                                                                         (17, 'Can change covenants members'),
--                                                                         (11, 'Can change covenant names'),
--                                                                         (17, 'Can change character skills');

-- INSERT INTO Character (User_id, Character_id, Name, Creation_date) VALUES(5, 1378, 'Corhyn', '1/19/2020');
-- INSERT INTO Character (User_id, Character_id, Name, Creation_date) VALUES(16, 0666, 'Varre', '3/20/2022');
-- INSERT INTO Character (User_id, Character_id, Name, Creation_date) VALUES(2, 8305, 'Fia', '6/30/2023');
-- INSERT INTO Character (User_id, Character_id, Name, Creation_date) VALUES(1, 5320, 'Ranni', '11/26/2021');
-- INSERT INTO Character (User_id, Character_id, Name, Creation_date) VALUES(9, 1623, 'Blaidd', '5/9/2020');
-- INSERT INTO Character (User_id, Character_id, Name, Creation_date) VALUES(10, 1735, 'Gowry', '4/26/2020');
-- INSERT INTO Character (User_id, Character_id, Name, Creation_date) VALUES(18, 8324, 'Millicent', '1/11/2022');
-- INSERT INTO Character (User_id, Character_id, Name, Creation_date) VALUES(7, 2645, 'Melina', '4/9/2024');
-- INSERT INTO Character (User_id, Character_id, Name, Creation_date) VALUES(3, 5126, 'Jerren', '3/1/2020');
-- INSERT INTO Character (User_id, Character_id, Name, Creation_date) VALUES(15, 4275, 'Latenna','1/4/2024');

--     INSERT INTO Skills (Name) VALUES ('Shard spiral'),
--                                      ('Comet'),
--                                      ('Star shower'),
--                                      ('Starlight'),
--                                      ('Blade barrage'),
--                                      ('Thundercrash'),
--                                      ('Shadowshot'),
--                                      ('Spectral blades'),
--                                      ('Daybreak'),
--                                      ('Needlestorm');


--     WITH RandomCharacterSkill AS (
--         SELECT Skill, Character_id, ROW_NUMBER() OVER (ORDER BY RANDOM()) AS rn
--         FROM (
--             SELECT Name AS Skill
--             FROM Skills ORDER BY RANDOM()) AS RandomSkill
--         CROSS JOIN (
--             SELECT Character_id
--             FROM Character
--             ORDER BY RANDOM())
--         AS RandomCharacter)
--     INSERT INTO Character_skills (Skill, Character_id)
--         SELECT Skill, Character_id
--         FROM RandomCharacterSkill
--         WHERE  rn <= 10;


-- INSERT INTO Stats (Vigor, Endurance, Mind, Strength, Dexterity, Character_id)
-- SELECT
--     FLOOR(RANDOM() * (95 - 5 + 1) + 5) AS Vigor,
--     FLOOR(RANDOM() * (95 - 5 + 1) + 5) AS Endurance,
--     FLOOR(RANDOM() * (95 - 5 + 1) + 5) AS Mind,
--     FLOOR(RANDOM() * (95 - 5 + 1) + 5) AS Strength,
--     FLOOR(RANDOM() * (95 - 5 + 1) + 5) AS Dexterity,
--     Character_id
-- FROM
--     (SELECT Character_id FROM Character LIMIT 10) AS c;

-- INSERT INTO Covenant (Covenant_name, Covenant_head, Location, Banner) values ('Volcano manor', 1378, 'Mt. Gelmir', 'Scarlet flame'),
--                                                                             ('Mohgwyn dynasty', 666, 'Siofra river', 'Bloody spear'),
--                                                                              ('Carian royal family', 5320, 'Academy of Raya Lucaria', 'Dark moon'),
--                                                                              ('Those who live in death', 8305, 'Deeproot depths', 'Scull'),
--                                                                              ('Golden order faction', 2645, 'Erd tree', 'Golden tree'),
--                                                                              ('Frenzied flame', 1735, 'Lands between', 'Three fingers'),
--                                                                              ('Primeval sorcerers', 5126, NULL, 'Magic stuff'),
--                                                                              ('Malenias faction', 8324, 'Haligtree', 'Scarlet tree'),
--                                                                              ('Radahns faction', 1623, 'Caelid', 'Falling star'),
--                                                                              ('Village of the Albinaurics', 4275, 'Village of the Albinaurics', 'Old house');
--
-- INSERT INTO Covenant_membership (Covenant_id, Member_id) VALUES (9, 1378),
--                                                                 (10, 666),
--                                                                 (11, 5320),
--                                                                 (12, 8305),
--                                                                 (13, 2645),
--                                                                 (14, 1735),
--                                                                 (15, 5126),
--                                                                 (16, 8324),
--                                                                 (17, 1623),
--                                                                 (18, 4275);


--     INSERT INTO Hostile (Covenant_id, Hostile_id) VALUES (9, 10),
--                                                          (12, 13),
--                                                          (16, 17),
--                                                          (18, 11),
--                                                          (14, 13),
--                                                          (13, 10),
--                                                          (10, 15),
--                                                          (15, 11),
--                                                          (16, 9),
--                                                          (16, 15);


--     INSERT INTO Item (Item_id, Character_id, Item_name, Item_type)  VALUES (1001, 1378, 'Sacred seal', 'Weapon'),
--                                                                            (2001, 1378, 'Holy robe', 'Armor'),
--                                                                            (1002, 0666, 'Steel bouquet', 'Weapon'),
--                                                                            (2002, 0666, 'White mask', 'Armor'),
--                                                                            (1003, 8305, 'Black knife', 'Weapon'),
--                                                                            (2003, 8305, 'Death companion robe', 'Armor'),
--                                                                            (1004, 5320, 'Dark Moon Greatesword', 'Weapon'),
--                                                                            (2004, 5320, 'Snow witch robe', 'Armor'),
--                                                                            (1005, 1623, 'Royal Greatesword', 'Weapon'),
--                                                                            (2005, 1623, 'Wolf mask', 'Armor'),
--                                                                            (1006, 1735, 'Monks Flamemace', 'Weapon'),
--                                                                            (2006, 1735, 'Fire monk hood', 'Armor'),
--                                                                            (1007, 8324, 'Shamshir', 'Weapon'),
--                                                                            (2007, 8324, 'Golden Prosthetic', 'Armor'),
--                                                                            (1008, 2645, 'Blade of Calling', 'Weapon'),
--                                                                            (2008, 2645, 'Travelers robe', 'Armor'),
--                                                                            (1009, 5126, 'Flamberge', 'Weapon'),
--                                                                            (2009, 5126, 'Eccentric helm', 'Armor'),
--                                                                            (1010, 4275, 'Hunters bow', 'Weapon'),
--                                                                            (2010, 4275, 'Blue silver robe', 'Armor');


--     INSERT INTO Weapon (Item_id, Character_id, Item_name, Type, Damage)
--     SELECT Item_id, Character_id, Item_name, Item_type, FLOOR(random() * (200 - 100 + 1)) + 100 AS Damage
--     FROM Item
--     WHERE Item_type = 'Weapon';
--
--     INSERT INTO Armor (Item_id, Character_id, Item_name, Type, Defense, Passive_skill)
--     SELECT Item_id, Character_id, Item_name, Item_type, FLOOR(random() * (90 -30 + 1)) + 30 as Defense,
--            (SELECT Passive_skill FROM (SELECT 'Increased physical damage' AS Passive_skill
--     UNION SELECT 'Increased magical resist'
--     UNION SELECT 'Increased magical damage'
--     UNION SELECT 'Increased movement speed'
--     UNION SELECT 'Increased critical hit chance'
--     UNION SELECT 'Decreased MP consumption'
--     UNION SELECT 'Increase HP') AS Skills ORDER BY RANDOM() LIMIT 1) AS Passive_skill
--     FROM Item
--     WHERE Item_type = 'Armor';
--
--     INSERT INTO Armor (Item_id, Character_id, Item_name, Type, Defense, Passive_skill)
--     VALUES (2009, 5126, 'Eccentric helm', 'Armor', 46, 'Increased magical damage');

-- WITH numbered_combinations AS (
--     SELECT
--         c.Character_id,
--         f.Fight_id,
--         ROW_NUMBER() OVER () AS row_num
--     FROM
--         Character c
--             CROSS JOIN
--         Fight f
-- )
-- INSERT INTO In_fight (Character_id, Fight_id)
-- SELECT Character_id, Fight_id
-- FROM numbered_combinations
-- WHERE row_num <= 32000
-- ON CONFLICT DO NOTHING;

-- INSERT INTO Developer_Character (Developer_id, Character_id, Special_access)
-- SELECT d.User_id, c.Character_id, ds.Special_access
-- FROM Developer d
-- CROSS JOIN Character c
-- JOIN Developer_special_access ds ON d.User_id = ds.Developer_id
-- WHERE
--     (d.User_id, c.Character_id, ds.Special_access) NOT IN
--         (SELECT Developer_id, Character_id, Special_access FROM Developer_Character)
--         AND ds.Special_access IN ('Can change character skills', 'Can change characters stats',
--                                   'Can change characters items')
-- ORDER BY random()
-- LIMIT 10;
--
-- INSERT INTO Developer_Item (Developer_id, Item_id, Special_access)
-- SELECT d.User_id, i.Item_id, ds.Special_access
-- FROM Developer d
--          CROSS JOIN Item i
--          JOIN Developer_special_access ds ON d.User_id = ds.Developer_id
-- WHERE
--     (d.User_id, i.Item_id, ds.Special_access) NOT IN
--     (SELECT Developer_id, Item_id, Special_access FROM Developer_Item)
--   AND ds.Special_access IN ('Can change item stats', 'Can change items types', 'Can change weapons skills',
--                             'Can change armors passive skills')
-- ORDER BY random()
-- LIMIT 10;
--
-- INSERT INTO Developer_Covenant (Developer_id, Covenant_id, Special_access)
-- SELECT d.User_id, c.Covenant_id, ds.Special_access
-- FROM Developer d
--          CROSS JOIN Covenant c
--          JOIN Developer_special_access ds ON d.User_id = ds.Developer_id
-- WHERE
--     (d.User_id, c.Covenant_id, ds.Special_access) NOT IN
--     (SELECT Developer_id, Covenant_id, Special_access FROM Developer_Covenant)
--   AND ds.Special_access IN ('Can change covenant names', 'Can change covenants members', 'Can change covenants enemies')
-- ORDER BY random()
-- LIMIT 10;




-------------------------------------------------
--requests:

SELECT
    u.Username,
    s.Name AS Special_access
FROM "User" u
         LEFT JOIN Developer d ON u.User_id = d.User_id
         LEFT JOIN Developer_special_access ds ON d.User_id = ds.Developer_id
         LEFT JOIN Special_access s ON ds.Special_access = s.Name
WHERE s.Name IS NOT NULL;

SELECT c.Name, s.Vigor, S.Endurance, s.Mind, s.Strength, s.Dexterity
FROM Character c
         INNER JOIN  Stats s ON c.Character_id = s.Character_id;

SELECT p.Username, c.Name
FROM Player p
         INNER JOIN Character c ON p.User_id = c.User_id
WHERE c.Creation_date >= '2024-01-01';

SELECT c.Covenant_name, COUNT(cm.Member_id) AS Total_Members
FROM Covenant c
         INNER JOIN Covenant_membership Cm on c.Covenant_id = Cm.Covenant_id
GROUP BY c.Covenant_name
HAVING COUNT(cm.Member_id) > 0;

SELECT c.Name, s.Vigor
FROM Character c
         INNER JOIN Stats s ON c.Character_id = s.Character_id
ORDER BY s.Vigor DESC
    LIMIT 5 OFFSET 2;

SELECT Name
FROM Skills
WHERE Name LIKE 'S%'
UNION
SELECT Name
FROM Skills
WHERE Name LIKE 'T%';

SELECT c.Name, s.Mind
FROM Character c
         INNER JOIN Stats s ON c.Character_id = s.Character_id
WHERE s.Mind > (SELECT AVG(Mind) FROM Stats)
ORDER BY Mind;