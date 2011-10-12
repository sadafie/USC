-- Sadaf Shahsahebi --
-- CSCI585 Assignment 2, Fall 2009 --
-- SID# 4512793270 --

USE csci585shahsahe;

SELECT DISTINCT person.name, has_phone.phone 
FROM person, has_phone 
WHERE person.ssn=has_phone.person AND person.ssn IN (
	SELECT person 
	FROM has_phone 
	GROUP BY phone HAVING COUNT(*) = 1
	) 
	AND person.ssn IN (
		SELECT person 
		FROM has_phone 
		GROUP BY person HAVING COUNT(*) = 1
		);