-- Sadaf Shahsahebi --
-- CSCI585 Assignment 2, Fall 2009 --
-- SID# 4512793270 --

USE csci585shahsahe;

SELECT person.name 
FROM person, patient 
WHERE patient.dob IN (
	SELECT MIN(patient.dob) 
	FROM patient
	) 
	AND patient.ssn=person.ssn;