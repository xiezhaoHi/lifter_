CREATE DATABASE if not exists `data` ;


 CREATE TABLE  If Not Exists  data.`be_observe_control_device` (
  `id` varchar(10) NOT NULL,
  `name` varchar(256) DEFAULT NULL,
  `describe` varchar(256) DEFAULT NULL,
  `belongs` varchar(256) DEFAULT NULL,
  `binddevice` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;

insert into  data.be_observe_control_device (be_observe_control_device.id,be_observe_control_device.name,
be_observe_control_device.describe,be_observe_control_device.belongs,be_observe_control_device.binddevice) values
('20010002','安全触点','电梯安全触点','00010001','10030001')
,('20010003','安全触点','电梯安全触点','00010001','10030001')
,('20010004','安全触点','电梯安全触点','00010001','10030001')
,('20010005','安全触点','电梯安全触点','00010001','10030001')
,('20010006','安全触点','电梯安全触点','00010001','10030001')
,('20010007','安全触点','电梯安全触点','00010001','10030001')
,('20010008','安全触点','电梯安全触点','00010001','10030001')
,('20010009','安全触点','电梯安全触点','00010001','10030001')
,('20010010','安全触点','电梯安全触点','00010001','10030001')
,('20010011','安全触点','电梯安全触点','00010001','10030001')
,('20010012','安全触点','电梯安全触点','00010001','10030001')
,('20010013','安全触点','电梯安全触点','00010001','10030001')
,('20010014','安全触点','电梯安全触点','00010001','10030001')
,('20010015','安全触点','电梯安全触点','00010001','10030001')
,('20010016','安全触点','电梯安全触点','00010001','10030001')
;



CREATE TABLE  If Not Exists  `data`.`client_config` (
  `id` varchar(10) NOT NULL,
  `address` varchar(45) DEFAULT NULL,
  `belongs` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


INSERT INTO `data`.`client_config`
(`id`,
`address`,
`belongs`)
VALUES
('30010001','192.168.1.8','00010001');






CREATE TABLE  If Not Exists  data.`device_config` (
  `id` varchar(10) NOT NULL,
  `config` varchar(2048) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

INSERT INTO `data`.`device_config`
(`id`,
`config`)
VALUES
('10030002',"<?xml version='1.0' encoding='utf8'?>
<device_name  ID = '10030002'>
<DI_DATA>
<DI ID = '01' LINK='20010001'/>
<DI ID = '02' LINK='20010002'/>
<DI ID = '03' LINK='20010003'/>
<DI ID = '04' LINK='20010004'/>
<DI ID = '05' LINK='20010005'/>
<DI ID = '06' LINK='20010006'/>
<DI ID = '07' LINK='20010007'/>
<DI ID = '08' LINK='20010008'/>
<DI ID = '09' LINK='20010009'/>
<DI ID = '10' LINK='20010010'/>
<DI ID = '11' LINK='20010011'/>
<DI ID = '12' LINK='20010012'/>
<DI ID = '13' LINK='20010013'/>
<DI ID = '14' LINK='20010014'/>
<DI ID = '15' LINK='20010015'/>
<DI ID = '16' LINK='20010016'/>
</DI_DATA>
<DO_DATA>
<DO ID = '01' LINK='20020001'/>
<DO ID = '02' LINK='20020002'/>
<DO ID = '03' LINK='20020003'/>
<DO ID = '04' LINK='20020004'/>
<DO ID = '05' LINK='20020005'/>
<DO ID = '06' LINK='20020006'/>
<DO ID = '07' LINK='20020007'/>
<DO ID = '08' LINK='20020008'/>
<DO ID = '09' LINK='20020009'/>
<DO ID = '10' LINK='20020010'/>
<DO ID = '11' LINK='20020011'/>
<DO ID = '12' LINK='20020012'/>
<DO ID = '13' LINK='20020013'/>
<DO ID = '14' LINK='20020014'/>
<DO ID = '15' LINK='20020015'/>
<DO ID = '16' LINK='20020016'/>
</DO_DATA>
</device_name>");




INSERT INTO `data`.`device_config`
(`id`,
`config`)
VALUES
('10030001',"<?xml version='1.0' encoding='utf8'?>
<device_name  ID = '10030001'>
<DI_DATA>
<DI ID = '01' LINK='20010017'/>
<DI ID = '02' LINK='20010018'/>
<DI ID = '03' LINK='20010019'/>
<DI ID = '04' LINK='20010020'/>
<DI ID = '05' LINK='20010021'/>
<DI ID = '06' LINK='20010022'/>
<DI ID = '07' LINK='20010023'/>
<DI ID = '08' LINK='20010024'/>
<DI ID = '09' LINK='20010025'/>
<DI ID = '10' LINK='20010026'/>
<DI ID = '11' LINK='20010027'/>
<DI ID = '12' LINK='20010028'/>
<DI ID = '13' LINK='20010029'/>
<DI ID = '14' LINK='20010030'/>
<DI ID = '15' LINK='20010031'/>
<DI ID = '16' LINK='20010032'/>
</DI_DATA>
<DO_DATA>
<DO ID = '01' LINK='20020017'/>
<DO ID = '02' LINK='20020018'/>
<DO ID = '03' LINK='20020019'/>
<DO ID = '04' LINK='20020020'/>
<DO ID = '05' LINK='20020021'/>
<DO ID = '06' LINK='20020022'/>
<DO ID = '07' LINK='20020023'/>
<DO ID = '08' LINK='20020024'/>
<DO ID = '09' LINK='20020025'/>
<DO ID = '10' LINK='20020026'/>
<DO ID = '11' LINK='20020027'/>
<DO ID = '12' LINK='20020028'/>
<DO ID = '13' LINK='20020029'/>
<DO ID = '14' LINK='20020030'/>
<DO ID = '15' LINK='20020031'/>
<DO ID = '16' LINK='20020032'/>
</DO_DATA>
</device_name>");

INSERT INTO `data`.`device_config`
(`id`,
`config`)
VALUES
('10040001',
"<?xml version='1.0' encoding='utf8'?>
<device_name  ID = '10040001'>
<cannum>0</cannum>
<connect>1</connect>
<devind>0</devind>
<port>4001</port>
</device_name>");



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
,('10020001','编码器','','192.168.1.110','26','00010001')
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
