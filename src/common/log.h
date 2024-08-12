#ifndef VN_LOG_H
#define VN_LOG_H


// need a way to set and remove warnings, such as 
// removed texture index from texture store used
// should be set on discovery and unset on user-resolution
class EngineLog {
public:
	void registerWarning() {}
	void unregisterWarning() {}
};

#endif // VN_LOG_H
