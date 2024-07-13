-- Crusader Strike
DELETE FROM `skill_line_ability` WHERE `id`=50000 AND `build`=5875;
INSERT INTO `skill_line_ability` (`id`, `build`, `skill_id`, `spell_id`, `race_mask`, `class_mask`, `req_skill_value`, `superseded_by_spell`, `learn_on_get_skill`, `max_value`, `min_value`, `req_train_points`) VALUES (50000, 5875, 184, 50002, 0, 2, 1, 0, 0, 0, 0, 0);
