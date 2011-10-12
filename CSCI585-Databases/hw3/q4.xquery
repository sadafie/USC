<clinics>
{
for $b in doc("vaccination.xml")/vaccinations/patientgroup[group="Healthcare Workers"]
for $a in doc("vaccination.xml")/vaccinations/clinic/group
let $p := $a/parent::node()
where $a="Healthcare Workers" and $b/quota<$p/employees
return <clinic>{$p/cname}{$p/address}<phone>{data($p/phone)}</phone></clinic>
}
</clinics>