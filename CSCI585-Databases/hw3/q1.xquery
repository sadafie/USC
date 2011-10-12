<clinics>
{
for $x in doc("vaccination.xml")/vaccinations/clinic
return <clinic>{$x/cname}{$x/address}<phone>
{
for $y in $x/phone
return string(concat($y,","))
}
</phone><num_groups>{count($x/group)}</num_groups></clinic>
}
</clinics>