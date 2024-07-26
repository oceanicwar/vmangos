CREATE TABLE IF NOT EXISTS `actions_enabled` (
  `script_name` varchar(50) COLLATE armscii8_bin NOT NULL DEFAULT '',
  `enabled` tinyint(4) DEFAULT '1',
  PRIMARY KEY (`script_name`)
) ENGINE=InnoDB DEFAULT CHARSET=armscii8 COLLATE=armscii8_bin;

DELETE FROM `actions_enabled`;
INSERT INTO `actions_enabled` (`script_name`, `enabled`) VALUES
	('FlexibleInstances', 1),
	('PlayerAnnouncer', 1);