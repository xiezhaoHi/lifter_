CREATE TABLE if not exists data.`observe_control_device` (
  `id` varchar(10) NOT NULL,
  `name` varchar(256) DEFAULT NULL,
  `describe` varchar(256) DEFAULT NULL,
  `ip_address` varchar(256) DEFAULT NULL,
  `port` varchar(256) DEFAULT NULL,
  `belongs` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


INSERT INTO `data`.`observe_control_device`
(`id`,
`name`,
`describe`,
`ip_address`,
`port`,
`belongs`)
VALUES
('10010001','传感器','','192.168.1.100','','00010001')
,('10020001','编码器','','192.168.1.110','','00010001')
,('10030001','继电器','','192.168.1.120','','00010001')
;


INSERT INTO `data`.`observe_control_device`
(`id`,
`name`,
`describe`,
`ip_address`,
`port`,
`belongs`)
VALUES
('10040001','CAN','','192.168.1.118','','00010001');
