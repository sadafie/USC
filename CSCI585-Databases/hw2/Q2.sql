-- Sadaf Shahsahebi --
-- CSCI585 Assignment 2, Fall 2009 --
-- SID# 4512793270 --

USE csci585shahsahe;

SELECT person.name, doctor.salary 
FROM person, doctor 
WHERE person.ssn=doctor.ssn AND doctor.salary > (SELECT AVG(salary) FROM doctor);