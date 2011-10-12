-- Sadaf Shahsahebi --
-- CSCI585 Assignment 2, Fall 2009 --
-- SID# 4512793270 --

USE csci585shahsahe;

SELECT DISTINCT person.name, doctor.specialty 
FROM person, doctor 
WHERE person.ssn=doctor.ssn AND doctor.ssn IN (
	SELECT doctor 
	FROM works_in_clinic 
	GROUP BY doctor HAVING COUNT(*) >= 1
	) 
	AND doctor.ssn IN (
		SELECT doctor 
		FROM works_in_ward 
		GROUP BY doctor HAVING COUNT(*) >=1
		);