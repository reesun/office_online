#
# Table structure for table 'Rsc'
#

DROP TABLE Rsc;

CREATE TABLE Rsc (
  RscID int(11) NOT NULL auto_increment,
  RscMD5 varchar(8) DEFAULT '' NOT NULL,
  RscTypeID int(11) DEFAULT '0' NOT NULL,
  RscName varchar(127) DEFAULT '' NOT NULL,
  IsURL smallint(6) DEFAULT '-1' NOT NULL,
  RscStateID int(11) DEFAULT '0' NOT NULL,
  InCache smallint(6) DEFAULT '-1' NOT NULL,
  Size int(11) DEFAULT '0' NOT NULL,
  PRIMARY KEY (RscID),
  KEY RscTypeID (RscTypeID),
  KEY RscMD5 (RscName),
  KEY RscName (RscName)
);

#
# Table structure for table 'RscComp'
#

DROP TABLE RscComp;

CREATE TABLE RscComp (
  RscCompID int(11) NOT NULL auto_increment,
  RscID int(11) DEFAULT '0' NOT NULL,
  RscCompTypeID int(11) DEFAULT '0' NOT NULL,
  Position int(11) DEFAULT '0' NOT NULL,
  Size int(11) DEFAULT '0' NOT NULL,
  Filename char(255),
  PRIMARY KEY (RscCompID),
  KEY RscID (RscID),
  KEY RscCompTypeID (RscCompTypeID),
  KEY Position (Position),
  KEY Size (Size)
);

#
# Table structure for table 'RscCompType'
#

DROP TABLE RscCompType;

CREATE TABLE RscCompType (
  RscCompTypeID int(11) NOT NULL auto_increment,
  RscCompType char(50) DEFAULT '' NOT NULL,
  PRIMARY KEY (RscCompTypeID)
);

#
# Dumping data for table 'RscCompType'
#

INSERT INTO RscCompType VALUES (1,'Gif');
INSERT INTO RscCompType VALUES (2,'Jpeg');
INSERT INTO RscCompType VALUES (3,'HTML');
INSERT INTO RscCompType VALUES (4,'Text');

#
# Table structure for table 'RscCompView'
#

DROP TABLE RscCompView;

CREATE TABLE RscCompView (
  RscCompViewID int(11) NOT NULL auto_increment,
  RscCompID int(11) DEFAULT '0' NOT NULL,
  req_date timestamp(14),
  PRIMARY KEY (RscCompViewID),
  KEY req_date (req_date),
  KEY RscCompID (RscCompID)
);

#
# Table structure for table 'RscFetch'
#

DROP TABLE RscFetch;

CREATE TABLE RscFetch (
  RscFetchID int(11) NOT NULL auto_increment,
  RscID int(11) DEFAULT '0' NOT NULL,
  FetchDate timestamp(14),
  FetchSeconds double(16,4) DEFAULT '0.0000' NOT NULL,
  FromCache smallint(6) DEFAULT '0' NOT NULL,
  Size int(11) DEFAULT '0' NOT NULL,
  PRIMARY KEY (RscFetchID),
  KEY RscID (RscID),
  KEY FetchDate (FetchDate)
);

#
# Table structure for table 'RscMail'
#

DROP TABLE RscMail;

CREATE TABLE RscMail (
  RscMailID int(11) NOT NULL auto_increment,
  RscMD5 varchar(8) DEFAULT '' NOT NULL,
  Position int(11) DEFAULT '0' NOT NULL,
  RendID int(11) DEFAULT '0' NOT NULL,
  Mailfrom varchar(127) DEFAULT '' NOT NULL,
  Mailto varchar(127) DEFAULT '' NOT NULL,
  Comment text,
  req_date timestamp(14),
  PRIMARY KEY (RscMailID),
  KEY RscMD5 (RscMD5),
  KEY Position (Position),
  KEY RendID (RendID),
  KEY Mailto (Mailto),
  KEY req_date (req_date)
);

#
# Table structure for table 'RscType'
#

DROP TABLE RscType;

CREATE TABLE RscType (
  RscTypeID int(11) NOT NULL auto_increment,
  RscType char(50) DEFAULT '' NOT NULL,
  PRIMARY KEY (RscTypeID)
);

#
# Dumping data for table 'RscType'
#

INSERT INTO RscType VALUES (1,'(Unknown)');
INSERT INTO RscType VALUES (2,'Postscript');
INSERT INTO RscType VALUES (3,'PDF');
INSERT INTO RscType VALUES (4,'Word');
INSERT INTO RscType VALUES (5,'RTF');
INSERT INTO RscType VALUES (6,'Jpeg');
INSERT INTO RscType VALUES (7,'Gif');

#
# Table structure for table 'RscTypeRend'
#

DROP TABLE RscTypeRend;

CREATE TABLE RscTypeRend (
  RscTypeRendID int(11) NOT NULL auto_increment,
  RscTypeID int(11) DEFAULT '0' NOT NULL,
  RendDefault smallint(6) DEFAULT '0' NOT NULL,
  RscTypeRend char(50),
  RendCmd char(127),
  PRIMARY KEY (RscTypeRendID),
  KEY RscTypeID (RscTypeID),
  KEY RendDefault (RendDefault)
);

#
# Dumping data for table 'RscTypeRend'
#

INSERT INTO RscTypeRend VALUES (1,1,-1,'Plain','');
INSERT INTO RscTypeRend VALUES (2,2,-1,'As images','');
INSERT INTO RscTypeRend VALUES (3,3,-1,'As images','');
INSERT INTO RscTypeRend VALUES (4,4,-1,'As html','');
INSERT INTO RscTypeRend VALUES (5,5,-1,'As html','');
