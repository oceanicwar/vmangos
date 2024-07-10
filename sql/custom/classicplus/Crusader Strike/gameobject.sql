-- Shovel Game Object
INSERT INTO `gameobject` (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecsmin`, `spawntimesecsmax`, `animprogress`, `state`, `spawn_flags`, `visibility_mod`, `patch_min`, `patch_max`) VALUES (180651, 0, -9594.51, 742.656, 32.9868, 4.08282, 0, 0, 0.89129, -0.453434, 25, 25, 100, 1, 0, 0, 0, 10);

-- Shallow Grave Game Objects
DELETE FROM `gameobject` WHERE `guid`=3998644;
INSERT INTO `gameobject` (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecsmin`, `spawntimesecsmax`, `animprogress`, `state`, `spawn_flags`, `visibility_mod`, `patch_min`, `patch_max`) VALUES (211065, 0, -9597.52, 741.727, 32.9848, 4.09618, 0, 0, 0.888242, -0.459377, 25, 25, 100, 1, 0, 0, 0, 10);
DELETE FROM `gameobject` WHERE `guid`=3998643;
INSERT INTO `gameobject` (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecsmin`, `spawntimesecsmax`, `animprogress`, `state`, `spawn_flags`, `visibility_mod`, `patch_min`, `patch_max`) VALUES (211065, 0, -9594.91, 739.099, 33.0567, 3.92339, 0, 0, 0.924567, -0.38102, 25, 25, 100, 1, 0, 0, 0, 10);
