#ifndef __JSONHELPER_H__
#define __JSONHELPER_H__

#include "jsoncpp/json/json.h"
#include "MathGeoLib/include/MathGeoLib.h"


class JsonHelper
{
public:
	JsonHelper() {};

	void Fill(Json::Value&, const float3&) const;
	void Fill(float3&, const Json::Value&) const;
};

#endif // !__JSONHELPER_H__
