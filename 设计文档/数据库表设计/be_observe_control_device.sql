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