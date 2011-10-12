<patients>
{
for $x in doc("vaccination.xml")/vaccinations/patient
where $x/clinic='MUC'
order by $x/pname descending
return <patient>{$x/ssn}{$x/pname}{$x/dob}{$x/cellphone}</patient>
}
</patients>