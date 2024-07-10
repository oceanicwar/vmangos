-- Quest Start - Duthorian Rall
DELETE FROM `creature_questrelation` WHERE `id`=6171 AND `quest`=50001;
INSERT INTO `creature_questrelation` (`id`, `quest`, `patch_min`, `patch_max`) VALUES (6171, 50001, 0, 10);