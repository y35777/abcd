#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "encrypt.h"
#define  MAX_CODE_LEN  128
char g_caCodeTbl[64+1];

int cvt_code (unsigned char);
int Rebuild_CodeTbl(unsigned char);
unsigned char MakeKey();
unsigned char GetKey(char *);
void Initial_CodeTbl();

void Initial_CodeTbl ()
{
  int i;

  for (i=0; i<64; i++)
  if (i == 0)
     g_caCodeTbl [i] = 'A';
  else
  if (i == 26)
     g_caCodeTbl [i] = 'a';
  else
  if (i == 52)
     g_caCodeTbl [i] = '0';
  else
  if (i == 62)
     g_caCodeTbl [i] = '+';
  else
  if (i == 63)
     g_caCodeTbl [i] = '/';
  else
     g_caCodeTbl [i] = g_caCodeTbl [i-1] + 1;

}

int encode (char *pcaPass, char *pcaCode)
{
  unsigned int   i, j;
  unsigned int   shift;
  unsigned long  accum, value;
  unsigned char  ucKey;

  Initial_CodeTbl ();
  ucKey = MakeKey ();
  Rebuild_CodeTbl (ucKey);

  j = 1;
  accum = value = shift = 0;

  pcaCode [0] = (unsigned char) ucKey;
  for (i=0; i<=strlen(pcaPass); i++ )
  {
     value = (unsigned long) pcaPass[i];
     accum <<= 8;
     accum |= value;
     shift += 8;

     while ( shift >= 6 )
     {
        shift -= 6;
        value = (accum >> shift);
        value &= 0x3F;
        pcaCode [j++] = g_caCodeTbl [value];
     }
  }
  
  return 0;
}


int decode (char *pcaCode, char *pcaPass)
{
  unsigned int   i, j;
  unsigned long  accum, value, shift;
  unsigned char  ucKey, caTmpCode [MAX_CODE_LEN+1];

  Initial_CodeTbl ();
  ucKey = GetKey (pcaCode);
  Rebuild_CodeTbl (ucKey);

  if(pcaCode[0] == '\0')
  {
      caTmpCode[0] = '\0';
  }
  else
  {
      strncpy((char*)caTmpCode, pcaCode+1, sizeof(caTmpCode));
      caTmpCode[sizeof(caTmpCode) - 1] = '\0';
  }

  accum = value = shift = j = 0;

  for ( i = 0; (caTmpCode [i] != '\n') && (caTmpCode [i] != '\0'); i++ )
  {
     value = cvt_code (caTmpCode [i]);

     if ( value < 64 )
     {
        accum <<= 6;
        shift += 6;
        accum |= value;

        if ( shift >= 8 )
        {
           shift -= 8;
           value = accum >> shift;
           value &= 0xFF;
           pcaPass [j++] = (unsigned char) value;
        }
     }
     else
     {
        break;
     }
  }
  return 0;
}


int cvt_code (unsigned char code)
{
  int i;

  for (i=0; i<64; i++)
      if (code == g_caCodeTbl[i])
         return (i);

  return (-1);
}


unsigned char MakeKey()
{
  unsigned char ucKey;
  unsigned int  iTime;
  iTime = time (NULL);
  srand (iTime % 1000);
  ucKey = rand () % 26 + 65;
  return (ucKey);
}


int Rebuild_CodeTbl (unsigned char ucKey)
{
  int i, j;
  char caBuf[64+1];

  memset (caBuf, 0, sizeof(caBuf));

  while (ucKey >= 64)
        ucKey -= 64;

  for (i=ucKey, j=0; i<64; i++, j++)
      caBuf[j] = g_caCodeTbl[i];

  for (i=0; i<ucKey; i++, j++)
      caBuf[j] = g_caCodeTbl[i];

  memcpy (g_caCodeTbl, caBuf, sizeof(caBuf));
  return 0;
}


unsigned char GetKey (char *pcaCode)
{
  unsigned char ucKey;
   
  ucKey = (unsigned char) pcaCode[0];

  return (ucKey);
}