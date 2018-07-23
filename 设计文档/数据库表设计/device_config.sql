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