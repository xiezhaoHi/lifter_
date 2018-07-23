CREATE TABLE  If Not Exists  `data`.`client_config` (
  `id` varchar(10) DEFAULT NULL,
  `address` varchar(45) NOT NULL,
  `belongs` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`address`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


INSERT INTO `data`.`client_config`
(`id`,
`address`,
`belongs`)
VALUES
('','192.168.1.102','00010001');