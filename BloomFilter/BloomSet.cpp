#include "BloomSet.h"

BOOL BloomSet::checkBit(const CHAR *str, INT len)
{
	BOOL host = true, path = true, fname = true;
	const CHAR *tmphost = strchr(str, '/');
	const CHAR *tmppath = NULL;
	if(tmphost == NULL) //path == NULL
	{
		host = host && Host->checkBit(str, len);
		if(!host)
			Host->setBit(str, len);
	}
	else if(tmppath = strchr(tmphost + 1, '/'))  //path != NULL && host != NULL && fname =!= NULL 
	{
		host = host && Host->checkBit(str, tmphost - str);
		path = path && Path->checkBit(tmphost + 1, tmppath - tmphost - 1);
		fname = fname && FName->checkBit(tmppath + 1, len - (tmppath - str + 1));
		if(!(host && path && fname))
		{
			if(!host)
				Host->setBit(str, tmphost - str);
			if(!path)
				Path->setBit(tmphost + 1, tmppath - tmphost - 1);
			if(!fname)
				FName->setBit(tmppath + 1, len - (tmppath - str + 1));
		}
	}
	else  //fname == NULL
	{
		host = host && Host->checkBit(str, tmphost - str);
		path = path && Path->checkBit(tmphost + 1, len - (tmphost - str + 1));
		if(!host)
			Host->setBit(str, tmphost - str);
		if(!path)
			Path->setBit(tmphost + 1, len - (tmphost - str + 1));
	}
	return host && path && fname; 
}
