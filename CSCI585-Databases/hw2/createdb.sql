-- Sadaf Shahsahebi --
-- CSCI585 Assignment 2, Fall 2009 --
-- SID# 4512793270 --

CREATE DATABASE IF NOT EXISTS csci585shahsahe;

USE csci585shahsahe;

START TRANSACTION;

CREATE TABLE `person` (
	`ssn` char(11) NOT NULL,
	`name` varchar(30),
	PRIMARY KEY (`ssn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `has_phone` (
	`person` char(11) NOT NULL,
	`phone` char(12) NOT NULL,
	PRIMARY KEY (`person`,`phone`),
	CONSTRAINT `phone_fk` FOREIGN KEY (`person`) REFERENCES `person` (`ssn`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `patient` (
	`ssn` char(11) NOT NULL,
	`dob` date,
	PRIMARY KEY (`ssn`),
	CONSTRAINT `patient_fk` FOREIGN KEY (`ssn`) REFERENCES `person` (`ssn`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `doctor` (
	`ssn` char(11) NOT NULL,
	`specialty` text,
	`salary` int(8),
	PRIMARY KEY (`ssn`),
	CONSTRAINT `doctor_fk` FOREIGN KEY (`ssn`) REFERENCES `person` (`ssn`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `has_doctor` (
	`patient` char(11) NOT NULL,
	`doctor` char(11) NOT NULL,
	PRIMARY KEY (`patient`,`doctor`),
	CONSTRAINT `has_doctor_fk1` FOREIGN KEY (`patient`) REFERENCES `patient` (`ssn`) ON DELETE CASCADE,
	CONSTRAINT `has_doctor_fk2` FOREIGN KEY (`doctor`) REFERENCES `doctor` (`ssn`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `clinic` (
	`c_no` int(10) NOT NULL,
	`name` varchar(30),
	`address` text,
	PRIMARY KEY (`c_no`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `ward` (
	`w_no` int(10) NOT NULL,
	`name` varchar(30),
	`location` text,
	PRIMARY KEY (`w_no`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `works_in_ward` (
	`doctor` char(11) NOT NULL,
	`location` int(10) NOT NULL,
	PRIMARY KEY (`doctor`,`location`),
	CONSTRAINT `works_in_ward_fk1` FOREIGN KEY (`doctor`) REFERENCES `doctor` (`ssn`) ON DELETE CASCADE,
	CONSTRAINT `works_in_ward_fk2` FOREIGN KEY (`location`) REFERENCES `ward` (`w_no`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `works_in_clinic` (
	`doctor` char(11) NOT NULL,
	`location` int(10) NOT NULL,
	PRIMARY KEY (`doctor`,`location`),
	CONSTRAINT `works_in_clinic_fk1` FOREIGN KEY (`doctor`) REFERENCES `doctor` (`ssn`) ON DELETE CASCADE,
	CONSTRAINT `works_in_clinic_fk2` FOREIGN KEY (`location`) REFERENCES `clinic` (`c_no`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `outpatient` (
	`ssn` char(11) NOT NULL,
	`date_visit` date,
	`reg_no` int(10) NOT NULL,
	`clinic` int(10),
	PRIMARY KEY (`reg_no`),
	CONSTRAINT `outpatient_fk1` FOREIGN KEY (`ssn`) REFERENCES `patient` (`ssn`) ON DELETE CASCADE,
	CONSTRAINT `outpatient_fk2` FOREIGN KEY (`clinic`) REFERENCES `clinic` (`c_no`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
CREATE TABLE `inpatient` (
	`ssn` char(11) NOT NULL,
	`date_in` date,
	`date_out` date,
	`ward` int(10),
	PRIMARY KEY (`ssn`),
	CONSTRAINT `inpatient_fk1` FOREIGN KEY (`ssn`) REFERENCES `patient` (`ssn`) ON DELETE CASCADE,
	CONSTRAINT `inpatient_fk2` FOREIGN KEY (`ward`) REFERENCES `ward` (`w_no`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

COMMIT;