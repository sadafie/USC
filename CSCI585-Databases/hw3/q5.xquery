<clinics>
{
for $a in doc("vaccination.xml")/vaccinations/clinic
let $s := for $b in $a/group let $q := for $x in doc("vaccination.xml")/vaccinations/patientgroup where $x/group=$b return $x/quota return $q
return <clinic>{$a/cname}<total>{sum($s)}</total></clinic>
}
</clinics>