-- Sadaf Shahsahebi --
-- CSCI585 Assignment 2, Fall 2009 --
-- SID# 4512793270 --

USE csci585shahsahe;

SELECT DISTINCT person.name 
FROM person, outpatient, clinic 
WHERE clinic.name='USC University Hospital' AND outpatient.clinic=clinic.c_no AND person.ssn=outpatient.ssn;

-- Alternate, equivalent query: --
-- select name from person where ssn in (select distinct ssn from outpatient where clinic in (select c_no from clinic where name='USC University Hospital')); --