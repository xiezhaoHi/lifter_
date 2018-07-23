CREATE TABLE if not exists data.`lifter_config` (
  `id` varchar(10) NOT NULL,
  `name` varchar(256) DEFAULT NULL,
  `describe` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


insert into  data.lifter_config(lifter_config.id,lifter_config.name,lifter_config.describe) values
('00010001','曳引式简易升降机1','描述')
,('00020001','强制式式简易升降机1','描述')
,('00030001','sc双笼施工升降机1','描述')
;