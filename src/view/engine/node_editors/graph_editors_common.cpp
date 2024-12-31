
#include "graph_editors_common.h"

bool operator==(const NodeLinkKey& keyOne, const NodeLinkKey& keyTwo) {
	NodeLinkKeyHasher hasher{};
	return hasher(keyOne) == hasher(keyTwo);
}