
CREATE TABLE if not exists `data`.`lifter_id_describe` (
  `id` varchar(20) NOT NULL,
  `type` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


INSERT INTO `data`.`lifter_id_describe`
(`id`,
`type`)
VALUES
('0001','曳引式简易升降机')
,('0002','强制式简易升降机')
,('0003','sc双笼施工升降机')
,('1001','传感器')
,('1002','脉冲采集器rs485')
,('1003','继电器')
,('2001','安全触点1')
,('3001','客户端1');