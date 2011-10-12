-- Sadaf Shahsahebi --
-- CSCI585 Assignment 2, Fall 2009 --
-- SID# 4512793270 --

USE csci585shahsahe;

SELECT person.name 
FROM person, doctor 
WHERE person.ssn=doctor.ssn AND doctor.ssn IN (
	SELECT doctor 
	FROM has_doctor 
	GROUP BY doctor HAVING COUNT(*) > 1
	);