#include "JsonHelper.h"

void JsonHelper::Fill(Json::Value& json, const float3& vector) const
{
	json.append(vector.x).asFloat();
	json.append(vector.y).asFloat();
	json.append(vector.z).asFloat();
}

void JsonHelper::Fill(float3& vector, const Json::Value& json) const
{
	vector.x = json[0].asFloat();
	vector.y = json[1].asFloat();
	vector.z = json[2].asFloat();
}