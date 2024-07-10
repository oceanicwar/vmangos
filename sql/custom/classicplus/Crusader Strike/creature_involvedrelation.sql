-- Quest End - Duthorian Rall
DELETE FROM `creature_involvedrelation` WHERE `id`=6171 AND `quest`=50001;
INSERT INTO `creature_involvedrelation` (`id`, `quest`, `patch_min`, `patch_max`) VALUES (6171, 50001, 0, 10);