<?xml version="1.0"?>
<!--
 -  
  -  $Id$
 -
 -  This file is part of the OpenLink Software Virtuoso Open-Source (VOS)
 -  project.
 -  
 -  Copyright (C) 1998-2012 OpenLink Software
 -  
 -  This project is free software; you can redistribute it and/or modify it
 -  under the terms of the GNU General Public License as published by the
 -  Free Software Foundation; only version 2 of the License, dated June 1991.
 -  
 -  This program is distributed in the hope that it will be useful, but
 -  WITHOUT ANY WARRANTY; without even the implied warranty of
 -  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 -  General Public License for more details.
 -  
 -  You should have received a copy of the GNU General Public License along
 -  with this program; if not, write to the Free Software Foundation, Inc.,
 -  51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 -  
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:wfw="http://wellformedweb.org/CommentAPI/"
  xmlns:slash="http://purl.org/rss/1.0/modules/slash/"
  xmlns:content="http://purl.org/rss/1.0/modules/content/"
  xmlns:r="http://backend.userland.com/rss2"
  version="1.0">

<xsl:output indent="yes" doctype-public="+//IDN python.org//DTD XML Bookmark Exchange Language 1.0//EN//XML"
	doctype-system="http://pyxml.sourceforge.net/topics/dtds/xbel-1.0.dtd"/>


<!-- general element conversions -->

<xsl:template match="channel">
  <xsl:comment>XBEL based XML document generated By OpenLink Virtuoso</xsl:comment>
  <xbel>
      <title><xsl:value-of select="title"/></title>
      <folder id="{generate-id()}">
      <title><xsl:value-of select="description"/></title>
	      <xsl:apply-templates/>
	  </folder>
  </xbel>
</xsl:template>

<xsl:template match="item">
    <bookmark href="{link}" id="{generate-id()}">
	<title><xsl:value-of select="title"/></title>
    </bookmark>
</xsl:template>

<xsl:template match="text()"/>

</xsl:stylesheet>
