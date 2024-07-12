CREATE TABLE IF NOT EXISTS `flex_instance_template` (
  `map_id` int(11) NOT NULL DEFAULT '0',
  `player_count` int(11) NOT NULL DEFAULT '10',
  `hp_multi` float NOT NULL DEFAULT '1',
  `dmg_multi` float DEFAULT '1',
  `xp_multi` float DEFAULT '1',
  `gold_multi` float DEFAULT '1',
  `comment` varchar(50) COLLATE armscii8_bin DEFAULT '',
  PRIMARY KEY (`map_id`,`player_count`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=armscii8 COLLATE=armscii8_bin;

INSERT INTO `flex_instance_template` (`map_id`, `player_count`, `hp_multi`, `dmg_multi`, `xp_multi`, `gold_multi`, `comment`) VALUES
	(36, 1, 0.33, 0.33, 0.33, 0.33, 'Deadmines - 1M'),
	(36, 2, 2, 2, 1, 1, 'Deadmines - 2M+'),
	(409, 1, 0.1, 0.1, 1, 1, 'Molten Core - 1M'),
	(409, 2, 0.2, 0.2, 1, 1, 'Molten Core - 2M+');