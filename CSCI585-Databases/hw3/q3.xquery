<patients>
{
for $x in doc("vaccination.xml")/vaccinations/patient
where $x/dob>='2008-11-01'
return <patient>{$x/ssn}{$x/pname}{$x/dob}{$x/gender}</patient>
}
</patients>