
#ifndef JSON_EXPR_H
#define JSON_EXPR_H

#include <vector>

class JsonExpr {

};

class JsonObject : JsonExpr {
public:
	void JsonObject(std::vector<JsonKeyPair> keyPairs);
	std::vector<JsonKeyPair> keyPairs;
};


template <typename ValueType>
class JsonKeyPair : JsonExpr {
public:
	void JsonKeyPair(JsonString key, JsonValue<ValueType> value);
	JsonString key;
	JsonValue<ValueType> value;
};

template <typename ValueType>
class JsonValue : JsonValue {
public:
	void JsonValue(ValueType value);
	ValueType value;
};

template <typename ListType>
class JsonList : JsonExpr {
public:
	void JsonList(std::vector<ListType> items);
	ListType items;
};

class JsonString : JsonExpr {

};

#endif // JSON_EXPR_H