/*
	long_time.c

	Custom serializer for xsd:time stored in an ULONG64 with usec
	(microsecond) precision

	See long_time.h for documentation.

	Compile this file and link it with your code.

gSOAP XML Web services tools
Copyright (C) 2000-2015, Robert van Engelen, Genivia Inc., All Rights Reserved.
This part of the software is released under ONE of the following licenses:
GPL or the gSOAP public license.
--------------------------------------------------------------------------------
gSOAP public license.

The contents of this file are subject to the gSOAP Public License Version 1.3
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at
http://www.cs.fsu.edu/~engelen/soaplicense.html
Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the License.

The Initial Developer of the Original Code is Robert A. van Engelen.
Copyright (C) 2000-2015, Robert van Engelen, Genivia, Inc., All Rights Reserved.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org

This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

/* When using soapcpp2 option -q<name> or -p<name>, you need to change "soapH.h" below */

/* include soapH.h generated by soapcpp2 from .h file containing typedef ULONG64 xsd__time */
#ifdef SOAP_H_FILE      /* if set, use the soapcpp2-generated fileH.h file as specified with: cc ... -DSOAP_H_FILE=fileH.h */
# include "stdsoap2.h"
# include SOAP_XSTRINGIFY(SOAP_H_FILE)
#else
# include "soapH.h"	/* or manually replace with soapcpp2-generated *H.h file */
#endif

SOAP_FMAC3 void SOAP_FMAC4 soap_default_xsd__time(struct soap *soap, ULONG64 *a)
{
  (void)soap; /* appease -Wall -Werror */
  *a = 0;
}

SOAP_FMAC3 const char * SOAP_FMAC4 soap_xsd__time2s(struct soap *soap, ULONG64 a)
{
  unsigned int H, M, S, f;
  f = a % 1000000;
  a /= 1000000;
  S = a % 60;
  a /= 60;
  M = a % 60;
  a /= 60;
  H = a % 24;
#ifndef WITH_NOZONE
  if (f)
    (SOAP_SNPRINTF(soap->tmpbuf, sizeof(soap->tmpbuf), 17), "%02u:%02u:%02u.%06uZ", H, M, S, f);
  else
    (SOAP_SNPRINTF(soap->tmpbuf, sizeof(soap->tmpbuf), 10), "%02u:%02u:%02uZ", H, M, S);
#else
  if (f)
    (SOAP_SNPRINTF(soap->tmpbuf, sizeof(soap->tmpbuf), 16), "%02u:%02u:%02u.%06u", H, M, S, f);
  else
    (SOAP_SNPRINTF(soap->tmpbuf, sizeof(soap->tmpbuf), 9), "%02u:%02u:%02u", H, M, S);
#endif
  return soap->tmpbuf;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_xsd__time(struct soap *soap, const char *tag, int id, const ULONG64 *a, const char *type)
{
  if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_xsd__time), type)
   || soap_string_out(soap, soap_xsd__time2s(soap, *a), 0))
    return soap->error;
  return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_s2xsd__time(struct soap *soap, const char *s, ULONG64 *a)
{
  char *r;
  ULONG64 H = 0, M = 0, S = 0;
  float f = 0;
  *a = 0;
  if (s)
  {
    H = soap_strtoul(s, &r, 10);
    if (!r || *r != ':')
      return soap->error = SOAP_TYPE;
    s = r + 1;
    M = soap_strtoul(s, &r, 10);
    if (!r || *r != ':')
      return soap->error = SOAP_TYPE;
    s = r + 1;
    S = soap_strtoul(s, &r, 10);
    if (!r)
      return soap->error = SOAP_TYPE;
    s = r;
    if (*s == '.')
    {
#if defined(WITH_C_LOCALE) && defined(HAVE_STRTOD_L)
# ifdef WIN32
      f = (float)_strtod_l(s, NULL, SOAP_LOCALE(soap));
# else
      f = (float)strtod_l(s, NULL, SOAP_LOCALE(soap));
# endif
#elif defined(HAVE_STRTOD)
      f = (float)strtod(s, NULL);
#elif defined(WITH_C_LOCALE) && defined(HAVE_STRTOF_L)
      f = strtof_l((char*)s, NULL, SOAP_LOCALE(soap));
#elif defined(HAVE_STRTOF)
      f = strtof((char*)s, NULL);
#endif
      for (s++; *s; s++)
        if (*s < '0' || *s > '9')
          break;
    }
    if (*s == ' ' && !(soap->mode & SOAP_XML_STRICT))
      s++;
    if (*s == 'Z' || *s == 'z')
    {
      s++;
    }
    else if (*s == '+' || *s == '-')
    {
      int c = *s;
      int h = 0, m = 0;
      h = soap_strtol(s, &r, 10);
      if (!r || *r != ':')
	return soap->error = SOAP_TYPE;
      s = r + 1;
      m = soap_strtol(s, &r, 10);
      if (!r)
	return soap->error = SOAP_TYPE;
      s = r;
      H += 24 - h;
      if (c == '-')
      {
	M += m;
        if (M >= 60)
        {
          M -= 60;
          H++;
        }
      }
      else
      {
        M += 60 - m;
        if (M < 60)
          H += 23;
        else
          M -= 60;
      }
      H %= 24;
    }
    if (*s)
      return soap->error = SOAP_TYPE;
    /* convert H:M:S.f to unsigned long long int microseconds */
    *a = ((((((H * 60) + M) * 60) + S) * 1000000) + (ULONG64)(f * 1000000.0 + 0.5));
  }
  return soap->error;
}

SOAP_FMAC3 ULONG64 * SOAP_FMAC4 soap_in_xsd__time(struct soap *soap, const char *tag, ULONG64 *a, const char *type)
{
  if (soap_element_begin_in(soap, tag, 0, NULL))
    return NULL;
  if (*soap->type
   && soap_match_att(soap, soap->type, type)
   && soap_match_att(soap, soap->type, ":time"))
  {
    soap->error = SOAP_TYPE;
    soap_revert(soap);
    return NULL;
  }
  a = (ULONG64*)soap_id_enter(soap, soap->id, a, SOAP_TYPE_xsd__time, sizeof(ULONG64), NULL, NULL, NULL, NULL);
  if (!a)
    return NULL;
  if (!*soap->href)
  {
    int err = soap_s2xsd__time(soap, soap_value(soap), a);
    if ((soap->body && soap_element_end_in(soap, tag)) || err)
      return NULL;
  }
  else
  {
    a = (ULONG64*)soap_id_forward(soap, soap->href, a, 0, SOAP_TYPE_xsd__time, 0, sizeof(ULONG64), 0, NULL, NULL);
    if (soap->body && soap_element_end_in(soap, tag))
      return NULL;
  }
  return a;
}
