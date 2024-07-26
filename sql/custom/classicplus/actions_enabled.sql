CREATE TABLE IF NOT EXISTS `actions_enabled` (
  `script_name` varchar(50) NOT NULL DEFAULT '',
  `enabled` tinyint(4) DEFAULT '1',
  PRIMARY KEY (`script_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DELETE FROM `actions_enabled`;
INSERT INTO `actions_enabled` (`script_name`, `enabled`) VALUES
	('FlexibleInstances', 1),
	('PlayerAnnouncer', 1);