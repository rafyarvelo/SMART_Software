<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">

  <xsl:output method="html" indent="yes"/>

  <xsl:template match="/*">
    <table>
      <thead>
        <tr>
          <xsl:apply-templates select="*[1]/*" mode="th"/>
        </tr>
      </thead>
      <tbody>
        <xsl:apply-templates select="*"/>
      </tbody>
    </table>
  </xsl:template>

  <xsl:template match="/*/*/*" mode="th">
    <th>
      <xsl:value-of select="name()"/>
    </th>
  </xsl:template>

  <xsl:template match="/*/*">
    <tr>
      <xsl:apply-templates select="*"/>
    </tr>
  </xsl:template>

  <xsl:template match="/*/*/*">
    <td>
      <xsl:value-of select="."/>
    </td>
  </xsl:template>

</xsl:stylesheet>
