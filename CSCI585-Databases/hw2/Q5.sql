-- Sadaf Shahsahebi --
-- CSCI585 Assignment 2, Fall 2009 --
-- SID# 4512793270 --

USE csci585shahsahe;

SELECT clinic.name 
FROM clinic, works_in_clinic 
WHERE clinic.c_no=works_in_clinic.location 
GROUP BY works_in_clinic.location HAVING COUNT(*) > 2;