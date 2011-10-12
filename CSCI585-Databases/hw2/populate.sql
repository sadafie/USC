-- Sadaf Shahsahebi --
-- CSCI585 Assignment 2, Fall 2009 --
-- SID# 4512793270 --

USE csci585shahsahe;

START TRANSACTION;

INSERT INTO `person` (ssn,name) VALUES
('111-11-1111', 'Joe Smith'),
('222-22-2222', 'Jane Doe'),
('333-33-3333', 'Dr. Sally James'),
('444-44-4444', 'Dr. Peter Keys'),
('555-55-5555', 'Jack Peterson'),
('666-66-6666', 'Sue Jasper'),
('777-77-7777', 'Dr. Kim Scott');

INSERT INTO `has_phone` (person,phone) VALUES
('111-11-1111', '111-111-1111'),
('111-11-1111', '200-200-2000'),
('333-33-3333', '200-200-2000'),
('444-44-4444', '400-400-4000');

INSERT INTO `patient` (ssn,dob) VALUES
('111-11-1111', '1980-01-01'),
('222-22-2222', '1970-01-01'),
('555-55-5555', '1960-01-01');

INSERT INTO `doctor` (ssn,specialty,salary) VALUES
('333-33-3333', 'Orthopedics', 700000),
('444-44-4444', 'Pediatrics', 400000),
('777-77-7777', 'Family Medicine', 360000);

INSERT INTO `has_doctor` (patient,doctor) VALUES
('111-11-1111', '333-33-3333'),
('222-22-2222', '333-33-3333'),
('555-55-5555', '777-77-7777');

INSERT INTO `clinic` (c_no,name,address) VALUES
(1234, 'USC University Hospital', ''),
(56786, 'Stanford Medical Clinic', '4000 Campus Drive, Stanford, CA');

INSERT INTO `ward` (w_no,name,location) VALUES
(13, 'Orthopedics', 'Stanford Medical Clinic'),
(2, 'Family Medicine', 'USC University Hospital'),
(909, 'Pediatrics', 'USC University Hospital');

INSERT INTO `works_in_ward` (doctor,location) VALUES
('333-33-3333', 2),
('333-33-3333', 909),
('333-33-3333', 13),
('444-44-4444', 13),
('777-77-7777', 13);

INSERT INTO `works_in_clinic` (doctor,location) VALUES
('333-33-3333', 1234),
('333-33-3333', 56786),
('777-77-7777', 56786),
('444-44-4444', 56786);

INSERT INTO `outpatient` (ssn,date_visit,reg_no,clinic) VALUES
('111-11-1111', '2009-09-01', 100, 1234),
('111-11-1111', '2009-09-02', 101, 1234),
('111-11-1111', '2008-02-02', 12, 56786),
('222-22-2222', '2009-05-12', 88, 1234),
('555-55-5555', '2008-12-19', 30, 56786);

INSERT INTO `inpatient` (ssn,date_in,date_out,ward) VALUES
('222-22-2222', '2009-05-01', '2009-05-02', 2),
('555-55-5555', '2009-11-03', '2009-11-09', 13);

COMMIT;