#ifndef __JSONHELPER_H__
#define __JSONHELPER_H__

#include "jsoncpp/json/json.h"
#include "MathGeoLib/include/MathGeoLib.h"


class JsonHelper
{
public:
	JsonHelper() {};

	void Fill(Json::Value&, float3);
};

#endif // !__JSONHELPER_H__
