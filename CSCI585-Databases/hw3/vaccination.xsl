<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/vaccinations">
  <html>
  <body>
	<h2>Patient Group</h2>
    <table border="2" style="font-family:sans-serif; font-size:14px;">
      <tr style="font-weight:bold;">
        <th>Group</th>
        <th>Vaccine Quota</th>
      </tr>
      <xsl:for-each select="patientgroup">
        <tr style="background-color:yellow;">
          <td><xsl:value-of select="group"/></td>
          <td><xsl:value-of select="quota"/></td>
        </tr>
      </xsl:for-each>
    </table>

	<h2>Clinics</h2>
	<table border="1" style="font-family:times; font-size:12px;">
      <tr style="font-weight:bold;">
        <th>ID</th>
        <th>Name</th>
        <th>Address</th>
        <th>Groups Served</th>
        <th>Phone</th>
        <th>Fax</th>
        <th>Emp #</th>
      </tr>
      <xsl:for-each select="clinic">
        <tr style="background-color:green;">
          <td><xsl:value-of select="id"/></td>
          <td><xsl:value-of select="cname"/></td>
          <td><xsl:value-of select="address"/></td>
          <td><xsl:for-each select="group"><p><xsl:value-of select="."/></p></xsl:for-each></td>
          <td><xsl:for-each select="phone"><p><xsl:value-of select="."/></p></xsl:for-each></td>
          <td><xsl:for-each select="fax"><p><xsl:value-of select="."/></p></xsl:for-each></td>
          <td><xsl:value-of select="employees"/></td>
        </tr>
      </xsl:for-each>
    </table>

	<h2>Patients</h2>
	<table border="2" style="font-family:arial; font-size:10px;">
      <tr style="font-weight:bold;">
        <th>SSN</th>
        <th>Name</th>
        <th>DOB</th>
        <th>Gender</th>
        <th>Home Phone</th>
        <th>Cell Phone</th>
        <th>Clinic</th>
      </tr>
      <xsl:for-each select="patient">
        <tr style="background-color:pink;">
          <td><xsl:value-of select="ssn"/></td>
          <td><xsl:value-of select="pname"/></td>
          <td><xsl:value-of select="dob"/></td>
          <td><xsl:value-of select="gender"/></td>
          <td><xsl:value-of select="homephone"/></td>
          <td><xsl:value-of select="cellphone"/></td>
          <td><xsl:value-of select="clinic"/></td>
        </tr>
      </xsl:for-each>
    </table>
  </body>
  </html>
</xsl:template>

</xsl:stylesheet>