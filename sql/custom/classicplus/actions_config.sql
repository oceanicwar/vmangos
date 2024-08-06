CREATE TABLE IF NOT EXISTS `actions_config` (
  `script_name` varchar(64) NOT NULL DEFAULT '',
  `key` varchar(64) NOT NULL DEFAULT '',
  `value` varchar(256) NOT NULL DEFAULT '',
  `comment` varchar(256) DEFAULT '',
  PRIMARY KEY (`script_name`,`key`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DELETE FROM `actions_config`;
INSERT INTO `actions_config` (`script_name`, `key`, `value`, `comment`) VALUES ('PlayerAnnouncer', 'Login.Format', '|cff00FF00[ |cffFFFFFFWorld |cff00FF00]: |cffFFFFFFPlayer %s |cffFFFFFFhas entered the world.|r', 'The message that shows when the player enters the world.');
INSERT INTO `actions_config` (`script_name`, `key`, `value`, `comment`) VALUES ('PlayerAnnouncer', 'Logout.Format', '|cffFF0000[ |cffFFFFFFWorld |cffFF0000]: |cffFFFFFFPlayer %s |cffFFFFFFhas left the world.|r', 'The message that shows when the player leaves the world.');
INSERT INTO `actions_config` (`script_name`, `key`, `value`, `comment`) VALUES ('AutoGuild', 'GuildId', '1', 'This is the guild id from the guild table.');
INSERT INTO `actions_config` (`script_name`, `key`, `value`, `comment`) VALUES ('PlayerAnnouncer', 'FirstLogin.Format', '|cff00FF00[ |cffFFFFFFWorld |cff00FF00]: |cffFFFFFFA new player %s |cffFFFFFFhas entered the world.|r', 'The message that shows when the player enters the world for the first time.');
