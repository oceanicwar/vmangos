CREATE TABLE IF NOT EXISTS `flex_instance_template` (
  `map_id` int(11) NOT NULL DEFAULT '0',
  `player_count` int(11) NOT NULL DEFAULT '10',
  `hp_multi` float NOT NULL DEFAULT '1',
  `dmg_multi` float DEFAULT '1',
  `comment` varchar(50) COLLATE armscii8_bin DEFAULT '',
  PRIMARY KEY (`map_id`,`player_count`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=armscii8 COLLATE=armscii8_bin;

DELETE FROM `flex_instance_template`;
INSERT INTO `flex_instance_template` (`map_id`, `player_count`, `hp_multi`, `dmg_multi`, `comment`) VALUES
	(409, 10, 0.4, 0.65, 'Molten Core - 10M'),
	(409, 20, 0.5, 0.75, 'Molten Core - 20M');