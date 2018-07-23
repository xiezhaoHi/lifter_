CREATE TABLE  If Not Exists  `data`.`structure_stress` (
	`structure_stress_device_id` varchar(10) NOT NULL,
  `structure_stress_id` varchar(10) NOT NULL,
  `structure_stress_date` varchar(45) DEFAULT NULL,
  `structure_stress_time` varchar(45) DEFAULT NULL,
  `structure_stress_ms` varchar(45) DEFAULT NULL,
  `structure_stress_data` varchar(45) DEFAULT NULL,
  `save_date` varchar(45) DEFAULT NULL,
  `save_time` varchar(45) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


