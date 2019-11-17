#include "JsonHelper.h"

void JsonHelper::Fill(Json::Value& json, float3 vector)
{
	json.append(vector.x).asFloat();
	json.append(vector.y).asFloat();
	json.append(vector.z).asFloat();
}