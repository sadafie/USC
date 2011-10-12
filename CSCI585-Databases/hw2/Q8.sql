-- Sadaf Shahsahebi --
-- CSCI585 Assignment 2, Fall 2009 --
-- SID# 4512793270 --

USE csci585shahsahe;

SELECT DISTINCT person.name 
FROM person,works_in_ward 
WHERE person.ssn=works_in_ward.doctor GROUP BY works_in_ward.doctor HAVING COUNT(*) IN (
	SELECT count(*) 
	FROM ward
	);